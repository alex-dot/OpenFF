

#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#include "shaders/BillboardShader.h"

class BackgroundBillboard{
  public:
    explicit BackgroundBillboard();

    void draw();

  private:
    Magnum::GL::Mesh      _billboard;
    BillboardShader       _shader;
    Magnum::GL::Texture2D _texture;
};
