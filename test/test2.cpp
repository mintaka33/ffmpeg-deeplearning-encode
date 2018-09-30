#include "detector.h"

int main(int argc, char ** argv)
{
    String cfgFile = "../../models/MobileNetSSD_deploy.prototxt.txt";
    String modelFile = "../../models/MobileNetSSD_deploy.caffemodel";

    DNNDetector detector;
    detector.initNet(cfgFile, modelFile, "caffe");
    std::vector<ObjectInfo> objInfo;

    // Open a video file or an image file or a camera stream.
    String inputFile = "../../test/test.mp4";
    VideoCapture cap;
    cap.open(inputFile);

    Mat frame;
    for (int i=0; i<100; i++)
    {
        cap >> frame;
        if (frame.empty())
        {
            break;
        }

        objInfo = detector.detectFrame(&frame);

        std::cout << "******** Frame # " << i << std::endl;
        for (auto o: objInfo)
        {
            std::cout <<o.name<<" ("<<o.confidence<<") "<<": "<< "[" << o.left << ", " << o.top << ", " 
                << o.right << ", " << o.bottom << "]" << std::endl;
        }
    }

    return 0;
}