#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>

#include "MusicVisualiser.h"
#include "../misc/TerminalOutput.h"

namespace OpenFF {

MusicVisualiser::MusicVisualiser(unsigned int bar_count) :
        _bar_count(bar_count) {
  using namespace Magnum;
  using namespace Math::Literals;

  _color.setStorage(1, GL::TextureFormat::RGBA8, Vector2i(4096));

  struct QuadVertex {
    Vector2 position;
  };
  QuadVertex data[]{
    {{-1.0f,  1.0f}},
    {{-1.0f, -1.0f}},
    {{ 1.0f,  1.0f}},
    {{ 1.0f, -1.0f}}
  };

  for( unsigned int i = 0; i < _bar_count; ++i ) {
    GL::Buffer buffer;
    buffer.setData(data);
    Magnum::GL::Mesh* bar = new Magnum::GL::Mesh();

    bar->setPrimitive(MeshPrimitive::TriangleStrip)
            .setCount(4)
            .addVertexBuffer(std::move(buffer), 0,
                 MusicVisualiserShader::Position{});

    _bars.push_back(bar);
  }
  _previous_magnitude_bin_left_1 = std::vector<float>(_bar_count,0.0f);
  _previous_magnitude_bin_left_2 = std::vector<float>(_bar_count,0.0f);
  _previous_magnitude_bin_right_1 = std::vector<float>(_bar_count,0.0f);
  _previous_magnitude_bin_right_2 = std::vector<float>(_bar_count,0.0f);
}

void MusicVisualiser::draw(
        std::vector<float> magnitude_bin_left,
        std::vector<float> magnitude_bin_right) {
  using namespace Magnum::Math::Literals;

  for( unsigned int i = 0; i < _bar_count; ++i ) {
    _shader.setColor(0xffffff_rgbf)
           .setIndex(i)
           .setMaxIndex(_bar_count)
           .setHeight(magnitude_bin_left[i])
           .setChannel(0)
           .setHeightPrev1(_previous_magnitude_bin_left_1[i])
           .setHeightPrev2(_previous_magnitude_bin_left_2[i])
           .draw(*(_bars[i]));
    _shader.setColor(0xffffff_rgbf)
           .setIndex(i)
           .setMaxIndex(_bar_count)
           .setHeight(magnitude_bin_right[i])
           .setChannel(1)
           .setHeightPrev1(_previous_magnitude_bin_right_1[i])
           .setHeightPrev2(_previous_magnitude_bin_right_2[i])
           .draw(*(_bars[i]));
  }

  std::swap(_previous_magnitude_bin_left_2,_previous_magnitude_bin_left_1);
  std::swap(magnitude_bin_left, _previous_magnitude_bin_left_2);
  std::swap(_previous_magnitude_bin_right_2,_previous_magnitude_bin_right_1);
  std::swap(magnitude_bin_right, _previous_magnitude_bin_right_2);
}

}
