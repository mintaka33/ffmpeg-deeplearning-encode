# VideoCPET
A E2E solution for Video Capturing, Processing, Encoding and Transmitting (CPET) used in intelligent surveillance video

# Build
```bash
# build opencv
mkdir build_opencv
cd build_opencv

# install dependencies
sudo apt install build-essential cmake pkg-config
sudo apt install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev
sudo apt install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt install libxvidcore-dev libx264-dev
sudo apt install libgtk2.0-dev libgtk-3-dev
sudo apt install libatlas-base-dev gfortran
sudo apt install python2.7-dev python3-dev

# if want to use Intel IPP library for better performance, set -DWITH_IPP=ON, 
# but need take some time to download IPP
cmake -DWITH_IPP=OFF -DWITH_HDF5=OFF ../opencv 

make -j8
sudo make install
sudo ldconfig

# build ffmpeg
mkdir build_ffmpeg
cd build_ffmpeg

$ sudo apt-get update -qq && sudo apt-get -y install \
autoconf \
automake \
build-essential \
cmake \
git \
libass-dev \
libfreetype6-dev \
libsdl2-dev \
libtool \
libvorbis-dev \
libxcb1-dev \
libxcb-shm0-dev \
libxcb-xfixes0-dev \
pkg-config \
texinfo \
wget \
zlib1g-dev \
nasm

sudo apt install libx264-dev libx265-dev

../FFmpeg/configure --enable-debug=3 --disable-optimizations \
--enable-libx264 --enable-libx265 --enable-gpl

make -j8
sudo make install
```

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
