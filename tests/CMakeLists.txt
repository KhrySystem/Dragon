cmake_minimum_required(VERSION 3.22)

project(DragonTests)

include(../UseDragon.cmake)

add_executable(dgWindowOpenTest dgWindowOpenTest.cpp)
target_link_libraries(dgWindowOpenTest Dragon)