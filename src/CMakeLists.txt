include(CMakeParseArguments)

# Function to wrap a given string into multiple lines at the given column position.
# Parameters:
#   VARIABLE    - The name of the CMake variable holding the string.
#   AT_COLUMN   - The column position at which string will be wrapped.
function(WRAP_STRING)
    set(oneValueArgs VARIABLE AT_COLUMN)
    cmake_parse_arguments(WRAP_STRING "${options}" "${oneValueArgs}" "" ${ARGN})

    string(LENGTH ${${WRAP_STRING_VARIABLE}} stringLength)
    math(EXPR offset "0")

    while(stringLength GREATER 0)

        if(stringLength GREATER ${WRAP_STRING_AT_COLUMN})
            math(EXPR length "${WRAP_STRING_AT_COLUMN}")
        else()
            math(EXPR length "${stringLength}")
        endif()

        string(SUBSTRING ${${WRAP_STRING_VARIABLE}} ${offset} ${length} line)
        set(lines "${lines}\n${line}")

        math(EXPR stringLength "${stringLength} - ${length}")
        math(EXPR offset "${offset} + ${length}")
    endwhile()

    set(${WRAP_STRING_VARIABLE} "${lines}" PARENT_SCOPE)
endfunction()

# Function to embed contents of a file as byte array in C/C++ header file(.h). The header file
# will contain a byte array and integer variable holding the size of the array.
# Parameters
#   SOURCE_FILE     - The path of source file whose contents will be embedded in the header file.
#   VARIABLE_NAME   - The name of the variable for the byte array. The string "_SIZE" will be append
#                     to this name and will be used a variable name for size variable.
#   HEADER_FILE     - The path of header file.
#   APPEND          - If specified appends to the header file instead of overwriting it
#   NULL_TERMINATE  - If specified a null byte(zero) will be append to the byte array. This will be
#                     useful if the source file is a text file and we want to use the file contents
#                     as string. But the size variable holds size of the byte array without this
#                     null byte.
# Usage:
#   bin2h(SOURCE_FILE "Logo.png" HEADER_FILE "Logo.h" VARIABLE_NAME "LOGO_PNG")
function(BIN2H)
    set(options APPEND NULL_TERMINATE)
    set(oneValueArgs SOURCE_FILE VARIABLE_NAME HEADER_FILE)
    cmake_parse_arguments(BIN2H "${options}" "${oneValueArgs}" "" ${ARGN})

    # reads source file contents as hex string
    file(READ ${BIN2H_SOURCE_FILE} hexString HEX)
    string(LENGTH ${hexString} hexStringLength)

    # appends null byte if asked
    if(BIN2H_NULL_TERMINATE)
        set(hexString "${hexString}00")
    endif()

    # wraps the hex string into multiple lines at column 32(i.e. 16 bytes per line)
    wrap_string(VARIABLE hexString AT_COLUMN 32)
    math(EXPR arraySize "${hexStringLength} / 2")

    # adds '0x' prefix and comma suffix before and after every byte respectively
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " arrayValues ${hexString})
    # removes trailing comma
    string(REGEX REPLACE ", $" "" arrayValues ${arrayValues})

    # converts the variable name into proper C identifier
    string(MAKE_C_IDENTIFIER "${BIN2H_VARIABLE_NAME}" BIN2H_VARIABLE_NAME)
    string(TOUPPER "${BIN2H_VARIABLE_NAME}" BIN2H_VARIABLE_NAME)

    # declares byte array and the length variables
    set(arrayDefinition "const unsigned char ${BIN2H_VARIABLE_NAME}[] = { ${arrayValues} };")
    set(arraySizeDefinition "const size_t ${BIN2H_VARIABLE_NAME}_SIZE = ${arraySize};")

    set(declarations "${arrayDefinition}\n\n${arraySizeDefinition}\n\n")
    if(BIN2H_APPEND)
        file(APPEND ${BIN2H_HEADER_FILE} "${declarations}")
    else()
        file(WRITE ${BIN2H_HEADER_FILE} "${declarations}")
    endif()
endfunction()
    

# Check if the library is to be built as static or shared
if(${Dragon_BUILD_STATIC})
	# Create a static library with the given name, and specify the sources to be compiled
	add_library(${Dragon_LIB_NAME} STATIC
		"engine.cpp"
		"window.cpp"
		"gpu.cpp"
		"validation.cpp"
        "vertex.cpp"
	)
else() 
	# Create a shared library with the given name, and specify the sources to be compiled
	add_library(${Dragon_LIB_NAME} SHARED 
		"engine.cpp"
		"window.cpp"
		"gpu.cpp"
		"validation.cpp"
	    "vertex.cpp"
    )
	# Define the preprocessor symbol for shared library build
	target_compile_definitions(${Dragon_LIB_NAME} PUBLIC -DDRAGON_BUILD_DLL)
endif()


execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${Dragon_BINARY_DIR}/shaders)
if(${Vulkan_glslc_FOUND})
    message(STATUS "Using glslc to build SPIR-V Binaries")
    execute_process(
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_SOURCE_DIR}/src/shaders/shader.frag -o ${Dragon_BINARY_DIR}/shaders/frag.spv --target-env=vulkan1.3
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_SOURCE_DIR}/src/shaders/shader.vert -o ${Dragon_BINARY_DIR}/shaders/vert.spv --target-env=vulkan1.3
    )

    add_custom_target(Shaders SOURCES ${Dragon_BINARY_DIR}/shaders/vert.spv ${Dragon_BINARY_DIR}/shaders/frag.spv DEPENDS ${Dragon_SOURCE_DIR}/src/shaders/shader.frag ${Dragon_SOURCE_DIR}/src/shaders/shader.vert "vertex.cpp")

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/shaders.hpp")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/shaders.hpp" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/shaders.hpp" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)
elseif(${Vulkan_glslangValidator_FOUND})
    message(STATUS "Using glslangValidator to build SPIR-V Binaries")

    execute_process(
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${Dragon_SOURCE_DIR}/src/shaders/shader.frag -o ${Dragon_BINARY_DIR}/shaders/frag.spv --target-env vulkan1.3
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${Dragon_SOURCE_DIR}/src/shaders/shader.vert -o ${Dragon_BINARY_DIR}/shaders/vert.spv --target-env vulkan1.3
    )

    add_custom_target(Shaders SOURCES ${Dragon_BINARY_DIR}/shaders/vert.spv ${Dragon_BINARY_DIR}/shaders/frag.spv DEPENDS ${Dragon_SOURCE_DIR}/src/shaders/shader.frag ${Dragon_SOURCE_DIR}/src/shaders/shader.vert)

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/shaders.hpp")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/shaders.hpp" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/shaders.hpp" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)

else()
    message(WARNING "Using prebuilt SPIR-V Binaries")
endif()

add_dependencies(${Dragon_LIB_NAME} Shaders)

# Specify the include directories for the library
target_include_directories(${Dragon_LIB_NAME} PUBLIC ${Dragon_INCLUDE_DIRS})

# Link the library with the vulkan library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC Vulkan::Vulkan)

# Link the library with the glfw library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC glfw)

# Link the library with Boost::Thread
target_link_libraries(${Dragon_LIB_NAME} PUBLIC Boost::thread)

# Create an alias for the library
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})