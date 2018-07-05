#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace odc
{
using namespace cv;
using namespace dnn;

struct ObjectInfo
{
    std::string name;
    double confidence;
    int left;
    int top;
    int right;
    int bottom;
};

class DNNDetector
{
public:
    DNNDetector();
    ~DNNDetector();

    int initNet(String cfgFile, String modelFile, String framework);
    std::vector<ObjectInfo>& detectFrame(Mat* inputFrame);

private:
    std::vector<String> DNNDetector::getOutputsNames(const Net& net);
    void DNNDetector::postprocess(Mat& frame, const std::vector<Mat>& outs, Net& net);
    void addObject(int id, double confidence, int left, int top, int right, int bottom);
    std::string getName(int classId);

private:
    float confThreshold = 0.5; // Confidence threshold
    float scale = (float)0.00392; // (1/255)
    Scalar mean = {0.0, 0.0, 0.0, 0.0};
    bool swapRB = false;
    int inpWidth = 300;
    int inpHeight = 300;
    int backend = 0; // 0: default C++ backend; 1: Halide language; 2: Intel CVSDK Inference Engine
    int target = 0;  // 0: CPU target (by default); 1: OpenCL"

    Net net;
    std::vector<std::string> classes;
    std::vector<ObjectInfo> objects;
    std::vector<std::string> vocNames; // class name of Pascal VOC training set
    std::vector<std::string> cocoNames; // class name of VOCO training set
};
}
