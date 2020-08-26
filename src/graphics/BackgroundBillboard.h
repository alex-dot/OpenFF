#pragma once

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#include "shaders/BillboardShader.h"

namespace OpenFF {

class BackgroundBillboard{
  public:
    explicit BackgroundBillboard();

    void draw();
    void setBackground(Containers::Optional<Trade::ImageData2D> &image);
    void setRelativeBillboardRatio(Magnum::Vector2i windowSize);

    Magnum::Vector2 getRelativeBillboardRatio() {return _relative_billboard_ratio;}

    Magnum::GL::Framebuffer& getFramebuffer() {return _framebuffer;};

  private:
    BillboardShader                   _shader;
    Magnum::GL::Mesh                  _billboard;
    Magnum::GL::Texture2D             _texture;
    Magnum::GL::Texture2D             _color;
    Magnum::GL::Framebuffer           _framebuffer;
    Magnum::Vector2i                  _backgroundSize;
    Magnum::Vector2                   _relative_billboard_ratio;
};

}
