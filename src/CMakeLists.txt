find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED
    GL
    MeshTools
    Shaders
    SceneGraph
    Trade
    Sdl2Application
    AnySceneImporter
    AnyImageImporter)
find_package(MagnumPlugins REQUIRED AssimpImporter)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(OpenFF MainApplication.cpp
    graphics/BackgroundBillboard.cpp
    graphics/DebugBox.cpp
    graphics/shaders/BillboardShader.cpp
    utilities/Configuration.cpp
    utilities/InputHandler.cpp)

add_dependencies(OpenFF
    Magnum::AnySceneImporter
    Magnum::AnyImageImporter
    MagnumPlugins::AssimpImporter
    MagnumPlugins::TinyGltfImporter
    MagnumPlugins::PngImporter)

target_include_directories(OpenFF PRIVATE
    ../libraries/inicpp/include/inicpp/)

target_link_libraries(OpenFF PRIVATE
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Trade
    inicpp)
