#include <stdio.h>
#include "interface.h"

int main(int argc, char** argv)
{
    int ret = 0;
    DNNDetector* dtr = create_detector();

    ret = init_detector(dtr);

    return 0;
}