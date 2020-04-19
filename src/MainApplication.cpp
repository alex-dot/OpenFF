#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include "graphics/BackgroundBillboard.h"

using namespace Magnum;

class OpenFF: public Platform::Application {
  public:
    explicit OpenFF(const Arguments& arguments);

  private:
    void drawEvent() override;

    BackgroundBillboard*  _bb;
};

OpenFF::OpenFF(const Arguments& arguments):
  Platform::Application{arguments, Configuration{}
                                    .setTitle("OpenFF")}
{
  PluginManager::Manager<Trade::AbstractImporter> manager;
  Containers::Pointer<Trade::AbstractImporter> png_importer =
    manager.loadAndInstantiate("PngImporter");
  if(!png_importer) std::exit(1);

  if(!png_importer->openFile("../../ff7/data/flevel/ancnt1.png")) std::exit(2);
  Containers::Optional<Trade::ImageData2D> image = png_importer->image2D(0);
  CORRADE_INTERNAL_ASSERT(image);

  _bb = new BackgroundBillboard();
  _bb->setWindowSize(Platform::Sdl2Application::windowSize());
  _bb->setBackground(image);
}

void OpenFF::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  _bb->draw();

  swapBuffers();
}

MAGNUM_APPLICATION_MAIN(OpenFF)
