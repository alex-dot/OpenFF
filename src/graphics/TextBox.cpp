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

#include "TextBox.h"

using namespace OpenFF;

TextBox::TextBox() {
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
    _box.setPrimitive(MeshPrimitive::TriangleStrip)
            .setCount(4)
            .addVertexBuffer(std::move(box_buffer), 0,
                    TextBoxShader::Position{},
                    TextBoxShader::TextureCoordinates{});
}

void TextBox::setBorder(Containers::Optional<Trade::ImageData2D> &image) {
  _texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
          .setMagnificationFilter(GL::SamplerFilter::Nearest)
          .setMinificationFilter(GL::SamplerFilter::Nearest)
          .setStorage(1 , GL::textureFormat(image->format()), image->size())
          .setSubImage(0, {}, *image);
}

void TextBox::setColor(Color3 color) {
  _shader.setColorMode(OpenFF::ColorMode::unicolor)
         .setUniColor(color);
}

void TextBox::setColor(
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

void TextBox::draw() {
  _shader.bindTexture(_texture)
         .draw(_box);
}
