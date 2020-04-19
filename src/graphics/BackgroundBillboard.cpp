#include <Corrade/Containers/Optional.h>
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

#include <iostream>

#include "BackgroundBillboard.h"

BackgroundBillboard::BackgroundBillboard() {
    using namespace Magnum;
    using namespace Math::Literals;

    struct QuadVertex {
      Vector2 position;
      Vector2 textureCoordinates;
    };
    QuadVertex data[]{
      {{-1.0f,  1.0f}, {0.0f, 1.0f}},
      {{-1.0f, -1.0f}, {0.0f, 0.0f}},
      {{ 1.0f,  1.0f}, {1.0f, 1.0f}},
      {{ 1.0f, -1.0f}, {1.0f, 0.0f}}
    };

    GL::Buffer buffer;
    buffer.setData(data);

    _billboard.setPrimitive(MeshPrimitive::TriangleStrip)
      .setCount(4)
      .addVertexBuffer(std::move(buffer), 0,
        BillboardShader::Position{},
        BillboardShader::TextureCoordinates{});
}

void BackgroundBillboard::setBackground(Containers::Optional<Trade::ImageData2D> &image) {
    _texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
      .setMagnificationFilter(GL::SamplerFilter::Linear)
      .setMinificationFilter(GL::SamplerFilter::Linear)
      .setStorage(1, GL::textureFormat(image->format()), image->size())
      .setSubImage(0, {}, *image);
    _backgroundSize = image->size();
}

void BackgroundBillboard::draw() {
  using namespace Magnum::Math::Literals;

  Vector2 relative_billboard_ratio;
  float window_ratio = float(_windowSize.x())/float(_windowSize.y());
  float background_ratio = float(_backgroundSize.x())/float(_backgroundSize.y());
  float relative_ratio = window_ratio/background_ratio;
  if (relative_ratio >= 1.0f)
    relative_billboard_ratio = Vector2(1.0f/relative_ratio, 1.0f);
  else
    relative_billboard_ratio = Vector2(1.0f, relative_ratio);
  _shader.setRelativeBillboardRatio(relative_billboard_ratio);

  _shader.setColor(0xffffff_rgbf)
    .bindTexture(_texture)
    .draw(_billboard);
}
