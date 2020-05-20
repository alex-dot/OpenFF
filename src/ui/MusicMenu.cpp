#include <Magnum/GL/Renderer.h>

#include "MusicMenu.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

MusicMenu::MusicMenu() {}
MusicMenu::MusicMenu(
        Magnum::Vector2i window_size,
        Magnum::Vector2  dpi_scaling,
        Magnum::Vector2i framebuffer_size) :
                MusicMenu::MusicMenu()
{
  _ui.emplace(Magnum::Vector2{window_size}/dpi_scaling, window_size, framebuffer_size);
  _plane = new Ui::Plane{*_ui,
            Ui::Snap::Top|Ui::Snap::Bottom|Ui::Snap::Left|Ui::Snap::Right,
            0, 16, 128};
  _label = new Ui::Label{*_plane,
            {Ui::Snap::Bottom|Ui::Snap::Left, Vector2(80, 32)},
            std::string(100,' '), Text::Alignment::MiddleLeft};
  _label->setText("");
}
//MusicMenu::MusicMenu(InputHandler* input) : MusicMenu::MusicMenu() {
//  this->bindCallbacks(input);
//}

void MusicMenu::draw() {
  _label->setText("bla");
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
/*
void MusicMenu::bindCallbacks(InputHandler* input) {
  input->setMusicIncreaseGainCallback(&Music::increaseGain);
  input->setMusicDecreaseGainCallback(&Music::decreaseGain);
  input->setMusicPauseCallback(&Music::pauseResume);
  input->setCallableObjects(
          static_cast<void*>(this),
          { InputEvents::music_increase_gain,
            InputEvents::music_decrease_gain,
            InputEvents::music_pause}
          );
}*/
