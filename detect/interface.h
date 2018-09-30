#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct _RegionInfo
{
    int detected;
    char name[256];
    double confidence;
    int x;
    int y;
    int w;
    int h;
};

typedef struct _RegionInfo RegionInfo;
typedef struct DNNDetector DNNDetector;

DNNDetector* create_detector();
int init_detector(DNNDetector* d);
int detect_frame(DNNDetector* d, char* buf_y, char* buf_uv, int pitch, int height, RegionInfo* reg);
void destroy_detector();

#ifdef __cplusplus
}
#endif