cmake_minimum_required(VERSION 2.8)

project( test )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories ("${PROJECT_SOURCE_DIR}/../libdetect")
link_directories("${PROJECT_SOURCE_DIR}/../build/libdetect")

# need use C++11 to invoke OpenCV
set (CMAKE_CXX_STANDARD 11)

add_executable( edge edge.cpp )
target_link_libraries( edge ${OpenCV_LIBS} )

add_executable( detect detect.cpp )
target_link_libraries( detect ${OpenCV_LIBS} )

add_executable( detect2 detect2.cpp )
target_link_libraries( detect2 libdetect ${OpenCV_LIBS} )