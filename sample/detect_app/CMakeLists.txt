
cmake_minimum_required(VERSION 2.8)

project( detect_app )

include_directories ("${PROJECT_SOURCE_DIR}/../../libdetect")
link_directories("${PROJECT_SOURCE_DIR}/../../build_libdetect")

# need set C++11 to use OpenCV
set (CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable( detect_app detect_app.cpp )

set_target_properties(detect_app PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries( detect_app libdetect opencv_core opencv_videoio)