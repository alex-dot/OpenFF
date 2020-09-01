#pragma once

#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include <string>
#include <map>
#include <tuple>

#include "../ui/Font.h"
#include "../misc/TerminalOutput.h"

namespace OpenFF {

using namespace Magnum;

class RessourceLoader{
  public:
    explicit RessourceLoader();

    void getImage(std::string, Containers::Optional<Trade::ImageData2D>&);
    Font* getFont(std::string, int);
    std::tuple<Audio::BufferFormat,Containers::Array<char>,ALsizei>
            getAudio(std::string);

  private:
    PluginManager::Manager<Trade::AbstractImporter>*    _img_manager;
    Containers::Pointer<Trade::AbstractImporter>        _png_importer;
    std::map<std::string,Font*>                         _font_manager;
    PluginManager::Manager<Audio::AbstractImporter>     _audio_manager;
    Containers::Pointer<Audio::AbstractImporter>        _audio_importer;
};

}
