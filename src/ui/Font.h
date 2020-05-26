#pragma once

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>

#include "../utilities/InputHandler.h"

using namespace Magnum;

namespace OpenFF {

enum FontRenderType {
  normal,
  shadow,
  FONT_RENDER_TYPE_MAX = shadow
};

class Font{
  public:
    explicit Font(std::string, int);

    // Font has no extra Framebuffer and renders to previously bound FB
    void draw(FontRenderType = normal);
//    void bindCallbacks(InputHandler*);
    void setRelativeBillboardRatio(Magnum::Vector2);

  private:

    PluginManager::Manager<Text::AbstractFont>  _font_manager;
    Containers::Pointer<Text::AbstractFont>     _font;
    Text::GlyphCache*                           _glyph_cache;
    Shaders::DistanceFieldVector2D              _text_shader;
    GL::Buffer                                  _vertex_buffer, _index_buffer;
    GL::Mesh                                    _mesh;
    GL::Texture2D*                              _glyph_texture;
    Matrix3                                     _text_projection_matrix;
    Matrix3                                     _text_translation_matrix;
    Matrix3                                     _text_scaling_matrix;
};

}
