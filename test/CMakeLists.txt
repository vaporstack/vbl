cmake_minimum_required(VERSION 3.0)
project(test_vbl)
set (test_vbl_VERSION_MAJOR 0)
set (test_vbl_VERSION_MINOR 0)
set (test_vbl_VERSION_PATCH 1)

file(GLOB sources *.c)
message(${sources})

link_directories(../../../build/libs/vbl
	../../../build/libs/wsh
	../../../build/libs/r4
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib/platform/darwin
)

add_executable(test_vbl ${sources})
target_include_directories (test_vbl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs)
target_include_directories (test_vbl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib/include)
target_include_directories (test_vbl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib)
target_link_libraries(test_vbl LINK_PUBLIC vbl glfw)



