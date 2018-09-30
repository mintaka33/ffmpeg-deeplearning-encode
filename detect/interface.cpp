#include "interface.h"
#include "detector.h"

extern "C" {

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

int detect_frame(DNNDetector* d)
{
    return 0;
}

void destroy_detector()
{

}

}