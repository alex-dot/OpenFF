find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED GL Trade Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(OpenFF MainApplication.cpp
    graphics/BackgroundBillboard.cpp
    graphics/shaders/BillboardShader.cpp)

add_dependencies(OpenFF MagnumPlugins::PngImporter)

target_link_libraries(OpenFF PRIVATE
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    Magnum::Trade)