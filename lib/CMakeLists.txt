set(SKIP_PERFORMANCE_COMPARISON ON)

set(BOOST_ENABLE_PYTHON ON)

add_subdirectory(Boost)

add_subdirectory(OpenAL-Soft)

add_subdirectory(glfw)
set(GLFW_INCLUDE_DIRECTORIES 
	${CMAKE_CURRENT_SOURCE_DIR}/glfw 
	${CMAKE_CURRENT_SOURCE_DIR}/glfw/src 
	${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/glfw/deps
)

add_subdirectory(glm)
set(GLM_INCLUDE_DIRECTORIES
	${CMAKE_CURRENT_SOURCE_DIR}/glm
)