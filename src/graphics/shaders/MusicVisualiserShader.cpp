#include "MusicVisualiserShader.h"

#include <Corrade/Containers/Reference.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

namespace OpenFF {

MusicVisualiserShader::MusicVisualiserShader() {
  MAGNUM_ASSERT_GL_VERSION_SUPPORTED(GL::Version::GL330);

  using namespace Magnum;

  GL::Shader vert{GL::Version::GL330, GL::Shader::Type::Vertex};
  GL::Shader frag{GL::Version::GL330, GL::Shader::Type::Fragment};

  vert.addFile("../src/graphics/shaders/MusicVisualiserShader.vert");
  frag.addFile("../src/graphics/shaders/MusicVisualiserShader.frag");

  CORRADE_INTERNAL_ASSERT_OUTPUT(GL::Shader::compile({vert, frag}));

  attachShaders({vert, frag});

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  _color_uniform = uniformLocation("color");
  _index_uniform = uniformLocation("index");
  _max_index_uniform = uniformLocation("max_index");
  _channel_uniform = uniformLocation("channel");
  _height_uniform = uniformLocation("height");
  _height_prev1_uniform = uniformLocation("height_prev1");
  _height_prev2_uniform = uniformLocation("height_prev2");
}

}
