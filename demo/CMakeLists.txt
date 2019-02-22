cmake_minimum_required(VERSION 3.0)

message("configuring demo.")

project(vbl_demo)

set(vbl_demo_VERSION_MAJOR 0)
set(vbl_demo_VERSION_MINOR 0)
set(vbl_demo_VERSION_PATCH 1)

macro(use_c99)
  if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif()
  else()
    set(CMAKE_C_STANDARD 99)
  endif()
endmacro(use_c99)

use_c99()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules/")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(OperatingSystem "macos")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET(OperatingSystem "linux")
ENDIF()

IF(WIN32)
  SET(OperatingSystem "windows")
ENDIF()

IF(OperatingSystem STREQUAL "linux") # SET(OperatingSystem "linux")
  set(cpp_library "stdc++")

  # Store in CMAKE_DEB_HOST_ARCH var the current build architecture
  execute_process(COMMAND dpkg-architecture -qDEB_HOST_ARCH
                  OUTPUT_VARIABLE CMAKE_DEB_HOST_ARCH
                                  OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(${CMAKE_DEB_HOST_ARCH} MATCHES "armhf")
    set(link_directory /usr/lib/arm-linux-gnueabihf
        ${CMAKE_SOURCE_DIR}/contrib/platform/linux2/armhf) # message("arm HARD
                                                           # FLOAT WOOO")
  elseif(${CMAKE_DEB_HOST_ARCH} MATCHES "i386")
    set(link_directory /usr/lib/
        ${CMAKE_SOURCE_DIR}/contrib/platform/linux2/x86) # message("eighty sixed
                                                     # WOOOOO")
  else()
    set(link_directory /usr/lib/x86_64-linux-gnu/
        ${CMAKE_SOURCE_DIR}/contrib/platform/linux2/x86_64)

  endif()

  set(platform_libs X11 Xrandr Xi Xinerama z GLEW Xcursor)
ENDIF(OperatingSystem STREQUAL "linux")

link_directories(${link_directory} ${CMAKE_BINARY_DIR}
                 ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/libs/wsh
                 ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/libs/r4
                 ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib/platform/darwin

                 )

if(${OperatingSystem} MATCHES "macos")
  find_library(FREETYPE_LIB freetype $ENV{HOME}/art/r4/contrib/platform/darwin)
  FIND_LIBRARY(PA_LIB portaudio $ENV{HOME}/art/r4/contrib/platform/darwin)
  FIND_LIBRARY(COCOA_LIB Cocoa)
  FIND_LIBRARY(IL_Library IL)
  FIND_LIBRARY(PM_LIBRARY portaudio)
  FIND_LIBRARY(AU_LIB AudioUnit)
  FIND_LIBRARY(AT_LIB AudioToolbox)
  FIND_LIBRARY(CA_LIB CoreAudio)
  FIND_LIBRARY(CARBON_LIB Carbon)
  FIND_LIBRARY(CM_LIB CoreMidi)
  FIND_LIBRARY(CV_LIB CoreVideo)
  FIND_LIBRARY(IO_LIB IOKit)
  FIND_LIBRARY(GL_LIB OpenGL)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}
                   /usr/local/lib)

endif()


find_package(OpenGL)
if(OPENGL_FOUND)
  MESSAGE("OpenGL Correctly Found")
  include_directories(${OPENGL_INCLUDE_DIR})
else(OPENGL_FOUND)
  MESSAGE("OpenGL environment missing")
endif(OPENGL_FOUND)

file(GLOB sources
          src/*.c
         # src/demos/*.c
         # src/support/*.c
         # src/input/*.c
         # src/contrib/macos/*.m
         # ${CMAKE_CURRENT_SOURCE_DIR}/../cjson/cJSON.c
         )

# message(${sources})

add_executable(vbl_demo ${sources})

target_include_directories(vbl_demo PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs
                           ${CMAKE_CURRENT_SOURCE_DIR}/contrib
                           ${CMAKE_CURRENT_SOURCE_DIR}/../contrib
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../../contrib
                           ${CMAKE_CURRENT_SOURCE_DIR}/contrib/ftgl/src)

if(${OperatingSystem} MATCHES "macos")
  target_link_libraries(vbl_demo
                        LINK_PUBLIC
                        ${ADDITIONAL_LIBS}
                        glfw
                        vbl
                        ftgl
                        ${GL_LIB}
                        ${COCOA_LIB}
                        ${CARBON_LIB}
                        ${IO_LIB}
                        ${CV_LIB})

endif(${OperatingSystem} MATCHES "macos")

if(${OperatingSystem} MATCHES "linux")
  target_link_libraries(vbl_demo LINK_PUBLIC vbl glfw3 GL m X11 Xi)
endif()
