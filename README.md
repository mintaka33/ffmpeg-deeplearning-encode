# VideoCPET
A E2E solution for Video Capturing, Processing, Encoding and Transmitting (CPET) used in intelligent surveillance video

# FFmpeg 

## ffmpeg basic usage
```bash
# USB camera preview
./ffplay -f v4l2 -s 640x480 -i /dev/video0 

# capture video and encode (SW)
./ffmpeg -f v4l2 -s 640x480 -i /dev/video0 -vframes 100 -c:v h264 out.mp4

# capture video and encode (HW)
./ffmpeg -vaapi_device /dev/dri/renderD128 -hwaccel vaapi -f v4l2 -i /dev/video0 \
-vf 'format=nv12,hwupload' -vframes 100 -c:v h264_vaapi out.mp4 -y

# capture video, scaling+encode (HW)
./ffmpeg -vaapi_device /dev/dri/renderD128 -hwaccel vaapi -f v4l2 -i /dev/video0 \
-vf 'format=nv12,hwupload,scale_vaapi=640:480:nv12' -vframes 100 -c:v h264_vaapi out.mp4 -y

```

## ffmpeg server streaming usage
```bash

# server (note: 10.239.141.208 is client IP)
./ffmpeg -vaapi_device /dev/dri/renderD128 -hwaccel vaapi -f v4l2 -i /dev/video0 \
-vf 'format=nv12,hwupload,scale_vaapi=640:480:nv12' -vframes 10000 -c:v h264_vaapi \
-f mpegts udp://10.239.141.208:1234 -re

# client (note: 10.239.141.20 is server IP)
ffplay udp://10.239.141.20:1234

```
