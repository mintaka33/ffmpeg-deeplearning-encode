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