cmake_minimum_required(VERSION 2.8)

project(nbody)
add_executable(nbody main.cpp Particle.h ocTree.h posVec.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(nbody ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_SOURCE_DIR}/CMake-bin;CMake;ALL;/")
include (CPack)
