#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>
#include "Magnum/Math/Matrix3.h"

#include "../../misc/TerminalOutput.h"

using namespace Magnum;

class TextBoxShader: public GL::AbstractShaderProgram {
  public:
    typedef GL::Attribute<0, Vector2> Position;
    typedef GL::Attribute<1, Vector2> TextureCoordinates;

    explicit TextBoxShader();
    explicit TextBoxShader(Vector2i viewportSize);
    explicit TextBoxShader(
            Vector2i viewportSize,
            Vector2i boxSize,
            Vector2i offset);

    TextBoxShader& bindTexture(GL::Texture2D& texture) {
      texture.bind(TextureUnit);
      return *this;
    }

    TextBoxShader& setColor(Color3 color) {
      setUniform(_color_uniform, color);
      return *this;
    }

    TextBoxShader& setViewportSize(Vector2i size) {
      setUniform(_viewport_uniform, size);
      return *this;
    }

    TextBoxShader& setBoxSize(Vector2i size) {
      setUniform(_box_size_uniform, size);
      return *this;
    }

    TextBoxShader& setOffset(Vector2i coordinates) {
      setUniform(_offset_uniform, coordinates);
      return *this;
    }

    TextBoxShader& setRelativeBillboardRatio(const Vector2& ratio) {
      setUniform(_relative_billboard_ratio_uniform, ratio);
      return *this;
    }

  private:
    enum: int { TextureUnit = 0 };

    int _color_uniform;
    int _viewport_uniform;
    int _box_size_uniform;
    int _offset_uniform;
    int _relative_billboard_ratio_uniform;
};
