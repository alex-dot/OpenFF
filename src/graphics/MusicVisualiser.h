#pragma once

#include <Magnum/GL/Mesh.h>

#include <vector>

#include "shaders/MusicVisualiserShader.h"

namespace OpenFF {

class MusicVisualiser{
  public:
    explicit MusicVisualiser(unsigned int bar_count);

    void draw(std::vector<float>, std::vector<float>);

  private:
    MusicVisualiserShader             _shader;
    unsigned int                      _bar_count;
    std::vector<Magnum::GL::Mesh*>    _bars;
    std::vector<float>                _previous_magnitude_bin_left_1;
    std::vector<float>                _previous_magnitude_bin_left_2;
    std::vector<float>                _previous_magnitude_bin_right_1;
    std::vector<float>                _previous_magnitude_bin_right_2;
    Magnum::GL::Texture2D             _color;
};

}
