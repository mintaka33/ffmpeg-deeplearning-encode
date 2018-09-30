#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DNNDetector DNNDetector;

DNNDetector* create_detector();
int init_detector(DNNDetector* d);
int detect_frame(DNNDetector* d);
void destroy_detector();

#ifdef __cplusplus
}
#endif