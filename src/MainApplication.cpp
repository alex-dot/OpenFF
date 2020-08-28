#include <Magnum/Timeline.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/ImageData.h>

#include <functional>

#include "audio/Music.h"
#include "graphics/BackgroundBillboard.h"
#include "graphics/DebugBox.h"
//#include "ui/MusicMenu.h"
#include "ui/Textbox.h"
#include "utilities/Configuration.h"
#include "utilities/InputHandler.h"
#include "utilities/RessourceLoader.h"

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

    Timeline*                     _timeline;

    OpenFF::BackgroundBillboard*  _bb;
    OpenFF::Music*                _music;
    OpenFF::Configuration*        _config;
    OpenFF::RessourceLoader*      _ressource_loader;
    OpenFF::InputHandler*         _input;
    OpenFF::Textbox*              _textbox;
    OpenFF::MusicMenu*            _music_menu;
    OpenFF::DebugBox*             _debug_box;
};

OpenFF_Main::OpenFF_Main(const Arguments& arguments):
  Platform::Application{arguments,
                        Configuration{}
                                .setTitle("OpenFF_Main")
                                .setWindowFlags(Configuration::WindowFlag::Resizable)}
{
  _input = new OpenFF::InputHandler();

  // initialise configuration
  _config = new OpenFF::Configuration(_input);
  _ressource_loader = new OpenFF::RessourceLoader();

  // populate main application callbacks
  _input->setCallbacks(
          *this, OpenFF::ObjectType::main_app,
          {std::make_pair(&OpenFF_Main::exitMain,OpenFF::InputEvents::app_close)});

  // Music object
  //_music = new OpenFF::Music(_config, _input);

  Containers::Optional<Trade::ImageData2D> image;

  // initialise background billboard
  _ressource_loader->getImage(_config->getBackgroundLocation(), image);
  _bb = new OpenFF::BackgroundBillboard();
  _bb->setBackground(image);
  _bb->setRelativeBillboardRatio(Platform::Sdl2Application::windowSize());

  // Textbox
  _textbox = new OpenFF::Textbox(
          _config,
          _ressource_loader,
          _bb->getRelativeBillboardRatio(),
          Vector2i(158,40),
          Vector2i(154,148));
  _textbox->write("Cloud:\n“Aeris?“");

  // Menus
/*
  _music_menu = new OpenFF::MusicMenu(
          _text.get(), _glyph_cache,
          Platform::Sdl2Application::windowSize(),
          Platform::Sdl2Application::dpiScaling(),
          Platform::Sdl2Application::framebufferSize(),
          _music);
  _music_menu->bindCallbacks(_input);
*/

  _debug_box = new OpenFF::DebugBox();

  // set rendering
  setSwapInterval(0);
  setMinimalLoopPeriod(16);
  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::SourceAlpha,
          GL::Renderer::BlendFunction::OneMinusSourceAlpha);

  _timeline = new Timeline();
  _timeline->start();
}

void OpenFF_Main::drawEvent() {
  // first render the background to its own framebuffer
  _bb->getFramebuffer()
          .clear(GL::FramebufferClear::Color)
          .bind();
  _bb->draw();

  // Textbox
  int offset = int(floor(_timeline->previousFrameTime()*5));
  _textbox->setOffset(Vector2i(offset));
  _textbox->draw();

//  _music_menu->draw();

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
  redraw();
  _timeline->nextFrame();
}

void OpenFF_Main::viewportEvent(ViewportEvent& event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
  _bb->getFramebuffer().setViewport(GL::defaultFramebuffer.viewport());
  _bb->setRelativeBillboardRatio(GL::defaultFramebuffer.viewport().size());
  _textbox->setRelativeBillboardRatio(_bb->getRelativeBillboardRatio());
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
  if( _input->processKeyReleaseEvent(event) )
    event.setAccepted();
}

MAGNUM_APPLICATION_MAIN(OpenFF_Main)
