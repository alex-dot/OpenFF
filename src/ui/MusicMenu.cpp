#include <Magnum/GL/Renderer.h>
#include <Magnum/Text/Alignment.h>
#include <Magnum/Ui/Anchor.h>
#include <Magnum/Ui/Style.h>

#include <Corrade/PluginManager/Manager.h>

#include "MusicMenu.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

MusicMenu::MusicMenu() : _music(nullptr) {}
MusicMenu::MusicMenu(
        Magnum::Vector2i window_size,
        Magnum::Vector2  dpi_scaling,
        Magnum::Vector2i framebuffer_size) :
                MusicMenu::MusicMenu()
{
  this->initialise_ui(window_size, dpi_scaling, framebuffer_size);
}
MusicMenu::MusicMenu(
        Text::AbstractFont* font,
        Text::GlyphCache* glyph_cache,
        Magnum::Vector2i window_size,
        Magnum::Vector2 dpi_scaling,
        Magnum::Vector2i framebuffer_size) :
                MusicMenu::MusicMenu()
{
  this->initialise_ui(window_size, dpi_scaling, framebuffer_size, font, glyph_cache);
}
MusicMenu::MusicMenu(
        Magnum::Vector2i window_size,
        Magnum::Vector2  dpi_scaling,
        Magnum::Vector2i framebuffer_size,
        Music* music) :
                MusicMenu::MusicMenu(
                        window_size,
                        dpi_scaling,
                        framebuffer_size) {
  _music = music;
  _label->setText(_music->getCurrentTrackName());
}
MusicMenu::MusicMenu(
        Text::AbstractFont* font,
        Text::GlyphCache* glyph_cache,
        Magnum::Vector2i window_size,
        Magnum::Vector2  dpi_scaling,
        Magnum::Vector2i framebuffer_size,
        Music* music) :
                MusicMenu::MusicMenu(
                        font,
                        glyph_cache,
                        window_size,
                        dpi_scaling,
                        framebuffer_size) {
  _music = music;
  _label->setText(_music->getCurrentTrackName());
}

void MusicMenu::initialise_ui(
        Magnum::Vector2i window_size,
        Magnum::Vector2  dpi_scaling,
        Magnum::Vector2i framebuffer_size,
        Text::AbstractFont* font,
        Text::GlyphCache* glyph_cache) {
  if( font != nullptr )
    _ui.emplace(
            Magnum::Vector2{window_size}/dpi_scaling,
            window_size,
            framebuffer_size,
            *font,
            *glyph_cache,
            Magnum::Ui::defaultStyleConfiguration());
  else
    _ui.emplace(Magnum::Vector2{window_size}/dpi_scaling, window_size, framebuffer_size);
  _plane = new Ui::Plane{*_ui,
            Ui::Snap::Top|Ui::Snap::Bottom|Ui::Snap::Left|Ui::Snap::Right,
            0, 16, 256};
  _label = new Ui::Label{*_plane,
            {Ui::Snap::Bottom|Ui::Snap::Left, Vector2(80, 32)},
            "", Text::Alignment::MiddleLeft, 80};
}

void MusicMenu::draw() {
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::One,
          GL::Renderer::BlendFunction::OneMinusSourceAlpha);
  _ui->draw();
  GL::Renderer::setBlendFunction(
          GL::Renderer::BlendFunction::One,
          GL::Renderer::BlendFunction::One);
  GL::Renderer::disable(GL::Renderer::Feature::Blending);
}

MusicMenu& MusicMenu::increaseGain() {
  _music->increaseGain();
  return *this;
}
MusicMenu& MusicMenu::decreaseGain() {
  _music->decreaseGain();
  return *this;
}
MusicMenu& MusicMenu::pauseResume() {
  _music->pauseResume();
  return *this;
}

void MusicMenu::bindCallbacks(InputHandler* input) {
  input->setCallbacks(
      *this,
      ObjectType::menu_music,
      {
          std::make_pair(&MusicMenu::increaseGain,InputEvents::music_increase_gain),
          std::make_pair(&MusicMenu::decreaseGain,InputEvents::music_decrease_gain),
          std::make_pair(&MusicMenu::pauseResume,InputEvents::music_pause)
      }
  );
}
