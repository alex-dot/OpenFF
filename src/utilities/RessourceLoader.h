#pragma once

#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include <string>

#include "../misc/TerminalOutput.h"

class OpenFF_Main;

namespace OpenFF {

using namespace Magnum;

class RessourceLoader{
  public:
    explicit RessourceLoader();

    void getImage(std::string, Containers::Optional<Trade::ImageData2D>&);

  private:
    PluginManager::Manager<Trade::AbstractImporter>*    _manager;
    Containers::Pointer<Trade::AbstractImporter>        _png_importer;
};

}
