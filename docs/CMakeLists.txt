cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE Dragon_Docs_Source ${Dragon_SOURCE_DIR}/docs/res)
file(GLOB_RECURSE Dragon_Main_Docs_SOURCE ${Dragon_SOURCE_DIR}/headers/include)

set(Dragon_DOXYGEN_INPUT 
	${Dragon_Docs_SOURCE} 
	${Dragon_Main_Docs_SOURCE}
)

configure_file(Doxyfile.in Doxyfile @ONLY)

add_custom_target(Dragon_Docs ALL "${DOXYGEN_EXECUTABLE}"
	WORKING_DIRECTORY "${Dragon_BINARY_DIR}/docs"
	COMMENT "Generating Dragon HTML documentation" VERBATIM
)