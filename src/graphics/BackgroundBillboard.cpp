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

#include "BackgroundBillboard.h"

using namespace OpenFF;

BackgroundBillboard::BackgroundBillboard() :
        _framebuffer(GL::defaultFramebuffer.viewport()) {
    using namespace Magnum;
    using namespace Math::Literals;

    _color.setStorage(1, GL::TextureFormat::RGBA8, Vector2i(4096));
    _framebuffer.attachTexture(GL::Framebuffer::ColorAttachment{0}, _color, 0);

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
    this->setRelativeBillboardRatio(_framebuffer.viewport().size());
}

void BackgroundBillboard::setRelativeBillboardRatio(Magnum::Vector2i windowSize) {
  Vector2 relative_billboard_ratio;
  float relative_ratio =   (float(windowSize.x())/float(windowSize.y()))
                         / (float(_backgroundSize.x())/float(_backgroundSize.y()));
  if (relative_ratio >= 1.0f)
    relative_billboard_ratio = Vector2(1.0f/relative_ratio, 1.0f);
  else
    relative_billboard_ratio = Vector2(1.0f, relative_ratio);
  _relative_billboard_ratio = relative_billboard_ratio;
  _shader.setRelativeBillboardRatio(_relative_billboard_ratio);
}

void BackgroundBillboard::draw() {
  using namespace Magnum::Math::Literals;

  _shader.setColor(0xffffff_rgbf)
    .bindTexture(_texture)
    .draw(_billboard);
}
