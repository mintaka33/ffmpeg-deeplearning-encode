cmake_minimum_required(VERSION 2.8)

project( libdetect )

find_package( OpenCV 4.0.0 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# need set C++11 to use OpenCV
set (CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_library(libdetect SHARED detector.cpp detector.h)

MESSAGE( STATUS "variable is: " ${OpenCV_LIBS} )

set_target_properties(libdetect PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( libdetect ${OpenCV_LIBS} )
