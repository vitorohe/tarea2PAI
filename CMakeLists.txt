cmake_minimum_required(VERSION 2.8)
project( tarea-2 )
find_package( OpenCV REQUIRED )
include_directories("${CMAKE_SOURCE_DIR}")
add_library(funciones funciones.cpp funciones.h)
add_library(mst mst.cpp mst.h)
add_library(graph graph.cpp graph.h)
add_library(segmentation segmentation.cpp segmentation.h)
add_executable( main main.cpp )
target_link_libraries( main graph segmentation mst funciones ${OpenCV_LIBS} )