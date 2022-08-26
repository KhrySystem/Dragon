cmake_minimum_required(VERSION 3.10)

include(dgGraphicsModule/UseGraphics.cmake)

add_library(Dragon-Source SHARED dgEngine.cpp initialization.cpp
    dgGraphicsModule/window.cpp
)