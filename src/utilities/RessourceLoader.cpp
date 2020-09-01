#include "RessourceLoader.h"
#include "../misc/TerminalOutput.h"

namespace OpenFF {

RessourceLoader::RessourceLoader() {
  _img_manager = new PluginManager::Manager<Trade::AbstractImporter>;
  _png_importer = _img_manager->loadAndInstantiate("PngImporter");
  if(!(_png_importer)) FatlIOError("Could not load PngImporter Plugin");

  _audio_importer = _audio_manager.loadAndInstantiate("AnyAudioImporter");
  if(!_audio_importer) FatlIOError("Could not load AnyAudioImporter Plugin");
}

void RessourceLoader::getImage(
        std::string location,
        Containers::Optional<Trade::ImageData2D>& image) {
  if(!_png_importer->openFile(location))
    FatlIOError("Could not load Image at location: "+location);
  image = _png_importer->image2D(0);
  CORRADE_INTERNAL_ASSERT(image);
}

Font* RessourceLoader::getFont(std::string font_location, int font_size) {
  Font* font;
  std::map<std::string,Font*>::iterator iter = _font_manager.find(font_location);
  if( iter != _font_manager.end() ) {
    font = iter->second;
  } else {
    font = new Font(font_location, font_size, 0.08f);
    _font_manager.insert(std::make_pair(font_location,font));
  }
  return font;
}

std::tuple<Audio::BufferFormat,Containers::Array<char>,ALsizei>
        RessourceLoader::getAudio(std::string location) {
  if( !_audio_importer->openFile(location) )
    FatlIOError("Could not load audio file at location: "+location);
  return std::make_tuple(
          _audio_importer->format(),
          std::move(_audio_importer->data()),
          _audio_importer->frequency());
}

}
