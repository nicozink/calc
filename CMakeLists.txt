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
	${BISON_calc_bison_OUTPUTS})

add_library(calc_parser ${SRC_PARSER})

set(SRC_MAIN
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(calc_test ${SRC_MAIN})

target_include_directories(calc_test
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(calc_test calc_parser)

