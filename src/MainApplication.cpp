#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>

#include <functional>

#include "audio/Music.h"
#include "graphics/BackgroundBillboard.h"
#include "graphics/DebugBox.h"
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
    OpenFF::MusicMenu*            _music_menu;
    OpenFF::DebugBox*             _debug_box;

    PluginManager::Manager<Text::AbstractFont> _font_manager;
    Containers::Pointer<Text::AbstractFont> _font;
    Text::GlyphCache*                       _glyph_cache;
    Shaders::DistanceFieldVector2D _text_shader;
//    Shaders::Flat2D _text_shader;
    GL::Buffer        _vertex_buffer, _index_buffer;
    GL::Mesh          _mesh;
    GL::Texture2D*    _glyph_texture;
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
  _music = new OpenFF::Music(_config, _input);

  // Menus
  _font = _font_manager.loadAndInstantiate("StbTrueTypeFont");
  if(!_font || !_font->openFile("../ressources/CaverasReactor7Font/Reactor7.ttf", 18.0f))
      Fatal{} << "Can't open font with StbTrueTypeFont";
  std::string base_glyphs =
          "abcdefghijklmnopqrstuvwxyz"
          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
          "0123456789 ";
  std::string base_sign_glyphs = "!?#$&%()*+-/,.^~:;<>'\"";
  std::string extended_sign_glyphs =
          "¡¢£¤¥€¦§¨©ª«¬®¯°±´µ¶·¸º»¿@[]"
          "‐–—―\\_`{}|…†‡‰÷×‸※‽‾⁋⁖℅™⌘";
  std::string super_subscript_glyphs =
          "⁰¹²³⁴⁵⁶⁷⁸⁹⁺⁻⁼⁽⁾ⁿ"
          "₀₁₂₃₄₅₆₇₈₉₊₋₌₍₎ₐ"
          "ₑₒₓₔₕₖₗₘₙₚₛₜ";
  std::string roman_numeral_glyphs = "ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫ";
  std::string ligature_glyphs = "ﬀﬁﬂﬃﬄ";
  std::string arrow_glyphs = "←↑→↓↔↕↼↽↾↿⇀⇁⇂⇃⇐⇑⇒⇓⇔⇕⟵⟶";
  std::string math_glyphs =
          "%()*+-/÷×·°^~"
          "∂∆∇∑−∓∕∖∙√∝∞∫∼∽≈≠≡≢≤≥";
  std::string geometric_glyphs =
          "▀▁▂▃▄▅▆▇█▉▊▋▌▍▎▏▐▔▕"
          "▖▗▘▙▚▛▜▝▞▟■□▢▣▤▥▦▧▨▩▪▫▬▭▮▯▰▱"
          "▲△▴▵▶▷▸▹►▻▼▽▾▿◀◁◂◃◄◅"
          "◆◇◈◉◊○◌◍◎●◐◑◒◓◔◕◖◗◯"
          "◜◝◞◟◠◡◢◣◤◥◦◧◨◩◪◫◸◹◺◻◼◽◾◿"
          "⬒⬓⬔⬕⬖⬗⬘⬙⬚";
  std::string miscellaneous_glyphs = "★☆☉☥♀♂♡♥";
  std::string cjk_symbol_glyphs = "、。々「」『』〜";
  // As defined at https://decodeunicode.org/en/languages/de#de
  std::string german_auxiliary_glyphs =
          "ÄäÖöÜüßÁáÀàÂâåÃãĀāÆæÇç"  // ĂăÅ
          "ÉéÈèÊêËëĒēÍíÌìÎîÏïİ"     // ĔĕĞğĬĭ
          "ĪīıÑñÓóÒòÔôØøŌōŒœŞşÚúÙù" // ŎŏŬŭ
          "ÛûŪūŸÿ“„";
  std::string glyphs = base_glyphs+base_sign_glyphs+extended_sign_glyphs+miscellaneous_glyphs;
  _glyph_cache = new Text::GlyphCache{Vector2i{static_cast<int>(glyphs.length()*1.5)}};
  _font->fillGlyphCache(*_glyph_cache, glyphs);

//  _text_shader = Shaders::Flat2D{Shaders::Flat3D::Flag::Textured};
  _glyph_texture = &_glyph_cache->texture();
  _glyph_texture->setMinificationFilter(SamplerFilter::Nearest);
  _glyph_texture->setMagnificationFilter(SamplerFilter::Nearest);
  std::tie(_mesh, std::ignore) = Text::Renderer2D::render(
          *_font, *_glyph_cache, 1.0f,
          "Hello World!", _vertex_buffer, _index_buffer,
          GL::BufferUsage::StaticDraw, Text::Alignment::LineCenter);

/*
  _music_menu = new OpenFF::MusicMenu(
          _font.get(), _glyph_cache,
          Platform::Sdl2Application::windowSize(),
          Platform::Sdl2Application::dpiScaling(),
          Platform::Sdl2Application::framebufferSize(),
          _music);
  _music_menu->bindCallbacks(_input);
*/

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

  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::One,
          GL::Renderer::BlendFunction::OneMinusSourceAlpha);
  using namespace Math::Literals;
  _text_shader.setTransformationProjectionMatrix(Matrix3::projection(Vector2(10.0f)))
//              .setColor(0xffffffff_rgbaf)
//              .bindTexture(_glyph_cache->texture())
              .setSmoothness(0.0f)
//              .setOutlineRange(0.5f,0.5f)
              .bindVectorTexture(*_glyph_texture)
              .draw(_mesh);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::One,
          GL::Renderer::BlendFunction::One);
  GL::Renderer::disable(GL::Renderer::Feature::Blending);

//  _music_menu->draw();

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
