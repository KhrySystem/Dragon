cmake_minimum_required(VERSION 3.20)

set(Dragon_INSTALL_DEST ${Dragon_VERSION}/${CMAKE_BUILD_TYPE})

install(TARGETS Dragon-Headers DESTINATION ${Dragon_VERSION}/${CMAKE_BUILD_TYPE}/include)
install(TARGETS ${Dragon_LIB_NAME} DESTINATION ${Dragon_VERSION}/${CMAKE_BUILD_TYPE}/bin)
