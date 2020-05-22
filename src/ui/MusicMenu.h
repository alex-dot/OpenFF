#pragma once

#include <Corrade/Containers/Optional.h>
#include <Magnum/Ui/Label.h>
#include <Magnum/Ui/Plane.h>
#include <Magnum/Ui/UserInterface.h>

#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>

#include "../utilities/InputHandler.h"

using namespace Magnum;

namespace OpenFF {

class MusicMenu{
  public:
    explicit MusicMenu();
    explicit MusicMenu(
            Magnum::Vector2i window_size,
            Magnum::Vector2 dpi_scaling,
            Magnum::Vector2i framebuffer_size);
    explicit MusicMenu(
            Text::AbstractFont* font,
            Text::GlyphCache* glyph_cache,
            Magnum::Vector2i window_size,
            Magnum::Vector2 dpi_scaling,
            Magnum::Vector2i framebuffer_size);
//    explicit MusicMenu(InputHandler*);

    void draw();
//    void bindCallbacks(InputHandler*);

  private:
    void initialise_ui(
            Magnum::Vector2i window_size,
            Magnum::Vector2  dpi_scaling,
            Magnum::Vector2i framebuffer_size,
            Text::AbstractFont* font = nullptr,
            Text::GlyphCache* glyph_cache = nullptr);

    Containers::Optional<Ui::UserInterface> _ui;
    Ui::Plane*                              _plane;
    Ui::Label*                              _label;
};

}