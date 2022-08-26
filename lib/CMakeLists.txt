set(DRAGON_BUILD_OPENAL_SOFT CACHE BOOL ON)

if(DRAGON_BUILD_OPENAL_SOFT)
	add_subdirectory(OpenAL-Soft)
endif()

add_subdirectory(glfw)

add_subdirectory(glm)
set(GLM_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/glm
)