#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>

using namespace Magnum;

namespace OpenFF {

class MusicVisualiserShader: public GL::AbstractShaderProgram {
  public:
    typedef GL::Attribute<0, Vector2> Position;

    explicit MusicVisualiserShader();

    MusicVisualiserShader& setColor(const Color3& color) {
      setUniform(_color_uniform, color);
      return *this;
    }
    MusicVisualiserShader& setIndex(const unsigned int i) {
      setUniform(_index_uniform, float(i));
      return *this;
    }
    MusicVisualiserShader& setMaxIndex(const unsigned int i) {
      setUniform(_max_index_uniform, float(i));
      return *this;
    }
    MusicVisualiserShader& setChannel(const unsigned int i) {
      setUniform(_channel_uniform, float(i));
      return *this;
    }
    MusicVisualiserShader& setHeight(const float h) {
      setUniform(_height_uniform, h);
      return *this;
    }
    MusicVisualiserShader& setHeightPrev1(const float h) {
      setUniform(_height_prev1_uniform, h);
      return *this;
    }
    MusicVisualiserShader& setHeightPrev2(const float h) {
      setUniform(_height_prev2_uniform, h);
      return *this;
    }

  private:
    int _color_uniform;
    int _index_uniform;
    int _max_index_uniform;
    int _channel_uniform;
    int _height_uniform;
    int _height_prev1_uniform;
    int _height_prev2_uniform;
};

}
