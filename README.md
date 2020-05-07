# Tracer

GPU Accelerated Parallel Computed Ray Tracer

## How to build

Requires CMake to generate project files  
Requires OpenCL to generate project files and build project


1. Install SDL2 libraries and includes into /libs/SDL2
2. Add sdl2-config.cmake to SDL2 folder  


Project Directory should look like this:    
* Tracer   
   * libs  
     * SDL2  
       * sdl2-config.cmake  
       * include folder  
       * lib folder  
   * src
     * src contents  
   * CMakeLists.txt  
   * CMakeSettings.json  
   * README.md   



    Code for sdl2-config.cmake:   
    ```
    set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2main.lib")
    else ()
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2main.lib")
    endif ()

    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
    ```


3. `mkdir build`
4. `cd build`
5. Run cmake command with generator of choice and specificying the path to source as `..` (Example: `cmake .. -G "Visual Studio 16 2019"`)
