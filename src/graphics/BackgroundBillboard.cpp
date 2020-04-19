#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
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

BackgroundBillboard::BackgroundBillboard() {
    using namespace Magnum;
    using namespace Math::Literals;

    struct QuadVertex {
      Vector2 position;
      Vector2 textureCoordinates;
    };
    const QuadVertex data[]{
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

    PluginManager::Manager<Trade::AbstractImporter> manager;
    Containers::Pointer<Trade::AbstractImporter> importer =
      manager.loadAndInstantiate("PngImporter");
    if(!importer) std::exit(1);

    if(!importer->openFile("../../ff7/data/flevel/ancnt1.png")) std::exit(2);
    Containers::Optional<Trade::ImageData2D> image = importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
    _texture.setWrapping(GL::SamplerWrapping::ClampToEdge)
      .setMagnificationFilter(GL::SamplerFilter::Linear)
      .setMinificationFilter(GL::SamplerFilter::Linear)
      .setStorage(1, GL::textureFormat(image->format()), image->size())
      .setSubImage(0, {}, *image);
}

void BackgroundBillboard::draw() {
  using namespace Magnum::Math::Literals;
  _shader.setColor(0xffb2b2_rgbf)
    .bindTexture(_texture)
    .draw(_billboard);
}
