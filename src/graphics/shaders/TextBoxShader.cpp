#include "TextBoxShader.h"

#include <Corrade/Containers/Reference.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

TextBoxShader::TextBoxShader() {
  MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL330);

  using namespace Magnum;

  GL::Shader vert{GL::Version::GL330, GL::Shader::Type::Vertex};
  GL::Shader frag{GL::Version::GL330, GL::Shader::Type::Fragment};

  vert.addFile("../src/graphics/shaders/TextBoxShader.vert");
  frag.addFile("../src/graphics/shaders/TextBoxShader.frag");

  CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vert, frag}));

  attachShaders({vert, frag});

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  setUniform(uniformLocation("textureData"), TextureUnit);

  _color_uniform = uniformLocation("boxColor");
  _viewport_uniform = uniformLocation("viewportSize");
  _box_size_uniform = uniformLocation("boxSize");
  _offset_uniform = uniformLocation("boxOffset");
  _relative_billboard_ratio_uniform = uniformLocation("relativeBillboardRatio");
  setUniform(_relative_billboard_ratio_uniform, Vector2(1.0f));
}

TextBoxShader::TextBoxShader(Vector2i viewport_size)
        : TextBoxShader::TextBoxShader() {
  setViewportSize(viewport_size);
}

TextBoxShader::TextBoxShader(
        Vector2i viewport_size,
        Vector2i box_size,
        Vector2i offset)
        : TextBoxShader::TextBoxShader() {
  setViewportSize(viewport_size);
  setBoxSize(box_size);
  setOffset(offset);
}
