
#include "detector.h"

namespace odc
{

DNNDetector::DNNDetector()
{
    vocNames = { "aeroplane", "bicycle", "bird", "boat",
        "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
        "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
        "sofa", "train", "tvmonitor", "background",
    };
}

DNNDetector::~DNNDetector()
{
}

int DNNDetector::initNet(String cfgFile, String modelFile, String framework)
{
    // Load a model
    net = readNet(modelFile, cfgFile, framework);
    net.setPreferableBackend(backend);
    net.setPreferableTarget(target);

    return 0;
}

std::vector<ObjectInfo>& DNNDetector::detectFrame(Mat* inputFrame)
{
    Mat frame, blob;
    frame = *inputFrame;

    // Create a 4D blob from a frame.
    Size inpSize(inpWidth > 0 ? inpWidth : frame.cols,
        inpHeight > 0 ? inpHeight : frame.rows);

    blobFromImage(frame, blob, scale, inpSize, mean, swapRB, false);

    // Run a model.
    net.setInput(blob);
    if (net.getLayer(0)->outputNameToIndex("im_info") != -1)  // Faster-RCNN or R-FCN
    {
        resize(frame, frame, inpSize);
        Mat imInfo = (Mat_<float>(1, 3) << inpSize.height, inpSize.width, 1.6f);
        net.setInput(imInfo, "im_info");
    }
    std::vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));

    objects.clear();

    postprocess(frame, outs, net);

    return objects;
}

void DNNDetector::addObject(int id, double confidence, int left, int top, int right, int bottom)
{
    ObjectInfo obj = {};
    obj.name = getName(id);
    obj.confidence = confidence;
    obj.left = left;
    obj.top = top;
    obj.right = right;
    obj.bottom = bottom;

    objects.push_back(obj);
}

std::string DNNDetector::getName(int classId)
{
    if (classId >= vocNames.size())
    {
        return "Unknown";
    }
    else
    {
        return vocNames[classId];
    }
}

std::vector<String> DNNDetector::getOutputsNames(const Net& net)
{
    static std::vector<String> names;
    if (names.empty())
    {
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<String> layersNames = net.getLayerNames();
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

void DNNDetector::postprocess(Mat& frame, const std::vector<Mat>& outs, Net& net)
{
    static std::vector<int> outLayers = net.getUnconnectedOutLayers();
    static std::string outLayerType = net.getLayer(outLayers[0])->type;

    if (net.getLayer(0)->outputNameToIndex("im_info") != -1)  // Faster-RCNN or R-FCN
    {
        // Network produces output blob with a shape 1x1xNx7 where N is a number of
        // detections and an every detection is a vector of values
        // [batchId, classId, confidence, left, top, right, bottom]
        CV_Assert(outs.size() == 1);
        float* data = (float*)outs[0].data;
        for (size_t i = 0; i < outs[0].total(); i += 7)
        {
            float confidence = data[i + 2];
            if (confidence > confThreshold)
            {
                int left = (int)data[i + 3];
                int top = (int)data[i + 4];
                int right = (int)data[i + 5];
                int bottom = (int)data[i + 6];
                int classId = (int)(data[i + 1]) - 1;  // Skip 0th background class id.

                addObject(classId, confidence, left, top, right, bottom);
            }
        }
    }
    else if (outLayerType == "DetectionOutput")
    {
        // Network produces output blob with a shape 1x1xNx7 where N is a number of
        // detections and an every detection is a vector of values
        // [batchId, classId, confidence, left, top, right, bottom]
        CV_Assert(outs.size() == 1);
        float* data = (float*)outs[0].data;
        for (size_t i = 0; i < outs[0].total(); i += 7)
        {
            float confidence = data[i + 2];
            if (confidence > confThreshold)
            {
                int left = (int)(data[i + 3] * frame.cols);
                int top = (int)(data[i + 4] * frame.rows);
                int right = (int)(data[i + 5] * frame.cols);
                int bottom = (int)(data[i + 6] * frame.rows);
                int classId = (int)(data[i + 1]) - 1;  // Skip 0th background class id.

                addObject(classId, confidence, left, top, right, bottom);
            }
        }
    }
    else if (outLayerType == "Region")
    {
        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<Rect> boxes;
        for (size_t i = 0; i < outs.size(); ++i)
        {
            // Network produces output blob with a shape NxC where N is a number of
            // detected objects and C is a number of classes + 4 where the first 4
            // numbers are [center_x, center_y, width, height]
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
            {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                Point classIdPoint;
                double confidence;
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold)
                {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    addObject(0, confidence, left, top, left + width, top + height);

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(Rect(left, top, width, height));
                }
            }
        }
        std::vector<int> indices;
        NMSBoxes(boxes, confidences, confThreshold, 0.4f, indices);
        for (size_t i = 0; i < indices.size(); ++i)
        {
            int idx = indices[i];
            Rect box = boxes[idx];
        }
    }
    else
    {
        CV_Error(Error::StsNotImplemented, "Unknown output layer type: " + outLayerType);
    }
}

}