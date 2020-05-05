# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (Tracer)

# Include sub-projects.
add_subdirectory ("src")

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)

