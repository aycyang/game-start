# x_vcpkg_install_local_dependencies requires CMake >=3.14
# NOTE(2022-07-23): I would go higher but Ubuntu 20.04 LTS only has CMake 3.16.3
cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME MyGame)

project(${PROJECT_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Generate compile_commands.json so that clangd can know how the project is
# built so that it can highlight compilation errors in your code editor
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} main.cc)

configure_file(assets/SDL_logo.png assets/SDL_logo.png COPYONLY)
configure_file(assets/bloop.wav assets/bloop.wav COPYONLY)
configure_file(assets/Inconsolata.ttf assets/Inconsolata.ttf COPYONLY)

IF(EMSCRIPTEN)
  # To build with Emscripten, I'm using the hardcoded flags to pull in ports.
  # NOTE: using CMake to build via Emscripten won't work until you've built the
  # ports explicitly (weird because invoking emcc with these flags directly
  # will trigger the ports to be built if they aren't already). You can
  # explicitly build the ports with e.g. 'embuilder build sdl2'.
  # Observed with Emscripten version 3.1.7
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL=2)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL_IMAGE=2)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL_TTF=2)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_SDL_MIXER=2)
  target_link_options(${PROJECT_NAME} PRIVATE -sUSE_VORBIS=1)
  target_link_options(${PROJECT_NAME} PRIVATE --use-preload-plugins)
  target_link_options(${PROJECT_NAME} PRIVATE --preload-file assets)
  configure_file(index.html index.html @ONLY)
  set(CMAKE_INSTALL_PREFIX install)
  install(FILES
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.wasm
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.js
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.data
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/index.html
    DESTINATION .
  )
ELSEIF(WIN32)
  # Prerequisite: 'vcpkg install' all of the following packages
  # For some reason the naming conventions of these packages aren't very
  # consistent. That's just how it is on vcpkg.
  # Tested with Visual Studio 2022 and vcpkg.
  find_package(SDL2 REQUIRED)
  find_package(sdl2-image REQUIRED)
  find_package(sdl2-ttf REQUIRED)
  find_package(SDL2_mixer REQUIRED)
  target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
    SDL2::SDL2main
    SDL2::SDL2_image
    SDL2::SDL2_ttf
    SDL2_mixer::SDL2_mixer
  )

  install(TARGETS ${PROJECT_NAME} DESTINATION .)
  install(DIRECTORY assets DESTINATION .)
  # NOTE (2022-07-23): vcpkg already injects code into CMake's native
  # add_library() function to copy the required DLLs into the build directory,
  # but it doesn't do the same for the install directory. Here I'm using the
  # experimental function x_vcpkg_install_local_dependencies to fix that
  # inconsistency.
  x_vcpkg_install_local_dependencies(TARGETS ${PROJECT_NAME} DESTINATION .)
ELSEIF(UNIX)
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_ttf REQUIRED)
  find_package(SDL2_mixer REQUIRED)

  target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image
    SDL2_ttf
    SDL2_mixer::SDL2_mixer
  )

  # NOTE: distribution for Linux is hard, so I'm not going to try to handle
  # that with CMake.
  # It can be done in an architecture-agnostic ("universal") way by bundling
  # 32-bit and 64-bit builds and using a shell script [1] to select between them.
  # Libraries can be bundled and loaded manually using LD_LIBRARY_PATH [2].
  # Steam provides a container runtime [3].
  # [1] https://gist.github.com/flibitijibibo/5365145
  # [2] https://itch.io/docs/itch/integrating/platforms/linux.html
  # [3] https://partner.steamgames.com/doc/store/application/platforms/linux
ENDIF()

# CPack can generate a zip of the compiled binaries. It is equivalent to
# doing a CMake install and adding the contents of the install directory to a
# zip.  Example usage: cd build/; cpack -G ZIP
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
