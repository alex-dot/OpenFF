#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Color.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>

#include "Window.h"

using namespace OpenFF;

Window::Window() :
        _hidden(true),
        _hide_state(hide_state_min) {
  struct QuadVertex {
    Vector2 position;
    Vector2 textureCoordinates;
  };
  QuadVertex box_data[]{
    {{-1.0f,  1.0f}, {0.0f, 0.0f}},
    {{-1.0f, -1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
    {{ 1.0f, -1.0f}, {1.0f, 0.0f}}
  };
  GL::Buffer box_buffer;
  box_buffer.setData(box_data);
  _window.setPrimitive(MeshPrimitive::TriangleStrip)
         .setCount(4)
         .addVertexBuffer(std::move(box_buffer), 0,
                  WindowShader::Position{},
                  WindowShader::TextureCoordinates{});
  _shader.setHiddenStateFactor(0.0f);
}

Window::Window(Vector2i viewport_size) :
        Window::Window() {
  this->setViewportSize(viewport_size);
}

void Window::setBorder(Containers::Optional<Trade::ImageData2D> &image) {
  _texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
          .setMagnificationFilter(GL::SamplerFilter::Nearest)
          .setMinificationFilter(GL::SamplerFilter::Nearest)
          .setStorage(1 , GL::textureFormat(image->format()), image->size())
          .setSubImage(0, {}, *image);
}

void Window::setColor(Color3 color) {
  _shader.setColorMode(OpenFF::ColorMode::unicolor)
         .setUniColor(color);
}
void Window::setColor(Color4 color) {
  _shader.setColorMode(OpenFF::ColorMode::unicolor)
         .setUniColor(color.rgb())
         .setBodyTransparency(color.a());
}

void Window::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color) {
  _shader.setColorMode(OpenFF::ColorMode::gradient)
         .setTopLeftColor(top_left_color)
         .setTopRightColor(top_right_color)
         .setBottomRightColor(bottom_right_color)
         .setBottomLeftColor(bottom_left_color);
}
void Window::setColor(
        Color3 top_left_color,
        Color3 top_right_color,
        Color3 bottom_right_color,
        Color3 bottom_left_color,
        float alpha) {
  _shader.setColorMode(OpenFF::ColorMode::gradient)
         .setTopLeftColor(top_left_color)
         .setTopRightColor(top_right_color)
         .setBottomRightColor(bottom_right_color)
         .setBottomLeftColor(bottom_left_color)
         .setBodyTransparency(alpha);
}

void Window::setBodyTransparency(float alpha) {
  _shader.setBodyTransparency(alpha);
}
void Window::setBorderTransparency(float alpha) {
  _shader.setBorderTransparency(alpha);
}

void Window::draw() {
  if( !_hidden && _hide_state < hide_state_max ) {
    _hide_state = _hide_state + hide_state_step;
    _shader.setHiddenStateFactor(float(_hide_state)/float(hide_state_max));
  } else if ( _hidden && _hide_state > hide_state_min ) {
    _hide_state = _hide_state - hide_state_step;
    _shader.setHiddenStateFactor(float(_hide_state)/float(hide_state_max));
  }

  _shader.bindTexture(_texture)
         .draw(_window);
}

void Window::show() {
  _hidden = false;
  _hide_state = hide_state_min;
}
void Window::hide() {
  _hidden = true;
  _hide_state = hide_state_max;
}
bool Window::isFullyShown() {
  if( !_hidden && _hide_state == hide_state_max ) return true;
  return false;
}
