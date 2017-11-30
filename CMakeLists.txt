cmake_minimum_required(VERSION 3.5)

project(calc_test)

#add_custom_target(calc_parser_custom)

find_package(FLEX)

FLEX_TARGET(calc_flex
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_lexer.l
	${CMAKE_CURRENT_BINARY_DIR}/calc_lexer.cpp)

find_package(BISON)

BISON_TARGET(calc_bison
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_parser.y
	${CMAKE_CURRENT_BINARY_DIR}/calc_parser.cpp)

ADD_FLEX_BISON_DEPENDENCY(calc_flex calc_bison)

set(SRC_PARSER
	${FLEX_calc_flex_OUTPUTS}
	${BISON_calc_bison_OUTPUTS}
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/interpreter.cpp)

add_library(calc_parser ${SRC_PARSER})

target_include_directories(calc_parser
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser)

target_compile_features(calc_parser
	PRIVATE
	cxx_auto_type)

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
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(calc_test calc_parser llvm_backend)
