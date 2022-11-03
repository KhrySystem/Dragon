from sys import argv
from utils import *

Dragon_SOURCE_DIR = argv[1]
Dragon_BINARY_DIR = argv[2]
Dragon_INSTALL_DIR = argv[3]
Dragon_HEADERS_INSTALL_DIR = Dragon_INSTALL_DIR + "/include/"
Dragon_SOURCE_INSTALL_DIR = Dragon_INSTALL_DIR + "/src/"

def main():
	shutil.rmtree(Dragon_INSTALL_DIR)
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/glfw/include/", Dragon_HEADERS_INSTALL_DIR)
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/glm/", Dragon_HEADERS_INSTALL_DIR)
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openal/include/", Dragon_HEADERS_INSTALL_DIR)
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openxr/include/", Dragon_HEADERS_INSTALL_DIR)
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openxr/external/include/", Dragon_HEADERS_INSTALL_DIR)
	copy_headers_in_path(Dragon_SOURCE_DIR + "/headers/include/", Dragon_HEADERS_INSTALL_DIR)

	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/glfw/src/", Dragon_SOURCE_INSTALL_DIR + "glfw/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/glfw/src/", Dragon_SOURCE_INSTALL_DIR + "glfw/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/glfw/deps/", Dragon_SOURCE_INSTALL_DIR + "glfw/deps/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/glfw/deps/", Dragon_SOURCE_INSTALL_DIR + "glfw/deps/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openal/al/", Dragon_SOURCE_INSTALL_DIR + "openal/al/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/openal/al/", Dragon_SOURCE_INSTALL_DIR + "openal/al/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openal/alc/", Dragon_SOURCE_INSTALL_DIR + "openal/alc/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/openal/alc/", Dragon_SOURCE_INSTALL_DIR + "openal/alc/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openal/common/", Dragon_SOURCE_INSTALL_DIR + "openal/common/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/openal/common/", Dragon_SOURCE_INSTALL_DIR + "openal/common/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openal/core/", Dragon_SOURCE_INSTALL_DIR + "openal/core/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/openal/core/", Dragon_SOURCE_INSTALL_DIR + "openal/core/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openal/router/", Dragon_SOURCE_INSTALL_DIR + "openal/router/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/openal/router/", Dragon_SOURCE_INSTALL_DIR + "openal/router/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openal/utils/", Dragon_SOURCE_INSTALL_DIR + "openal/utils/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/openal/utils/", Dragon_SOURCE_INSTALL_DIR + "openal/utils/")
	copy_headers_in_path(Dragon_SOURCE_DIR + "/external/openxr/src/", Dragon_SOURCE_INSTALL_DIR + "openxr/src/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/external/openxr/src/", Dragon_SOURCE_INSTALL_DIR + "openxr/src/")
	copy_source_in_path(Dragon_SOURCE_DIR + "/src/dg/", Dragon_SOURCE_INSTALL_DIR + "dragon/")

if __name__ == "__main__":
	main()