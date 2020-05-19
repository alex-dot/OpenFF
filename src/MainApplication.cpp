#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include <functional>

#include "audio/Music.h"
#include "graphics/BackgroundBillboard.h"
#include "graphics/DebugBox.h"
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
    OpenFF::Music*                _music;
    OpenFF::Configuration*        _config;
    OpenFF::InputHandler*         _input;
    OpenFF::DebugBox*             _debug_box;
};

OpenFF_Main::OpenFF_Main(const Arguments& arguments):
  Platform::Application{arguments,
                        Configuration{}
                                .setTitle("OpenFF_Main")
                                .setWindowFlags(Configuration::WindowFlag::Resizable)}
{
  _input = new OpenFF::InputHandler(this);

  // initialise configuration
  _config = new OpenFF::Configuration(_input);

  // populate main application callbacks
  _input->setMainExitCallback(&OpenFF_Main::exitMain);

  // Music object
  _music = new OpenFF::Music();
  _music->bindCallbacks(_input);

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

  _debug_box = new OpenFF::DebugBox();

  // set rendering
  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  setSwapInterval(0);
  setMinimalLoopPeriod(16);
}

void OpenFF_Main::drawEvent() {
  // first render the background to its own framebuffer
  _bb->getFramebuffer()
          .clear(GL::FramebufferClear::Color)
          .bind();
  _bb->draw();

  // then bind the default framebuffer and blit the backgrounds framebuffer to it
  GL::defaultFramebuffer
          .clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth)
          .bind();
  _bb->getFramebuffer().mapForRead(GL::Framebuffer::ColorAttachment{0});
  GL::AbstractFramebuffer::blit(
          _bb->getFramebuffer(),
          GL::defaultFramebuffer,
          Range2Di(Vector2i(0), GL::defaultFramebuffer.viewport().size()),
          GL::FramebufferBlit::Color);

  _debug_box->draw();

  swapBuffers();
}

void OpenFF_Main::viewportEvent(ViewportEvent& event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
  _bb->getFramebuffer().setViewport(GL::defaultFramebuffer.viewport());
  _bb->setRelativeBillboardRatio(GL::defaultFramebuffer.viewport().size());
}

void OpenFF_Main::exitMain() {
  delete(_music);
  exit();
  redraw();
}

void OpenFF_Main::keyPressEvent(KeyEvent& event) {
  if(true) return;
  event.setAccepted();
}
void OpenFF_Main::keyReleaseEvent(KeyEvent& event) {
  if( _input->processKeyReleaseEvent(event) )
    event.setAccepted();
}

MAGNUM_APPLICATION_MAIN(OpenFF_Main)
