# Build

## build opencv

```bash
mkdir build && cd build
mkdir opencv && cd opencv

sudo apt update
sudo apt upgrade
# opencv will link with shared ffmepg library, install offical ffmpeg
sudo apt install ffmpeg

# install dependencies
# ubuntu 16.04
sudo apt install build-essential cmake pkg-config \
libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev \
libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
libxvidcore-dev libx264-dev \
libgtk2.0-dev libgtk-3-dev \
libatlas-base-dev gfortran \
python2.7-dev python3-dev
# ubuntu 18.04
sudo apt install build-essential cmake unzip pkg-config \
libjpeg-dev libpng-dev libtiff-dev \
libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
libxvidcore-dev libx264-dev \
libgtk-3-dev \
libatlas-base-dev gfortran \
python3-dev

# if want to use Intel IPP library for better performance, set -DWITH_IPP=ON, 
# but need take some time to download IPP
cmake -DWITH_IPP=OFF -DWITH_HDF5=OFF ../../opencv

make -j$(nproc)
sudo make install
sudo ldconfig
```

## build ffmpeg

```bash
mkdir build && cd build
mkdir ffmpeg && cd ffmpeg

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

../../FFmpeg/configure --enable-debug=3 --disable-optimizations --enable-libx264 --enable-libx265 --enable-gpl \
--extra-ldflags=-L/home/fresh/data/work/VideoCPET/build/detect --etrac-libs=-ldetect

make -j8
sudo make install
```
## build driver

```bash
# build libva
cd build && mkdir libva && cd libva
../../libva/autogen.sh CFLAGS=-g CXXFLAGS=-g
make
sudo make install

# build gmmlib
cd build && mkdir gmm && cd gmm
cmake ../../gmmlib
make -j$(nproc)
sudo make install

# build media-driver
cd build && mkdir driver && cd driver
cmake -DCMAKE_BUILD_TYPE=Debug ../../media-driver
make -j$(nproc)
sudo make install

```

## build object detection library

```bash
cd build && mkdir detect
cd detect
cmake ../../detect
make
```

## build test

```bash
cd build && mkdir test
cd test
cmake ../../test
make

```