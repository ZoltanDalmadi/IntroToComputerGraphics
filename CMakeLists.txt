cmake_minimum_required(VERSION 2.8)
project(bevgraf)

set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL -s)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/Utils
)

add_subdirectory(Homework_01)
add_subdirectory(Homework_02)
add_subdirectory(Homework_03)
add_subdirectory(Homework_04)
add_subdirectory(Homework_05)
add_subdirectory(Homework_06)
add_subdirectory(Homework_07)
add_subdirectory(Homework_08)
add_subdirectory(Homework_09)
add_subdirectory(Homework_10)
