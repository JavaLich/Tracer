# CMakeList.txt : CMake project for Tracer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add source to this project's executable.
file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable (${PROJECT_NAME} ${sources} ${headers})

find_package(SDL2 REQUIRED)
target_include_directories(Tracer PRIVATE ${SDL2_INCLUDE_DIRS})

find_package(OpenCL REQUIRED)

target_include_directories(Tracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(Tracer ${SDL2_LIBRARIES})
target_link_libraries(Tracer opengl32.lib)
target_link_libraries(Tracer OpenCL::OpenCL)

configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/SDL2.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/SDL2.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel/SDL2.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/SDL2.dll COPYONLY)
# TODO: Add tests and install targets if needed.
