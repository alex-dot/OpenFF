#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>

#include "../../misc/TerminalOutput.h"

namespace OpenFF {

enum ColorMode {
  unicolor = 0,
  gradient = 1,
  COLOR_MODE_MAX = gradient
};

using namespace Magnum;

class WindowShader: public GL::AbstractShaderProgram {
  public:
    typedef GL::Attribute<0, Vector2> Position;
    typedef GL::Attribute<1, Vector2> TextureCoordinates;

    explicit WindowShader();
    explicit WindowShader(Vector2i viewportSize);
    explicit WindowShader(
            Vector2i viewportSize,
            Vector2i windowSize,
            Vector2i offset);

    WindowShader& bindTexture(GL::Texture2D& texture) {
      texture.bind(TextureUnit);
      return *this;
    }

    WindowShader& setHiddenStateFactor(float factor) {
      setUniform(_hidden_state_uniform, factor);
      return *this;
    }

    WindowShader& setResizeFactor(float factor) {
      setUniform(_resize_factor_uniform, factor);
      return *this;
    }

    WindowShader& setColorMode(ColorMode mode) {
      setUniform(_color_mode_uniform, mode);
      return *this;
    }

    WindowShader& setUniColor(Color3 color) {
      setUniform(_uni_color_uniform, color);
      return *this;
    }

    WindowShader& setTopLeftColor(Color3 color) {
      setUniform(_top_left_color_uniform, color);
      return *this;
    }
    WindowShader& setTopRightColor(Color3 color) {
      setUniform(_top_right_color_uniform, color);
      return *this;
    }
    WindowShader& setBottomLeftColor(Color3 color) {
      setUniform(_bottom_left_color_uniform, color);
      return *this;
    }
    WindowShader& setBottomRightColor(Color3 color) {
      setUniform(_bottom_right_color_uniform, color);
      return *this;
    }

    WindowShader& setBorderTransparency(float alpha) {
      setUniform(_border_transparency_uniform, alpha);
      return *this;
    }
    WindowShader& setBodyTransparency(float alpha) {
      setUniform(_body_transparency_uniform, alpha);
      return *this;
    }

    WindowShader& setViewportSize(Vector2i size) {
      setUniform(_viewport_uniform, size);
      return *this;
    }

    WindowShader& setWindowSize(Vector2i size) {
      setUniform(_window_size_uniform, size);
      return *this;
    }

    WindowShader& setOffset(Vector2i coordinates) {
      setUniform(_offset_uniform, coordinates);
      return *this;
    }

    WindowShader& setBorderImageLength(unsigned int length) {
      setUniform(_border_image_length, length);
      return *this;
    }

    WindowShader& setRelativeBillboardRatio(const Vector2& ratio) {
      setUniform(_relative_billboard_ratio_uniform, ratio);
      return *this;
    }

    WindowShader& disableRelativeBillboardRendering() {
      setUniform(_relative_billboard_rendering_uniform, false);
      return *this;
    }
    WindowShader& enableRelativeBillboardRendering() {
      setUniform(_relative_billboard_rendering_uniform, true);
      return *this;
    }

  private:
    enum: int { TextureUnit = 0 };

    int _hidden_state_uniform;
    int _resize_factor_uniform;
    int _color_mode_uniform;
    int _uni_color_uniform;
    int _top_left_color_uniform;
    int _top_right_color_uniform;
    int _bottom_left_color_uniform;
    int _bottom_right_color_uniform;
    int _transparency_mode_uniform;
    int _border_transparency_uniform;
    int _body_transparency_uniform;
    int _viewport_uniform;
    int _window_size_uniform;
    int _offset_uniform;
    int _border_image_length;
    int _relative_billboard_ratio_uniform;
    int _relative_billboard_rendering_uniform;
};

}
