#pragma once

#include <Corrade/Containers/Array.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Audio/Context.h>
#include <Magnum/Audio/Listener.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/Audio/PlayableGroup.h>
#include <Magnum/Audio/Source.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/MatrixTransformation2D.h>
#include <Magnum/Timeline.h>

#include <future>

#include "../graphics/MusicVisualiser.h"
#include "../utilities/InputHandler.h"
#include "../utilities/Configuration.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::MatrixTransformation2D> Object2D;
typedef SceneGraph::Scene<SceneGraph::MatrixTransformation2D> Scene2D;

namespace OpenFF {

enum MusicVisualiserChannel {
  channel_left,
  channel_right,
  MUSIC_VISUALISER_CHANNEL_MAX = channel_right
};

float blackmanHarrisNuttalWindowTransform(float, unsigned int, unsigned int);

class Music{
  public:
    explicit Music();
    explicit Music(Configuration*, RessourceLoader*);
    explicit Music(Configuration*, RessourceLoader*, InputHandler*);

    void initStart();
    void draw();
    void bindCallbacks(InputHandler*);

    std::string getCurrentTrackName();
    bool isPaused();

    Music& increaseGain();
    Music& decreaseGain();
    Music& pauseResume();

    std::tuple<unsigned int,unsigned int,float> getVisualiserInformation();
    std::vector<std::vector<float>> processAudioForMagnitudeVisualiser(
            const MusicVisualiserChannel,
            const unsigned int,
            const unsigned int);

  private:
    Music& loadAudioData();
    Music& storeAudioData();
    Music& playAudioData();

    Music& prepareForMagnitudeVisualiser();

    void drawVisualiser();

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

    unsigned int                     _samples;
    unsigned int                     _channels;
    float                            _max_magnitude;
    std::vector<float>               _frequency_bin;
    unsigned int                     _visualiser_bar_count;
    unsigned int                     _maximum_frame_count;
    unsigned int                     _frame_slice_left;
    unsigned int                     _frame_slice_right;
    float                            _frame_window_size;
    std::vector<std::vector<float>>  _magnitude_bin_matrix_left;
    std::vector<std::vector<float>>  _magnitude_bin_matrix_right;
};

}
