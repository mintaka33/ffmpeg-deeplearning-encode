#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

int main(int argc, char** argv)
{
    int ret = 0;
    DNNDetector* dtr = create_detector();

    if(init_detector(dtr) != 0){
        printf("init detector failed!\n");
        return -1;
    }

    int pitch = 640, height = 360;
    int n, size = pitch * height * 3 / 2;
    char* nv12 = (char*)malloc(size);
    FILE* pf = fopen("/tmp/test.yuv", "rb");
    if(!pf) {
        printf("cannot open input yuv file!\n");
        return -1;
    }

    RegionInfo reg = {};
    for (int i=0; i<10; i++){
        n = fread(nv12, size, 1, pf);
        memset(&reg, 0, sizeof(reg));
        ret = detect_frame(dtr, nv12, pitch, height, &reg);
        if(ret != 0){
            printf("detect frame failed!\n");
            return -1;
        }
        printf("%s = %f @[%d, %d, %d, %d]\n", reg.name, reg.confidence, reg.x, reg.y, reg.w, reg.h);
    }

    destroy_detector();

    printf("test done!\n");
    free(nv12);
    fclose(pf);
    return 0;
}