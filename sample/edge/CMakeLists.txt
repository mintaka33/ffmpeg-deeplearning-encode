cmake_minimum_required(VERSION 2.8)

project( edge )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
set (CMAKE_CXX_STANDARD 11)

add_executable( edge edge.cpp )

target_link_libraries( edge ${OpenCV_LIBS} )