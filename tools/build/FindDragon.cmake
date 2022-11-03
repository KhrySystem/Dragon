find_package(Vulkan_REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenCL)

if(WIN32)
	if($ENV{DRAGON_SDK})
		list(APPEND _Dragon_hint_header_path $ENV{DG_SDK_PATH}/include)
		list(APPEND _Dragon_hint_binary_path $ENV{DG_SDK_PATH}/lib)
		list(APPEND _Dragon_hint_binary_path $ENV{DG_SDK_PATH}/lib64)
		list(APPEND _Dragon_hint_data_path $ENV{DG_SDK_PATH}/data)
	endif()
else()
	if($ENV{DRAGON_SDK})
		list(APPEND _Dragon_hint_header_path $ENV{DG_SDK_PATH}/include)
		list(APPEND _Dragon_hint_binary_path $ENV{DG_SDK_PATH}/lib)
		list(APPEND _Dragon_hint_binary_path $ENV{DG_SDK_PATH}/lib64)
		list(APPEND _Dragon_hint_data_path $ENV{DG_SDK_PATH}/share)
	endif()
endif()

if($ENV{DRAGON_SDK_VERSION})
	set(Dragon_VERSION $ENV{DRAGON_VERSION})
endif()

find_library(Dragon_LIBRARY
	NAMES dragon-0_7 dragon-1_0 dragon-1_1 dragon-1_2
	HINTS
		${_Dragon_hint_binary_path}
)

find_path(Dragon_INCLUDE_DIRS

)
