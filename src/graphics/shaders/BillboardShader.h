#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>

using namespace Magnum;

class BillboardShader: public GL::AbstractShaderProgram {
  public:
    typedef GL::Attribute<0, Vector2> Position;
    typedef GL::Attribute<1, Vector2> TextureCoordinates;

    explicit BillboardShader();

    BillboardShader& setColor(const Color3& color) {
      setUniform(_colorUniform, color);
      return *this;
    }

    BillboardShader& bindTexture(GL::Texture2D& texture) {
      texture.bind(TextureUnit);
      return *this;
    }

    BillboardShader& setRelativeBillboardRatio(const Vector2& ratio) {
      setUniform(_relativeBillboardRatioUniform, ratio);
      return *this;
    }

  private:
    enum: Int { TextureUnit = 0 };

    Int _colorUniform;
    Int _relativeBillboardRatioUniform;
};
