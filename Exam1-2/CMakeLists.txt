cmake_minimum_required(VERSION 2.8)
project( EX01 )
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Includes
include_directories(GLUT_INCLUDE_DIRS})
include_directories(OPENGL_INCLUDE_DIR})
include_directories(GLEW_INCLUDE_DIRS})
include_directories(include)

# Executables
add_executable(Ex02 Ex02.c )
add_executable(Ex03 Ex03.c )

# Libraries
add_library(EGL SHARED IMPORTED)
add_library(Utils Utils.c)

# Link dirs

# Link Libs
target_link_libraries(Ex02 ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} Utils m)
target_link_libraries(Ex03 ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} Utils m)

# After build
# add_custom_command()
