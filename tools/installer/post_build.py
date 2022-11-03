from os import path
from sys import argv
from utils import *

DRAGON_SOURCE_DIR = argv[1]
DRAGON_BINARY_DIR = argv[2]
DRAGON_INSTALL_DIR = argv[3]
DRAGON_HEADERS_INSTALL_DIR = r"{}/include/".format(DRAGON_INSTALL_DIR)
DRAGON_SOURCE_INSTALL_DIR = DRAGON_INSTALL_DIR + "/src/"
DRAGON_LIBRARY_INSTALL_DIR = DRAGON_INSTALL_DIR + "/lib/"
DRAGON_BINARY_INSTALL_DIR = DRAGON_INSTALL_DIR + "/bin/"
cmake_path = argv[4]
arr = cmake_path.split("/")
c = ""
for i in range(0, len(arr) - 2):
	c += arr[i] + "/"
arr = argv[5].split(".")
CMAKE_MODULE_PATH = c + "share/cmake-" + arr[0] + "_" + arr[1] + "/Modules/"

def main():
	try:
		shutil.rmtree(DRAGON_INSTALL_DIR)
	except:
		pass
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/glfw/include/", DRAGON_HEADERS_INSTALL_DIR)
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/glm/", DRAGON_HEADERS_INSTALL_DIR)
	copy_headers_in_path(DRAGON_BINARY_DIR + "/external/openal/", DRAGON_HEADERS_INSTALL_DIR + "openal/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openxr/include/", DRAGON_HEADERS_INSTALL_DIR)
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openxr/external/include/", DRAGON_HEADERS_INSTALL_DIR)
	copy_headers_in_path(DRAGON_BINARY_DIR + "/external/openxr/include", DRAGON_HEADERS_INSTALL_DIR)
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/include/", DRAGON_HEADERS_INSTALL_DIR)

	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/glfw/src/", DRAGON_SOURCE_INSTALL_DIR + "glfw/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/glfw/src/", DRAGON_SOURCE_INSTALL_DIR + "glfw/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/glfw/deps/", DRAGON_SOURCE_INSTALL_DIR + "glfw/deps/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/glfw/deps/", DRAGON_SOURCE_INSTALL_DIR + "glfw/deps/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openal/al/", DRAGON_SOURCE_INSTALL_DIR + "openal/al/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/openal/al/", DRAGON_SOURCE_INSTALL_DIR + "openal/al/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openal/alc/", DRAGON_SOURCE_INSTALL_DIR + "openal/alc/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/openal/alc/", DRAGON_SOURCE_INSTALL_DIR + "openal/alc/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openal/common/", DRAGON_SOURCE_INSTALL_DIR + "openal/common/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/openal/common/", DRAGON_SOURCE_INSTALL_DIR + "openal/common/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openal/core/", DRAGON_SOURCE_INSTALL_DIR + "openal/core/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/openal/core/", DRAGON_SOURCE_INSTALL_DIR + "openal/core/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openal/router/", DRAGON_SOURCE_INSTALL_DIR + "openal/router/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/openal/router/", DRAGON_SOURCE_INSTALL_DIR + "openal/router/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openal/utils/", DRAGON_SOURCE_INSTALL_DIR + "openal/utils/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/openal/utils/", DRAGON_SOURCE_INSTALL_DIR + "openal/utils/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/external/openxr/src/", DRAGON_SOURCE_INSTALL_DIR + "openxr/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/external/openxr/src/", DRAGON_SOURCE_INSTALL_DIR + "openxr/")
	copy_headers_in_path(DRAGON_SOURCE_DIR + "/src/", DRAGON_SOURCE_INSTALL_DIR + "dragon/")
	copy_source_in_path(DRAGON_SOURCE_DIR + "/src/", DRAGON_SOURCE_INSTALL_DIR + "dragon/")
	copy_headers_in_path(DRAGON_BINARY_DIR + "/external/openxr/src/", DRAGON_SOURCE_INSTALL_DIR + "openxr/src")
	copy_source_in_path(DRAGON_BINARY_DIR + "/external/openxr/src/", DRAGON_SOURCE_INSTALL_DIR + "openxr/src")

	copy_libraries_in_path(DRAGON_BINARY_DIR + "/external/glfw/src/Release/", DRAGON_LIBRARY_INSTALL_DIR)
	copy_binaries_in_path(DRAGON_BINARY_DIR + "/external/glfw/examples/Release/", DRAGON_BINARY_INSTALL_DIR)
	copy_libraries_in_path(DRAGON_BINARY_DIR + "/external/glm/glm/Release/", DRAGON_LIBRARY_INSTALL_DIR)
	copy_libraries_in_path(DRAGON_BINARY_DIR + "/external/openal/Release/", DRAGON_LIBRARY_INSTALL_DIR)
	copy_binaries_in_path(DRAGON_BINARY_DIR + "/external/openal/Release/", DRAGON_BINARY_INSTALL_DIR)
	copy_libraries_in_path(DRAGON_BINARY_DIR + "/external/openal/Release/", DRAGON_LIBRARY_INSTALL_DIR)
	copy_binaries_in_path(DRAGON_BINARY_DIR + "/external/openal/Release/", DRAGON_BINARY_INSTALL_DIR)

	copy_libraries_in_path(DRAGON_BINARY_DIR + "/tools/tests/Release/", DRAGON_LIBRARY_INSTALL_DIR)
	copy_binaries_in_path(DRAGON_BINARY_DIR + "/tools/tests/Release/", DRAGON_BINARY_INSTALL_DIR)
	
	copy_libraries_in_path(DRAGON_BINARY_DIR + "/src/Release/", DRAGON_LIBRARY_INSTALL_DIR)
	copy_binaries_in_path(DRAGON_BINARY_DIR + "/src/Release/", DRAGON_BINARY_INSTALL_DIR)


if __name__ == "__main__":
	main()