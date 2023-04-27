cmake_minimum_required(VERSION 3.6)
project(OpenGLTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lmingw32")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES main.cpp)
add_executable(OpenGLTest ${SOURCE_FILES})

target_link_libraries(OpenGLTest mingw32 SDL2main SDL2)
