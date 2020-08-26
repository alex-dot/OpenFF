#pragma once

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#include "shaders/WindowShader.h"

namespace OpenFF {

class Window{
  public:
    explicit Window();

    void draw();
    void setBorder(Containers::Optional<Trade::ImageData2D> &image);
    void setRelativeBillboardRatio(Magnum::Vector2 relative_billboard_ratio)
            {_shader.setRelativeBillboardRatio(relative_billboard_ratio);}
    // set uniform color
    void setColor(Color3);
    // set color gradient, starting top-left going clockwise
    void setColor(Color3, Color3, Color3, Color3);
    void setViewportSize(Vector2i size) {_shader.setViewportSize(size);}
    void setBoxSize(Vector2i size) {_shader.setBoxSize(size);}
    void setOffset(Vector2i coordinates) {_shader.setOffset(coordinates);}

  private:
    WindowShader             _shader;
    Magnum::GL::Mesh         _box;
    Magnum::GL::Texture2D    _texture;
};

}
