#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Debug.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include "graphics/BackgroundBillboard.h"

using namespace Magnum;

class OpenFF_Main: public Platform::Application {
  public:
    explicit OpenFF_Main(const Arguments& arguments);

  private:
    void drawEvent() override;
    void viewportEvent(ViewportEvent& event) override;

    OpenFF::BackgroundBillboard*  _bb;
};

OpenFF_Main::OpenFF_Main(const Arguments& arguments):
  Platform::Application{arguments, Configuration{}
                                    .setTitle("OpenFF_Main")
                                    .setWindowFlags(Configuration::WindowFlag::Resizable)}
{
  PluginManager::Manager<Trade::AbstractImporter> manager;
  Containers::Pointer<Trade::AbstractImporter> png_importer =
    manager.loadAndInstantiate("PngImporter");
  if(!png_importer) std::exit(1);

  if(!png_importer->openFile("../../ff7/data/flevel/ancnt1.png")) std::exit(2);
  Containers::Optional<Trade::ImageData2D> image = png_importer->image2D(0);
  CORRADE_INTERNAL_ASSERT(image);

  _bb = new OpenFF::BackgroundBillboard();
  _bb->setBackground(image);
  _bb->setRelativeBillboardRatio(Platform::Sdl2Application::windowSize());
}

void OpenFF_Main::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  _bb->draw();

  swapBuffers();
}

void OpenFF_Main::viewportEvent(ViewportEvent& event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
  _bb->setRelativeBillboardRatio(Platform::Sdl2Application::windowSize());
}

MAGNUM_APPLICATION_MAIN(OpenFF_Main)
