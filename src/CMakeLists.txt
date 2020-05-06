# CMakeList.txt : CMake project for Tracer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenCL REQUIRED)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "main.cpp")

target_link_libraries(Tracer ${SDL2_LIBRARIES})
target_link_libraries(Tracer opengl32.lib)
target_link_libraries(Tracer OpenCL::OpenCL)

configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_SOURCE_DIR}/build/src/Debug/SDL2.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_SOURCE_DIR}/build/src/Release/SDL2.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_SOURCE_DIR}/build/src/MinSizeRel/SDL2.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_SOURCE_DIR}/build/src/RelWithDebInfo/SDL2.dll COPYONLY)
# TODO: Add tests and install targets if needed.
