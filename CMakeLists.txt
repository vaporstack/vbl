cmake_minimum_required(VERSION 3.0)

macro(use_c99)
	if (CMAKE_VERSION VERSION_LESS "3.1")
		if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
			set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
		endif ()
	else ()
		set (CMAKE_C_STANDARD 99)
	endif ()
endmacro(use_c99)


set(project_name vbl)

project(${project_name})

set (${project_name}_VERSION_MAJOR 0)
set (${project_name}_VERSION_MINOR 0)
set (${project_name}_VERSION_PATCH 1)

option(BuildVBLLibrary "Build ${project_name} static library" ON)
option(BuildVBLDocumentation "BuildDocumentation" OFF)
option(BuildVBLDemo "BuildDemo" OFF)
option(BuildVBLTest "BuildTest" OFF)


if(${BuildVBLTest})
  add_subdirectory("test")
endif()


if(${BuildVBLDemo})
  add_subdirectory("demo")
endif()

file(GLOB_RECURSE sources vbl.c src/*.c)

add_library(vbl STATIC ${sources})

IF(WIN32)
  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/platform/win)
ELSE()
  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/platform/darwin
  	/usr/local/lib)
ENDIF()

target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/include)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/deps)

target_link_libraries(vbl wsh m r4)

