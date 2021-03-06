#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/Trade/MeshData.h>

#include "Window.h"

using namespace OpenFF;

Window::Window() :
        _hidden(true),
        _render_instantly(false),
        _render_state(window_render_state_min) {
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
  if( !_render_instantly ) {
    if( !_hidden && _render_state < window_render_state_max ) {
      _render_state = _render_state + window_render_state_step;
      _shader.setHiddenStateFactor(float(_render_state)/float(window_render_state_max));
    } else if ( !_render_instantly && _hidden && _render_state > window_render_state_min ) {
      _render_state = _render_state - window_render_state_step;
      _shader.setHiddenStateFactor(float(_render_state)/float(window_render_state_max));
    }
  } else {
    if( _hidden ) {
      _shader.setHiddenStateFactor(0.0f);
    } else {
      _shader.setHiddenStateFactor(1.0f);
    }
  }

  _shader.bindTexture(_texture)
         .draw(_window);
}

void Window::show() {
  _hidden = false;
  _render_state = window_render_state_min;
}
void Window::hide() {
  _hidden = true;
  _render_state = window_render_state_max;
}
bool Window::isFullyShown() {
  if( !_hidden && _render_state == window_render_state_max ) return true;
  if( !_hidden && _render_instantly == true ) return true;
  return false;
}

void Window::enableInstantRendering() {
  _render_instantly = true;
}
void Window::disableInstantRendering() {
  _render_instantly = false;
}

void Window::enableRelativeBillboardRendering() {
  _shader.enableRelativeBillboardRendering();
}
void Window::disableRelativeBillboardRendering() {
  _shader.disableRelativeBillboardRendering();
}
