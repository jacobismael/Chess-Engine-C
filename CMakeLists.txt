cmake_minimum_required (VERSION 3.5)
SET (CMAKE_BUILD_TYPE Debug)

project (main)
# The project version number.
SET (${PROJECT_NAME}_VERSION_MAJOR 1)
SET (${PROJECT_NAME}_VERSION_MINOR 0)

set (CMAKE_C_STANDARD 11)
SET (CMAKE_C_COMPILER             "/usr/bin/gcc")
SET (CMAKE_C_FLAGS                "-W -Wall -std=c11")
SET (CMAKE_C_FLAGS_DEBUG          "-g")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (CMAKE_AR      "/usr/bin/ar")
SET (CMAKE_LINKER  "/usr/bin/ld")
SET (CMAKE_NM      "/usr/bin/nm")
SET (CMAKE_OBJDUMP "/usr/bin/objdump")
SET (CMAKE_RANLIB  "/usr/bin/ranlib")


include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )