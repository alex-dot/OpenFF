#pragma once

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>

#include "../ui/Font.h"
#include "../utilities/InputHandler.h"

namespace OpenFF {

enum TextRenderType {
  normal,
  shadow,
  FONT_RENDER_TYPE_MAX = shadow
};

using namespace Magnum;

class Text{
  public:
    explicit Text(std::string location, int size);
    explicit Text(Font* font);

    void draw(TextRenderType = normal);
    void setRelativeBillboardRatio(Vector2);

  private:

    PluginManager::Manager<Magnum::Text::AbstractFont>  _text_manager;
    Containers::Pointer<Magnum::Text::AbstractFont>     _text;
    Magnum::Text::GlyphCache*                           _glyph_cache;
    OpenFF::Font*                                 _font;
    Shaders::DistanceFieldVector2D                _text_shader;
    GL::Buffer                                    _vertex_buffer, _index_buffer;
    GL::Mesh                                      _mesh;
    GL::Texture2D*                                _glyph_texture;
    Matrix3                                       _text_projection_matrix;
    Matrix3                                       _text_translation_matrix;
    Matrix3                                       _text_scaling_matrix;
};

}
