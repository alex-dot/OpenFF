#pragma once

#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include <string>
#include <map>

#include "../ui/Font.h"
#include "../misc/TerminalOutput.h"

namespace OpenFF {

using namespace Magnum;

class RessourceLoader{
  public:
    explicit RessourceLoader();

    void getImage(std::string, Containers::Optional<Trade::ImageData2D>&);
    Font* getFont(std::string, int);

  private:
    PluginManager::Manager<Trade::AbstractImporter>*    _img_manager;
    Containers::Pointer<Trade::AbstractImporter>        _png_importer;
    std::map<std::string,Font*>                         _font_manager;
};

}
