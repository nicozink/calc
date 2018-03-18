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

#
# cpp_util
#

set(SRC_CPP_UTIL
	${CMAKE_CURRENT_SOURCE_DIR}/src/cpp_util/types/variant_list.cpp)

add_library(cpp_util ${SRC_CPP_UTIL})

target_include_directories(cpp_util
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(cpp_util
	PRIVATE
	cxx_auto_type)

#
# parser
#

set(SRC_PARSER
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/lexer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parse_result.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.cpp)

add_library(parser ${SRC_PARSER})

target_include_directories(parser
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(parser
	PRIVATE
	cxx_auto_type
	cxx_strong_enums)

target_link_libraries(parser cpp_util)

#
# main
#

set(SRC_MAIN
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_parser_test.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_lexer.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/calc_parser.cpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/src/interpreter/interpreter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(calc_parser ${SRC_MAIN})

target_include_directories(calc_parser
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(calc_parser parser cpp_util)

target_compile_features(calc_parser
	PRIVATE
	cxx_strong_enums)

set(SRC_TEST
	${CMAKE_CURRENT_SOURCE_DIR}/test/parser/test_lexer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/parser/test_parser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/util/test_regex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/util/test_variant_list.cpp)

add_executable(calc_test ${SRC_TEST})

target_link_libraries(calc_test parser cpp_util unittest)

add_test(NAME calc_test
	COMMAND calc_test)
