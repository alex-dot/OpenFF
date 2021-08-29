#pragma once

#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/AbstractImageConverter.h>
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

    void getImage(std::string, Containers::Optional<Trade::ImageData2D>&, bool);
    void getImage(std::string, Containers::Optional<Trade::ImageData2D>&);
    void saveImage(std::string location, Image2D& image);
    Font* getFont(std::string font_location, int font_size);
    Font* getFont(
            std::string font_location,
            int font_size,
            std::string font_texture_location);
    std::tuple<Audio::BufferFormat,std::size_t,Containers::Array<char>,ALsizei>
            getAudio(std::string);

  private:
    PluginManager::Manager<Trade::AbstractImporter>*       _img_manager;
    PluginManager::Manager<Trade::AbstractImageConverter>* _img_exporter;
    Containers::Pointer<Trade::AbstractImporter>           _png_importer;
    Containers::Pointer<Trade::AbstractImageConverter>     _png_exporter;
    std::map<std::string,Font*>                            _font_manager;
    PluginManager::Manager<Audio::AbstractImporter>        _audio_manager;
    Containers::Pointer<Audio::AbstractImporter>           _audio_importer;
};

}
