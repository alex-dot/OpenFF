cmake_minimum_required(VERSION 3.4)
project(OpenFF)

set(BUILD_STATIC OFF CACHE BOOL "")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/" ${CMAKE_MODULE_PATH})

# Add Corrade as a subproject
add_subdirectory(libraries/corrade EXCLUDE_FROM_ALL)

# Add Magnum as a subproject
set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
add_subdirectory(libraries/magnum EXCLUDE_FROM_ALL)

# Add Magnum-Plugins as a subproject
set(WITH_PNGIMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(libraries/magnum-plugins EXCLUDE_FROM_ALL)

# Add minIni as a subproject
set(NO_GOOGLE_TEST ON CACHE BOOL "" FORCE)
add_subdirectory(libraries/inicpp EXCLUDE_FROM_ALL)

add_subdirectory(src)
