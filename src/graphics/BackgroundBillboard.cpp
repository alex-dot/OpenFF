

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColor.h>

#include "BackgroundBillboard.h"

BackgroundBillboard::BackgroundBillboard() {
    using namespace Magnum;
    using namespace Math::Literals;

    struct TriangleVertex {
        Vector3 position;
        Color3 color;
    };
    const TriangleVertex data[]{
        {{-0.5f, -0.5f,  0.0f}, 0xff0000_rgbf},
        {{ 0.5f, -0.5f,  0.0f}, 0x00ff00_rgbf},
        {{ 0.0f,  0.5f,  1.0f}, 0x0000ff_rgbf}
    };

    GL::Buffer buffer;
    buffer.setData(data);

    _billboard.setCount(3)
         .addVertexBuffer(std::move(buffer), 0,
            Shaders::VertexColor3D::Position{},
            Shaders::VertexColor3D::Color3{});
}

void BackgroundBillboard::draw() {
  _shader.draw(_billboard);
}
