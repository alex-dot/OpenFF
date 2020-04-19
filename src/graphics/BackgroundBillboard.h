

#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#include "shaders/BillboardShader.h"

class BackgroundBillboard{
  public:
    explicit BackgroundBillboard();

    void draw();
    void setBackground(Containers::Optional<Trade::ImageData2D> &image);

    void setWindowSize(Magnum::Vector2i windowSize) {
      _windowSize = windowSize;
    };

  private:
    Magnum::GL::Mesh      _billboard;
    BillboardShader       _shader;
    Magnum::GL::Texture2D _texture;
    Magnum::Vector2i      _windowSize;
    Magnum::Vector2i      _backgroundSize;
};
