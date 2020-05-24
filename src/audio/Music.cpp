#include <Corrade/Containers/Reference.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/SceneGraph/Drawable.h>

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
        _sourceRig(&_scene),
        _sourceObject(&_sourceRig),
        _cameraObject(&_scene),
        _camera(_cameraObject),
        _listener{_scene},
        _global_pause(false)
{
  _importer = _manager.loadAndInstantiate("AnyAudioImporter");
  if(!_importer)
    std::exit(1);
  _camera.setViewport(GL::defaultFramebuffer.viewport().size());
}
Music::Music(InputHandler* input) : Music::Music() {
  this->bindCallbacks(input);
}
Music::Music(Configuration* config) :
        Music::Music() {
  _config = config;
  _current_track_name = _config->getRandomMusic();
  this->playNextTrack();
}
Music::Music(Configuration* config, InputHandler* input) : Music::Music(config) {
  this->bindCallbacks(input);
}

Music& Music::playNextTrack() {
  std::string track_location;
  if(_config->getMusicLocation(track_location, _current_track_name))
    track_location = _config->getMusicLocation()+track_location;
  // this currently blocks, will wait for Magnum to implement video/audio streaming
  if( !_importer->openFile(track_location) )
    std::exit(2);

  _bufferData = _importer->data();
  _buffer.setData(_importer->format(), _bufferData, _importer->frequency());

  (new Audio::Playable2D{_sourceObject, &_playables})->source()
    .setBuffer(&_buffer)
    .setLooping(true)
    .play();

  return *this;
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
  _listener.update({_playables});
  _camera.draw(_drawables);
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
