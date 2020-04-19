

#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

class BackgroundBillboard{
  public:
    explicit BackgroundBillboard();

    void draw();

  private:
    Magnum::GL::Mesh _billboard;
    Magnum::Shaders::VertexColor3D _shader;
};
