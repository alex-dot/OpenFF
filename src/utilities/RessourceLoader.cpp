#include "RessourceLoader.h"
#include "../misc/TerminalOutput.h"

#include <Magnum/PixelFormat.h>
#include <Magnum/Image.h>
#include <Magnum/ImageView.h>

namespace OpenFF {

RessourceLoader::RessourceLoader() {
  _img_manager = new PluginManager::Manager<Trade::AbstractImporter>;
  _png_importer = _img_manager->loadAndInstantiate("PngImporter");
  if(!(_png_importer)) FatlIOError("Could not load PngImporter Plugin");

  _img_exporter = new PluginManager::Manager<Trade::AbstractImageConverter>;
  _png_exporter = _img_exporter->loadAndInstantiate("PngImageConverter");
  if(!(_png_exporter)) FatlIOError("Could not load PngConverter Plugin");

  _audio_importer = _audio_manager.loadAndInstantiate("AnyAudioImporter");
  if(!_audio_importer) FatlIOError("Could not load AnyAudioImporter Plugin");
}

void RessourceLoader::getImage(
        std::string location,
        Containers::Optional<Trade::ImageData2D>& image,
        bool do_fatal_exit) {
  if(!_png_importer->openFile(location)) {
    if( do_fatal_exit )
      FatlIOError("Could not load image at location: "+location);
  } else {
    image = _png_importer->image2D(0);
    CORRADE_INTERNAL_ASSERT(image);
  }
}

void RessourceLoader::getImage(
        std::string location,
        Containers::Optional<Trade::ImageData2D>& image) {
  this->getImage(location, image, true);
}

void RessourceLoader::saveImage(std::string location, Image2D& image) {
//  Trade::ImageData2D image_data{PixelFormat::RGB8Unorm, {32, 32}, std::move(image)};
  if( !_png_exporter->exportToFile(image, location) )
    FatlIOError("Could not save image at location: "+location);
}

Font* RessourceLoader::getFont(std::string font_location, int font_size) {
  Font* font;
  std::map<std::string,Font*>::iterator iter = _font_manager.find(font_location);
  if( iter != _font_manager.end() ) {
    font = iter->second;
  } else {
    font = new Font(font_location, font_size, 0.081f);
    _font_manager.insert(std::make_pair(font_location,font));
  }
  return font;
}

Font* RessourceLoader::getFont(
        std::string font_location,
        int font_size,
        std::string font_texture_location) {
  Font* font;
  std::map<std::string,Font*>::iterator iter = _font_manager.find(font_location);
  if( iter != _font_manager.end() ) {
    font = iter->second;
  } else {
    Containers::Optional<Trade::ImageData2D> glyph_texture;
    getImage(font_texture_location, glyph_texture, false);
    if( glyph_texture ) {
      font = new Font(font_location, font_size, glyph_texture, 0.081f);
    } else {
      Err("Could not load cached font glyph texture, creating. ");
      font = new Font(font_location, font_size, 0.081f);
      Image2D image = font->getGlyphTexture().image(0, {PixelFormat::RGBA8Unorm});
      saveImage(font_texture_location, image);
    }
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
