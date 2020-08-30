#include "BillboardShader.h"

#include <Corrade/Containers/Reference.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

BillboardShader::BillboardShader() {
  MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL330);

  using namespace Magnum;

  GL::Shader vert{GL::Version::GL330, GL::Shader::Type::Vertex};
  GL::Shader frag{GL::Version::GL330, GL::Shader::Type::Fragment};

  vert.addFile("../src/graphics/shaders/BillboardShader.vert");
  frag.addFile("../src/graphics/shaders/BillboardShader.frag");

  CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vert, frag}));

  attachShaders({vert, frag});

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  _colorUniform = uniformLocation("color");
  setUniform(uniformLocation("textureData"), TextureUnit);
  _relativeBillboardRatioUniform = uniformLocation("relativeBillboardRatio");
  setUniform(_relativeBillboardRatioUniform, Vector2(1.0f));
}
