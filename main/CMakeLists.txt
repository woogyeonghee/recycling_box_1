cmake_minimum_required(VERSION 3.1.0)
project(recycle_Box)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(CppLinuxSerial/include)

add_subdirectory(CppLinuxSerial/src)

add_executable (excute main.cpp)

target_compile_features(excute PRIVATE cxx_std_17)
target_link_libraries(excute ${OpenCV_LIBS})
target_link_libraries(excute CppLinuxSerial)

