cmake_minimum_required(VERSION 2.8)

project( test )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories ("${PROJECT_SOURCE_DIR}/../detect")
link_directories("${PROJECT_SOURCE_DIR}/../build/detect")

# need use C++11 to invoke OpenCV
set (CMAKE_CXX_STANDARD 11)

add_executable( edge edge.cpp )
target_link_libraries( edge ${OpenCV_LIBS} )

add_executable( test1 test1.cpp )
target_link_libraries( test1 detect ${OpenCV_LIBS} )

add_executable( test2 test2.cpp )
target_link_libraries( test2 detect ${OpenCV_LIBS} )