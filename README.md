## Build instructions

### Windows

1. Install CMake
1. Install Visual Studio
    - Make sure to include CMake support as part of the installation
1. Install vcpkg
1. Using vcpkg, install project dependencies: `vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-mixer:x64-windows sdl2-ttf:x64-windows`
1. Download this repo, open it up in Visual Studio, and build it
1. Optional: install the compiled binaries: Build > Install ...
1. Optional: create a zip of the compiled binaries: `cpack -G ZIP --config build-x64-Release/CPackConfig.cmake`

### Web

(Tested on Linux)

1. Install development dependencies: cmake, make, git, python3
1. Install [Emscripten](https://emscripten.org/docs/getting_started/downloads.html)
1. `git clone` this repo and `cd` into it
1. Generate build directory: `emcmake cmake -B build/`
1. Compile and link: `emmake cmake --build build/`
    - Note: there is a good chance this will fail with something like `fatal error: 'SDL2/SDL.h' file not found` because some of the required ports haven't been built and for some reason Emscripten doesn't automatically build them when invoked from CMake. Explicitly build the ports first: `embuilder build sdl2 sdl2_image sdl2_mixer sdl2_ttf`
1. `cd build`
1. Run a local server: `python3 -m http.server`
1. Visit the served page in your browser e.g. `http://localhost:8000`
1. Optional: install the compiled binaries: `cmake --install build/`
1. Optional: create a zip of the compiled binaries: `cpack -G ZIP --config build/CPackConfig.cmake`
