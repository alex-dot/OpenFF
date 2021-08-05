#pragma once

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/MeshObjectData3D.h>
#include <Magnum/Trade/SceneData.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/ImageView.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/PixelFormat.h>

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

namespace OpenFF {

class DebugBox{
  public:
    explicit DebugBox();

    void draw();

  private:
    GL::Mesh                      _cube;
    Shaders::FlatGL3D             _coloredShader;
    Scene3D                       _scene;
    Object3D                      _sceneRoot, _cameraObject;
    SceneGraph::Camera3D*         _camera;
    SceneGraph::DrawableGroup3D   _drawables;
};

}
