#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#include "shaders/BillboardShader.h"

namespace OpenFF {

class BackgroundBillboard{
  public:
    explicit BackgroundBillboard();

    void draw();
    void setBackground(Containers::Optional<Trade::ImageData2D> &image);
    void setRelativeBillboardRatio(
            Magnum::Vector2i windowSize = Magnum::Vector2i(-1.0f));

  private:
    Magnum::GL::Mesh      _billboard;
    BillboardShader       _shader;
    Magnum::GL::Texture2D _texture;
    Magnum::Vector2i      _windowSize;
    Magnum::Vector2i      _backgroundSize;
};

}
