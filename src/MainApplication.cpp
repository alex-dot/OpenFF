#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Debug.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include <functional>

#include "graphics/BackgroundBillboard.h"
#include "utilities/Configuration.h"
#include "utilities/InputHandler.h"

using namespace Magnum;

class OpenFF_Main: public Platform::Application {
  public:
    explicit OpenFF_Main(const Arguments& arguments);

  private:
    void drawEvent() override;
    void viewportEvent(ViewportEvent& event) override;

    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;

    void exitMain();

    OpenFF::BackgroundBillboard*  _bb;
    OpenFF::Configuration*        _config;
    OpenFF::InputHandler*         _input;
};

OpenFF_Main::OpenFF_Main(const Arguments& arguments):
  Platform::Application{arguments, Configuration{}
                                    .setTitle("OpenFF_Main")
                                    .setWindowFlags(Configuration::WindowFlag::Resizable)}
{
  _input = new OpenFF::InputHandler();

  // initialise configuration
  _config = new OpenFF::Configuration(_input);

  // populate main appilcation callbacks
  _input->setMainExitCallback(&OpenFF_Main::exitMain);

  PluginManager::Manager<Trade::AbstractImporter> manager;
  Containers::Pointer<Trade::AbstractImporter> png_importer =
    manager.loadAndInstantiate("PngImporter");
  if(!png_importer) std::exit(1);

  if(!png_importer->openFile(_config->getBackgroundLocation())) std::exit(2);
  Containers::Optional<Trade::ImageData2D> image = png_importer->image2D(0);
  CORRADE_INTERNAL_ASSERT(image);

  // initialise background billboard
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

void OpenFF_Main::exitMain() {
  exit();
  redraw();
}

void OpenFF_Main::keyPressEvent(KeyEvent& event) {
  if(true) return;
  event.setAccepted();
}
void OpenFF_Main::keyReleaseEvent(KeyEvent& event) {
  if( _input->processKeyReleaseEvent(event,this) )
    event.setAccepted();
}

MAGNUM_APPLICATION_MAIN(OpenFF_Main)
