#include "WindowShader.h"

#include <Corrade/Containers/Reference.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

namespace OpenFF {

WindowShader::WindowShader() {
  MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL330);

  using namespace Magnum;

  GL::Shader vert{GL::Version::GL330, GL::Shader::Type::Vertex};
  GL::Shader frag{GL::Version::GL330, GL::Shader::Type::Fragment};

  vert.addFile("../src/graphics/shaders/WindowShader.vert");
  frag.addFile("../src/graphics/shaders/WindowShader.frag");

  CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vert, frag}));

  attachShaders({vert, frag});

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  setUniform(uniformLocation("textureData"), TextureUnit);

  _hidden_state_uniform = uniformLocation("hideFactor");
  _color_mode_uniform = uniformLocation("windowColorMode");
  _uni_color_uniform = uniformLocation("windowUniColor");
  _top_left_color_uniform = uniformLocation("windowTopLeftColor");
  _top_right_color_uniform = uniformLocation("windowTopRightColor");
  _bottom_left_color_uniform = uniformLocation("windowBottomLeftColor");
  _bottom_right_color_uniform = uniformLocation("windowBottomRightColor");
  _border_transparency_uniform = uniformLocation("windowBorderTransparency");
  _body_transparency_uniform = uniformLocation("windowBodyTransparency");
  _viewport_uniform = uniformLocation("viewportSize");
  _window_size_uniform = uniformLocation("windowSize");
  _offset_uniform = uniformLocation("windowOffset");
  _relative_billboard_ratio_uniform = uniformLocation("relativeBillboardRatio");
  setUniform(_relative_billboard_ratio_uniform, Vector2(1.0f));
}

WindowShader::WindowShader(Vector2i viewport_size)
        : WindowShader::WindowShader() {
  setViewportSize(viewport_size);
}

WindowShader::WindowShader(
        Vector2i viewport_size,
        Vector2i window_size,
        Vector2i offset)
        : WindowShader::WindowShader() {
  setViewportSize(viewport_size);
  setWindowSize(window_size);
  setOffset(offset);
}

}
