set(CPACK_PACKAGE_NAME ${Dragon_LIB_NAME})
set(CPACK_PACKAGE_VENDOR "KhrySystem")
set(CPACK_PACKAGE_VERSION_MAJOR ${Dragon_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Dragon_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Dragon_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "code-first C++ Vulkan 3D game engine")
#set(CPACK_PACKAGE_FILE_NAME )
set(CPACK_PACKAGE_ICON ${Dragon_SOURCE_DIR}/tools/res/logo_64.png)
set(CPACK_RESOURCE_FILE_LICENSE ${Dragon_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${Dragon_SOURCE_DIR}/README.md)
set(CPACK_OUTPUT_CONFIG_FILE DragonCPackConfig.cmake)
set(CPACK_THREADS 0)

file(GLOB Boost_CMAKE_INSTALL ${Dragon_SOURCE_DIR}/external/boost/*/cmake_install.cmake)
foreach(f ${Boost_CMAKE_INSTALL})
    include(${f})
endforeach()

include(GNUInstallDirs)

#set(${Dragon_CONFIG_FILE} ${Dragon_BINARY_DIR}/lib/cmake/dragon/DragonConfig.cmake)
#file(WRITE ${Dragon_CONFIG_FILE} "include(CMakeFindDependencyMacro)")
#file(APPEND ${Dragon_CONFIG_FILE} "find_dependency(GLFW3)")
#file(APPEND ${Dragon_CONFIG_FILE} "include(\${CMAKE_CURRENT_LIST_DIR}/DragonTargets.cmake")

include(${Dragon_SOURCE_DIR}/tools/cmake/boost_helper_install.cmake)

install(TARGETS ${Dragon_LIB_NAME} 
    EXPORT DragonExport
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT DragonExport
    FILE DragonTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dragon
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${Dragon_SOURCE_DIR}/tools/cmake/DragonConfig.cmake.in ${Dragon_BINARY_DIR}/DragonConfig.cmake 
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dragon 
)
write_basic_package_version_file(${Dragon_BINARY_DIR}/DragonConfigVersion.cmake VERSION ${Dragon_VERSION} COMPATIBILITY SameMajorVersion)

install(DIRECTORY ${Dragon_SOURCE_DIR}/include/dragon DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(DIRECTORY ${Dragon_SOURCE_DIR}/external/glm/glm DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
install(DIRECTORY ${Dragon_BINARY_DIR}/src/shaders DESTINATION ${CMAKE_INSTALL_LIBDIR} FILES_MATCHING PATTERN "*.spv")
install(DIRECTORY ${Dragon_SOURCE_DIR}/src DESTINATION ${CMAKE_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.c" PATTERN "*.cpp" PATTERN "*.glsl")
install(TARGETS dginfo DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${Dragon_BINARY_DIR}/DragonConfig.cmake
              ${Dragon_BINARY_DIR}/DragonConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dragon)
include(CPack)


