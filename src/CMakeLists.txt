find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED
    Audio
    GL
    MeshTools
    Shaders
    SceneGraph
    Trade
    Sdl2Application
    AnySceneImporter
    AnyImageImporter)
find_package(MagnumPlugins REQUIRED AssimpImporter)
find_package(MagnumExtras REQUIRED Ui)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(OpenFF MainApplication.cpp
    audio/Music.cpp
    graphics/BackgroundBillboard.cpp
    graphics/Window.cpp
    graphics/Text.cpp
    graphics/DebugBox.cpp
    graphics/shaders/BillboardShader.cpp
    graphics/shaders/WindowShader.cpp
    ui/Font.cpp
    ui/MusicMenu.cpp
    utilities/Configuration.cpp
    utilities/InputHandler.cpp
    utilities/RessourceLoader.cpp)

add_dependencies(OpenFF
    Magnum::AnySceneImporter
    Magnum::AnyImageImporter
    Magnum::AnyAudioImporter
    MagnumPlugins::AssimpImporter
    MagnumPlugins::TinyGltfImporter
    MagnumPlugins::PngImporter
    MagnumPlugins::DrFlacAudioImporter
    MagnumPlugins::DrMp3AudioImporter
    MagnumPlugins::StbVorbisAudioImporter
    MagnumPlugins::StbTrueTypeFont)

target_include_directories(OpenFF PRIVATE
    ../libraries/inicpp/include/inicpp/)

target_link_libraries(OpenFF PRIVATE
    Corrade::Main
    Magnum::Audio
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Trade
    MagnumExtras::Ui
    inicpp)
