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
#include "graphics/Window.h"
#include "graphics/DebugBox.h"
#include "ui/Font.h"
#include "ui/MusicMenu.h"
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
    OpenFF::Window*              _window;
    OpenFF::Font*                 _font;
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

  // populate main application callbacks
  _input->setCallbacks(
          *this, OpenFF::ObjectType::main_app,
          {std::make_pair(&OpenFF_Main::exitMain,OpenFF::InputEvents::app_close)});

  // Music object
  //_music = new OpenFF::Music(_config, _input);

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

  // Font object
  _font = new OpenFF::Font(_config->getFontLocation(), _config->getFontBaseSize());
  _font->setRelativeBillboardRatio(_bb->getRelativeBillboardRatio());


  if(!png_importer->openFile(_config->getBorderLocation())) std::exit(2);
  Containers::Optional<Trade::ImageData2D> image_window = png_importer->image2D(0);
  CORRADE_INTERNAL_ASSERT(image_window);

  // Textbox
  _window = new OpenFF::Window(Vector2i(320,240));
  _window->setBorder(image_window);
  _window->setRelativeBillboardRatio(_bb->getRelativeBillboardRatio());
  _window->setBoxSize(Vector2i(158,40));
  _window->setOffset(Vector2i(154,148));
  using namespace Magnum::Math::Literals;
  //_window->setColor(0xff0000a0_rgbaf);
  _window->setColor(0x0000b0_rgbf,0x000080_rgbf,0x000020_rgbf,0x000050_rgbf);
  //_window->setBodyTransparency(0.5);
  //_window->setBorderTransparency(0.5);

  // Menus
/*
  _music_menu = new OpenFF::MusicMenu(
          _font.get(), _glyph_cache,
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
}

void OpenFF_Main::drawEvent() {
  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::SourceAlpha,
          GL::Renderer::BlendFunction::OneMinusSourceAlpha);

  // first render the background to its own framebuffer
  _bb->getFramebuffer()
          .clear(GL::FramebufferClear::Color)
          .bind();
  _bb->draw();

  // Textboxes
  _window->draw();

  // Text Shadow rendering
  _font->draw(OpenFF::FontRenderType::shadow);
  // Text rendering
  _font->draw();

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
}

void OpenFF_Main::viewportEvent(ViewportEvent& event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
  _bb->getFramebuffer().setViewport(GL::defaultFramebuffer.viewport());
  _bb->setRelativeBillboardRatio(GL::defaultFramebuffer.viewport().size());
  _font->setRelativeBillboardRatio(_bb->getRelativeBillboardRatio());
  _window->setRelativeBillboardRatio(_bb->getRelativeBillboardRatio());
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
