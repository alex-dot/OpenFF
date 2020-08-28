#include "RessourceLoader.h"
#include "../misc/TerminalOutput.h"

namespace OpenFF {

RessourceLoader::RessourceLoader() {
  _manager = new PluginManager::Manager<Trade::AbstractImporter>;
  _png_importer = _manager->loadAndInstantiate("PngImporter");
  if(!(_png_importer)) FatlIOError("Could not load PngImporter Plugin");
}

void RessourceLoader::getImage(
        std::string location,
        Containers::Optional<Trade::ImageData2D>& image) {
  if(!_png_importer->openFile(location))
    FatlIOError("Could not load Image at location: "+location);
  image = _png_importer->image2D(0);
  CORRADE_INTERNAL_ASSERT(image);
}

}
