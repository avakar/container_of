cmake_minimum_required(VERSION 3.16)

project(avakar.container_of)

if ("${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.19")
set(sources
	include/avakar/container_of.h
	src/bitcast.h
	)
endif()
add_library(avakar.container_of INTERFACE ${sources})
target_compile_features(avakar.container_of INTERFACE cxx_std_11)
target_include_directories(avakar.container_of INTERFACE include)

add_library(avakar::container_of ALIAS avakar.container_of)

if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
include(CTest)
if (BUILD_TESTING)

add_executable(avakar.container_of.test
	test/test.cpp
	)
target_link_libraries(avakar.container_of.test PUBLIC avakar::container_of)

add_test(NAME avakar.container_of.test COMMAND avakar.container_of.test)

endif()
endif()
