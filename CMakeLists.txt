cmake_minimum_required(VERSION 3.4)
project(OpenFF)

set(BUILD_STATIC OFF CACHE BOOL "")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/" ${CMAKE_MODULE_PATH})

# Add Corrade as a subproject
add_subdirectory(libraries/corrade EXCLUDE_FROM_ALL)

# Add Magnum as a subproject
set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
set(WITH_AUDIO ON CACHE BOOL "" FORCE)
set(WITH_ANYSCENEIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_ANYIMAGEIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_ANYAUDIOIMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(libraries/magnum EXCLUDE_FROM_ALL)

# Add minIni as a subproject
set(NO_GOOGLE_TEST ON CACHE BOOL "" FORCE)
add_subdirectory(libraries/inicpp EXCLUDE_FROM_ALL)

# Add yaml-cpp as a subproject
add_subdirectory(libraries/yaml-cpp EXCLUDE_FROM_ALL)

# Add Assimp with unwanted parts disabled, help Magnum find everything needed
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INCLUDE_DIR libraries/assimp/include CACHE STRING "" FORCE)
set(ASSIMP_LIBRARY_DEBUG assimp CACHE STRING "" FORCE)
set(ASSIMP_LIBRARY_RELEASE assimp CACHE STRING "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
add_subdirectory(libraries/assimp EXCLUDE_FROM_ALL)
add_library(Assimp::Assimp ALIAS assimp)

# Add Magnum-Plugins as a subproject
set(WITH_PNGIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_ASSIMPIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_TINYGLTFIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_DRFLACAUDIOIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_DRMP3AUDIOIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_STBVORBISAUDIOIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_STBTRUETYPEFONT ON CACHE BOOL "" FORCE)
add_subdirectory(libraries/magnum-plugins EXCLUDE_FROM_ALL)

add_subdirectory(src)
