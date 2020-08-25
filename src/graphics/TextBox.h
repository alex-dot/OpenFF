#pragma once

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#include "shaders/TextBoxShader.h"

namespace OpenFF {

class TextBox{
  public:
    explicit TextBox();

    void draw();
    void setBorder(Containers::Optional<Trade::ImageData2D> &image);
    void setRelativeBillboardRatio(Magnum::Vector2 relative_billboard_ratio)
            {_shader.setRelativeBillboardRatio(relative_billboard_ratio);}
    void setColor(Color3 color) {_shader.setColor(color);}
    void setViewportSize(Vector2i size) {_shader.setViewportSize(size);}
    void setBoxSize(Vector2i size) {_shader.setBoxSize(size);}
    void setOffset(Vector2i coordinates) {_shader.setOffset(coordinates);}

  private:
    TextBoxShader                     _shader;
    Magnum::GL::Mesh                  _box;
    Magnum::GL::Texture2D             _texture;
};

}
