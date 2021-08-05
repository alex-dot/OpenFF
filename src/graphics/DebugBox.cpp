#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/ImageView.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/PixelFormat.h>

#include "DebugBox.h"

using namespace Magnum;
using namespace Math::Literals;
using namespace OpenFF;

class ColoredDrawable: public SceneGraph::Drawable3D {
    public:
        explicit ColoredDrawable(Object3D& object,
                                 Shaders::FlatGL3D& shader,
                                 GL::Mesh& mesh,
                                 const Color4& color,
                                 SceneGraph::DrawableGroup3D& group) :
                         SceneGraph::Drawable3D{object, &group},
                         _shader(shader),
                         _mesh(mesh),
                         _color{color} {};

    private:
        void draw(const Matrix4& transformationMatrix,
                  SceneGraph::Camera3D& camera) override;

        Shaders::FlatGL3D& _shader;
        GL::Mesh& _mesh;
        Color4 _color;
};
void ColoredDrawable::draw(const Matrix4& transformationMatrix,
                           SceneGraph::Camera3D& camera) {
  _shader
      .setColor(_color)
      .setTransformationProjectionMatrix
              (camera.projectionMatrix()*transformationMatrix)
      .draw(_mesh);
}

DebugBox::DebugBox() {
  _cameraObject
      .setParent(&_scene)
      .translate(Vector3::zAxis(35.0f));
  (*(_camera = new SceneGraph::Camera3D{_cameraObject}))
      .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
      .setProjectionMatrix(Matrix4::perspectiveProjection(35.0_degf,
                                                          1.0f,
                                                          0.01f,
                                                          1000.0f))
      .setViewport(GL::defaultFramebuffer.viewport().size());
  _sceneRoot.setParent(&_scene);
  _coloredShader.setColor(0xff0000_rgbf);

  _cube  = MeshTools::compile(Primitives::cubeWireframe());
  new ColoredDrawable{_sceneRoot, _coloredShader, _cube, 0xff0000_rgbf, _drawables};
  _sceneRoot.rotateX(45.0_degf);
  _sceneRoot.rotateY(45.0_degf);
}

void DebugBox::draw() {
  _camera->draw(_drawables);
}
