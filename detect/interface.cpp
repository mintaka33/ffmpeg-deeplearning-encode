#include "interface.h"
#include "detector.h"

extern "C" {

DNNDetector* create_detector()
{
    return nullptr;
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