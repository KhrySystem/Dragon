macro(add_boost_export_rule t)
    set_target_properties(boost_${t} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")
    install(
        TARGETS boost_${t}
        EXPORT Boost${t}Export
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(EXPORT Boost${t}Export
        FILE Boost${t}Targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Boost/${t}
    )

    install(DIRECTORY ${Dragon_SOURCE_DIR}/external/boost/${t}/include/boost DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
endmacro()

FILE(GLOB children RELATIVE ${Dragon_SOURCE_DIR}/external/boost ${Dragon_SOURCE_DIR}/external/boost/*)
foreach(child ${children})
    if(IS_DIRECTORY ${Dragon_SOURCE_DIR}/external/boost/${child})
        add_boost_export_rule(${child})
    endif()
endforeach()