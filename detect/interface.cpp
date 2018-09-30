#include "interface.h"
#include "detector.h"

extern "C" {

static char* dtr_yv12_buf;

int yv12_to_nv12(char* srcnv12, char* dstyv12, int pitch, int height)
{
    if (!srcnv12 || !dstyv12)
        return -1;

    // copy y plane
    memcpy(dstyv12, srcnv12, pitch * height);

    // copy uv plane
    int uvWidth = pitch /2;
    int uvHeight = height / 2;
    char* srcUV = srcnv12 + pitch * height;
    char* dstU = dstyv12 + pitch * height;
    char* dstV = dstU + uvWidth * uvHeight;
    for (int i=0; i<height/2; i++)
    {
        for (int j = 0; j < pitch / 2; j++)
        {
            dstV[i*uvWidth + j] = srcUV[i*uvWidth * 2 + j * 2];
            dstU[i*uvWidth + j] = srcUV[i*uvWidth * 2 + j * 2 + 1];
        }
    }

    return 0;
}

DNNDetector* create_detector()
{
    DNNDetector* p = new DNNDetector();
    return p;
}

int init_detector(DNNDetector* d)
{
    std::string cfgFile = "/home/fresh/data/work/VideoCPET/models/MobileNetSSD_deploy.prototxt.txt";
    std::string modelFile = "/home/fresh/data/work/VideoCPET/models/MobileNetSSD_deploy.caffemodel";
    std::string framework = "caffe";
    
    if(!d)
        return -1;

    d->initNet(cfgFile, modelFile, framework);

    return 0;
}

int detect_frame(DNNDetector* d, char* buf, int pitch, int height, RegionInfo* reg)
{
    int size = pitch * height * 3 / 2;

    if(!dtr_yv12_buf) {
        dtr_yv12_buf = (char*)malloc(size);
        if (!dtr_yv12_buf)
            return -1;
    }

    if(yv12_to_nv12(buf, dtr_yv12_buf, pitch, height) != 0)
        return -1;

    Mat frame = Mat(height * 3 / 2, pitch, CV_8UC1, dtr_yv12_buf);
    if (!frame.empty())
    {
        Mat frameBGR;
        cvtColor(frame, frameBGR, 99); // CV_YUV2BGR_YV12 = 99

        //imwrite("test.bmp", frameBGR);

        std::vector<ObjectInfo> objInfo;
        objInfo = d->detectFrame(&frameBGR);

        bool detected = false;
        ObjectInfo roiInfo = {};
        for (auto o : objInfo)
        {
            if (o.name == "person" && o.confidence >0.8)
            {
                detected = true;
                roiInfo = o;
                break;
            }
        }

        if (detected) {
            reg->detected = true;
            memcpy(reg->name, roiInfo.name.c_str(), roiInfo.name.length());
            reg->confidence = roiInfo.confidence;
            reg->x = roiInfo.left;
            reg->y = roiInfo.top;
            reg->w = roiInfo.right - roiInfo.left;
            reg->h = roiInfo.bottom - roiInfo.top;
        }
    }

    return 0;
}

void destroy_detector()
{

}

}