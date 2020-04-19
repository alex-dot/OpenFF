find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED GL Trade Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(OpenFF MainApplication.cpp
    graphics/BackgroundBillboard.cpp)
target_link_libraries(OpenFF PRIVATE
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::Shaders)

#corrade_add_resource(TexturedTriangle_RESOURCES TexturedTriangleResources.conf)
#add_executable(MyTexturedTriangle
#  MyTexturedTriangle.cpp
#  TexturedTriangleShader.cpp
#  TexturedTriangleShader.h
#  ${TexturedTriangle_RESOURCES})
## So the TgaImporter gets built implicitly
#add_dependencies(MyTexturedTriangle Magnum::TgaImporter)
#target_link_libraries(MyTexturedTriangle PRIVATE
#  Corrade::Main
#  Magnum::Application
#  Magnum::GL
#  Magnum::Magnum
#  Magnum::Trade)
