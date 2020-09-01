#pragma once

#include <Corrade/Containers/Array.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Audio/Context.h>
#include <Magnum/Audio/Listener.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/Audio/PlayableGroup.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/MatrixTransformation2D.h>
#include <Magnum/Audio/Source.h>

#include <future>

#include "../utilities/InputHandler.h"
#include "../utilities/Configuration.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation2D> Object2D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation2D> Scene2D;

namespace OpenFF {

class Music{
  public:
    explicit Music();
    explicit Music(Configuration*, RessourceLoader*);
    explicit Music(Configuration*, RessourceLoader*, InputHandler*);

    void initStart();
    void draw();
    void bindCallbacks(InputHandler*);

    std::string getCurrentTrackName();
    bool isPaused() { return _global_pause; }

    Music& loadAudioData();
    Music& playAudioData();

    Music& increaseGain();
    Music& decreaseGain();
    Music& pauseResume();

  private:
    OpenFF::Configuration*        _config;
    OpenFF::RessourceLoader*      _ressource_loader;
    Audio::Context                _context;
    bool                          _current_track_loaded;
    Containers::Array<char>       _current_track_buffer_data;
    Audio::BufferFormat           _current_track_format;
    ALsizei                       _current_track_frequency;
    std::future<std::tuple<
      Audio::BufferFormat,
      Containers::Array<char>,
      ALsizei>
    >                             _current_track_future;
    Audio::Buffer                 _current_track_buffer;
    std::string                   _current_track_name;
    std::string                   _current_track_location;
    Scene2D                       _scene;
    Object2D                      _source_rig, _source_object, _camera_object;
    SceneGraph::Camera2D          _camera;
    SceneGraph::DrawableGroup2D   _drawables;
    Audio::Listener2D             _listener;
    Audio::PlayableGroup2D        _playables;
    bool                          _global_pause;
};

}
