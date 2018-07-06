cmake_minimum_required(VERSION 2.8)

project( libdetect )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# need set C++11 to use OpenCV
set (CMAKE_CXX_STANDARD 11)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_library(libdetect SHARED detector.cpp detector.h)

target_link_libraries( libdetect ${OpenCV_LIBS} )
