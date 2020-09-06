find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED
    Audio
    GL
    MeshTools
    Shaders
    SceneGraph
    Text
    Trade
    Sdl2Application
    AnySceneImporter
    AnyImageImporter
    WavAudioImporter)
find_package(MagnumPlugins REQUIRED AssimpImporter)
find_package(Threads REQUIRED)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(OpenFF MainApplication.cpp
    audio/Music.cpp
    graphics/BackgroundBillboard.cpp
    graphics/MusicVisualiser.cpp
    graphics/Text.cpp
    graphics/Window.cpp
    graphics/DebugBox.cpp
    graphics/shaders/BillboardShader.cpp
    graphics/shaders/MusicVisualiserShader.cpp
    graphics/shaders/WindowShader.cpp
    ui/Font.cpp
    ui/MusicMenu.cpp
    ui/Textbox.cpp
    utilities/Configuration.cpp
    utilities/InputHandler.cpp
    utilities/RessourceLoader.cpp)

add_dependencies(OpenFF
    Magnum::AnySceneImporter
    Magnum::AnyImageImporter
    Magnum::AnyAudioImporter
    Magnum::WavAudioImporter
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
    Magnum::Text
    Magnum::Trade
    inicpp
    muFFT
    Threads::Threads)
