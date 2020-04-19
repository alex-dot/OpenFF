cmake_minimum_required(VERSION 3.4)
project(OpenFF)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/" ${CMAKE_MODULE_PATH})

# Add Corrade as a subproject
add_subdirectory(libraries/corrade EXCLUDE_FROM_ALL)

# Add Magnum as a subproject
set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
add_subdirectory(libraries/magnum EXCLUDE_FROM_ALL)

add_subdirectory(src)
