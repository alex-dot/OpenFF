#pragma once

#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Trade/ImageData.h>

#include "shaders/WindowShader.h"

namespace OpenFF {

class Window{
  public:
    explicit Window();
    explicit Window(Vector2i viewport_size);

    void draw();
    void setBorder(Containers::Optional<Trade::ImageData2D> &image);
    void setRelativeBillboardRatio(Magnum::Vector2 relative_billboard_ratio)
            {_shader.setRelativeBillboardRatio(relative_billboard_ratio);}
    // set uniform color
    void setColor(Color3);
    void setColor(Color4);
    // set color gradient, starting top-left going clockwise
    void setColor(Color3, Color3, Color3, Color3);
    void setColor(Color3, Color3, Color3, Color3, float);
    void setBodyTransparency(float);
    void setBorderTransparency(float);
    void setViewportSize(Vector2i size) {_shader.setViewportSize(size);}
    void setWindowSize(Vector2i size) {_shader.setWindowSize(size);}
    void setOffset(Vector2i coordinates) {_shader.setOffset(coordinates);}

  private:
    WindowShader             _shader;
    Magnum::GL::Mesh         _window;
    Magnum::GL::Texture2D    _texture;
};

}
