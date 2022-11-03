cmake_minimum_required(VERSION 3.20)

add_executable(audioTest audioTest.cpp)
target_include_directories(audioTest PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(audioTest Dragon::Dragon)