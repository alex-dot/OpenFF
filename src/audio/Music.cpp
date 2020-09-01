#include <Corrade/Containers/Reference.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/SceneGraph/Drawable.h>

#include <chrono>
#include <tuple>

#include "Music.h"
#include "../misc/TerminalOutput.h"

using namespace Magnum;
using namespace Math::Literals;
using namespace OpenFF;

Music::Music() :
        _config(nullptr),
        _context{
          Audio::Context::Configuration{}
            .setHrtf(Audio::Context::Configuration::Hrtf::Enabled)
          },
        _current_track_loaded(false),
        _source_rig(&_scene),
        _source_object(&_source_rig),
        _camera_object(&_scene),
        _camera(_camera_object),
        _listener{_scene},
        _global_pause(true)
{
  _camera.setViewport(GL::defaultFramebuffer.viewport().size());
}
Music::Music(Configuration* config, RessourceLoader* ressource_loader) :
        Music::Music() {
  _config = config;
  _ressource_loader = ressource_loader;
  _current_track_name = _config->getRandomMusic();
  this->loadAudioData();
}
Music::Music(
        Configuration* config,
        RessourceLoader* ressource_loader,
        InputHandler* input) :
                Music::Music(config, ressource_loader) {
  this->bindCallbacks(input);
}

Music& Music::loadAudioData() {
  if(_config->getMusicLocation(_current_track_location, _current_track_name))
    _current_track_location = _config->getMusicLocation()+_current_track_location;
  else
    std::exit(2);

  _current_track_future = std::async(
          std::launch::async,
          &RessourceLoader::getAudio,
          _ressource_loader,
          _current_track_location);

  return *this;
}

Music& Music::playAudioData() {
  _current_track_buffer.setData(
          _current_track_format,
          _current_track_buffer_data,
          _current_track_frequency);

  (new Audio::Playable2D{_source_object, &_playables})->source()
    .setBuffer(&_current_track_buffer)
    .setLooping(true);
  if( !_global_pause ) {
    _playables.play();
  }

  return *this;
}

std::string Music::getCurrentTrackName() {
  std::string name;
  _config->getMusicName(name, _current_track_name);
  return name;
}

Music& Music::increaseGain() {
  _listener.setGain(_listener.gain()+0.2f);
  this->draw();
  return *this;
}
Music& Music::decreaseGain() {
  _listener.setGain(_listener.gain()-0.2f);
  this->draw();
  return *this;
}
Music& Music::pauseResume() {
  if( _global_pause ) {
    _playables.play();
    _global_pause = false;
  } else {
    _playables.pause();
    _global_pause = true;
  }
  return *this;
}

void Music::draw() {
  if( _current_track_loaded ) {
    _listener.update({_playables});
    _camera.draw(_drawables);
  } else {
    if( _current_track_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready ) {
      auto current_track_tuple = _current_track_future.get();
      _current_track_format = std::get<0>(current_track_tuple);
      _current_track_buffer_data = std::move(std::get<1>(current_track_tuple));
      _current_track_frequency = std::get<2>(current_track_tuple);
      this->playAudioData();
      _current_track_loaded = true;
    }
  }
}

void Music::bindCallbacks(InputHandler* input) {
  input->setCallbacks(
      *this,
      ObjectType::music,
      {
          std::make_pair(&Music::increaseGain,InputEvents::music_increase_gain),
          std::make_pair(&Music::decreaseGain,InputEvents::music_decrease_gain),
          std::make_pair(&Music::pauseResume,InputEvents::music_pause)
      }
  );
}
