#include <Corrade/Containers/Reference.h>
#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/SceneGraph/Drawable.h>

#include "Music.h"

using namespace Magnum;
using namespace Math::Literals;
using namespace OpenFF;

Music::Music() :
        _context{
          Audio::Context::Configuration{}
            .setHrtf(Audio::Context::Configuration::Hrtf::Enabled)
          },
        _sourceRig(&_scene),
        _sourceObject(&_sourceRig),
        _cameraObject(&_scene),
        _camera(_cameraObject),
        _listener{_scene}
{
  PluginManager::Manager<Audio::AbstractImporter> manager;
  Containers::Pointer<Audio::AbstractImporter> importer = manager.loadAndInstantiate("AnyAudioImporter");
  if(!importer)
    std::exit(1);
  // this currently blocks, will wait for Magnum to implement video/audio streaming
  if(!importer->openFile("../ressources/chimes.ogg"))
    std::exit(2);

  _bufferData = importer->data();
  _buffer.setData(importer->format(), _bufferData, importer->frequency());

  _camera.setViewport(GL::defaultFramebuffer.viewport().size());
  (new Audio::Playable2D{_sourceObject, &_playables})->source()
    .setBuffer(&_buffer)
    .setLooping(true)
    .play();
}

void Music::draw() {
  _listener.update({_playables});
  _camera.draw(_drawables);
}
