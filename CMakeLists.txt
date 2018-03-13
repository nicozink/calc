cmake_minimum_required(VERSION 3.5)

project(calc_test)

enable_testing()

set(CMAKE_PREFIX_PATH "/libraries")
set(as_subproject unittest)

macro(find_package name)
	if(NOT "${name}" IN_LIST as_subproject)
		_find_package(${ARGV})
	endif()
endmacro()

add_subdirectory(libraries/unittest)

set(SRC_PARSER
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_lexer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_parser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_parser_test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/interpreter/interpreter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/util/variant_list.cpp)

add_library(calc_parser ${SRC_PARSER})

target_include_directories(calc_parser
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(calc_parser
	PRIVATE
	cxx_auto_type
	cxx_strong_enums)

set(SRC_MAIN
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(llvm_backend INTERFACE)

target_include_directories(llvm_backend
	INTERFACE "/usr/lib/llvm-3.8/include/")

link_directories("/usr/lib/llvm-3.8/lib/")

target_link_libraries(llvm_backend
	INTERFACE
	LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMSelectionDAG LLVMAsmPrinter LLVMX86Desc LLVMMCDisassembler LLVMX86Info LLVMX86AsmPrinter LLVMX86Utils LLVMMCJIT LLVMInterpreter LLVMExecutionEngine LLVMRuntimeDyld LLVMCodeGen LLVMTarget LLVMScalarOpts LLVMInstCombine LLVMInstrumentation LLVMProfileData LLVMObject LLVMMCParser LLVMTransformUtils LLVMMC LLVMBitWriter LLVMAnalysis LLVMRuntimeDyld LLVMObject LLVMMCParser LLVMMC LLVMIRReader LLVMBitReader LLVMAsmParser LLVMCore LLVMSupport rt dl tinfo pthread z m ffi)

add_executable(calc_test ${SRC_MAIN})

target_include_directories(calc_test
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(calc_test calc_parser llvm_backend)

target_compile_features(calc_test
	PRIVATE
	cxx_strong_enums)

set(SRC_TEST
	${CMAKE_CURRENT_SOURCE_DIR}/test/util/test_variant_list.cpp)

add_executable(calc_test_test ${SRC_TEST})

target_link_libraries(calc_test_test calc_parser llvm_backend unittest)

add_test(NAME calc_test_test
	COMMAND calc_test_test)