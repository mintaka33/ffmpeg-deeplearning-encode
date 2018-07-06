#include "detector.h"

int main()
{
    String cfgFile = "MobileNetSSD_deploy.prototxt.txt";
    String modelFile = "MobileNetSSD_deploy.caffemodel";

    DNNDetector detector;
    detector.initNet(cfgFile, modelFile, "caffe");
    std::vector<ObjectInfo> objInfo;

    // Open a video file or an image file or a camera stream.
    String inputFile = "test.mp4";
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