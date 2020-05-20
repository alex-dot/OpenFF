#pragma once

#include <Corrade/Containers/Optional.h>
#include <Magnum/Text/Alignment.h>
#include <Magnum/Ui/Anchor.h>
#include <Magnum/Ui/Label.h>
#include <Magnum/Ui/Plane.h>
#include <Magnum/Ui/UserInterface.h>

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
//    explicit MusicMenu(InputHandler*);

    void draw();
//    void bindCallbacks(InputHandler*);

  private:
    Containers::Optional<Ui::UserInterface> _ui;
    Ui::Plane*                              _plane;
    Ui::Label*                              _label;
};

}
