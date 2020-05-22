#pragma once

#include <Corrade/Containers/Array.h>
#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Audio/Context.h>
#include <Magnum/Audio/Listener.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/Audio/PlayableGroup.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/MatrixTransformation2D.h>
#include <Magnum/Audio/Source.h>

#include "../utilities/InputHandler.h"
#include "../utilities/Configuration.h"

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation2D> Object2D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation2D> Scene2D;

namespace OpenFF {

class Music{
  public:
    explicit Music();
    explicit Music(InputHandler*);
    explicit Music(Configuration*);
    explicit Music(Configuration*, InputHandler*);

    void draw();
    void bindCallbacks(InputHandler*);

  private:
    Music& playNextTrack();
    Music& increaseGain();
    Music& decreaseGain();
    Music& pauseResume();

    PluginManager::Manager<Audio::AbstractImporter> _manager;
    Containers::Pointer<Audio::AbstractImporter>    _importer;
    OpenFF::Configuration*        _config;
    Audio::Context                _context;
    Containers::Array<char>       _bufferData;
    Audio::Buffer                 _buffer;
    Scene2D                       _scene;
    Object2D                      _sourceRig, _sourceObject, _cameraObject;
    SceneGraph::Camera2D          _camera;
    SceneGraph::DrawableGroup2D   _drawables;
    Audio::Listener2D             _listener;
    Audio::PlayableGroup2D        _playables;
    bool                          _global_pause;
    std::string                   _current_track_name;
};

}
