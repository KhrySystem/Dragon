cmake_minimum_required(VERSION 3.20)

add_executable(openWindow openWindow.cpp)
target_include_directories(openWindow PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(openWindow Dragon::Dragon)

add_executable(audioTest audioTest.cpp)
target_include_directories(audioTest PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(audioTest Dragon::Dragon)