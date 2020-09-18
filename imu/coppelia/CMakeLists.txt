cmake_minimum_required(VERSION 3.1.0)

#set project name
project(simpleTest C CXX)

find_package(Threads REQUIRED)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# vrep
include_directories(simpleTest remoteApi)
include_directories(simpleTest include)

add_definitions (-DNON_MATLAB_PARSING)
add_definitions (-DMAX_EXT_API_CONNECTIONS=255)
add_definitions (-DDO_NOT_USE_SHARED_MEMORY)

#bild
add_executable (simpleTest simpleTest.cpp remoteApi/extApi.c remoteApi/extApiPlatform.c)

target_link_libraries(simpleTest Threads::Threads)