#include "interface.h"
#include "detector.h"

extern "C" {

static char* dtr_yv12_buf;
DNNDetector g_detector;
static int g_init = 0;

int nv12_to_yv12(char* src_y, char* src_uv, char* dst, int pitch, int height)
{
    if (!src_y || !src_y || !dst)
        return -1;

    // copy y plane
    memcpy(dst, src_y, pitch * height);

    // copy uv plane
    int uv_width = pitch /2;
    int uv_height = height / 2;
    char* dst_u = dst + pitch * height;
    char* dst_v = dst_u + uv_width * uv_height;
    for (int i=0; i<height/2; i++)
    {
        for (int j = 0; j < pitch / 2; j++)
        {
            dst_v[i*uv_width + j] = src_uv[i*uv_width * 2 + j * 2];
            dst_u[i*uv_width + j] = src_uv[i*uv_width * 2 + j * 2 + 1];
        }
    }

    return 0;
}

DNNDetector* create_detector()
{
    DNNDetector* p = &g_detector; //new DNNDetector();
    return p;
}

int init_detector(DNNDetector* d)
{
    std::string cfgFile = "/home/fresh/data/work/VideoCPET/models/MobileNetSSD_deploy.prototxt.txt";
    std::string modelFile = "/home/fresh/data/work/VideoCPET/models/MobileNetSSD_deploy.caffemodel";
    std::string framework = "caffe";

    if (g_init)
        return 0;
    
    if(!d)
        return -1;

    d->initNet(cfgFile, modelFile, framework);
    g_init = 1;

    return 0;
}

int detect_frame(DNNDetector* d, char* buf_y, char* buf_uv, int pitch, int height, RegionInfo* reg)
{
    int size = pitch * height * 3 / 2;

    if(!dtr_yv12_buf) {
        dtr_yv12_buf = (char*)malloc(size);
        if (!dtr_yv12_buf)
            return -1;
    }

    if(nv12_to_yv12(buf_y, buf_uv, dtr_yv12_buf, pitch, height) != 0)
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
            if (o.name == "person" && o.confidence >0.5)
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