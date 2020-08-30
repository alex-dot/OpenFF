#include "MusicMenu.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

MusicMenu::MusicMenu() : _music(nullptr) {}
MusicMenu::MusicMenu(Music* music) : MusicMenu::MusicMenu() {
  _music = music;
}

MusicMenu::MusicMenu(
        Configuration* config,
        RessourceLoader* ressource_loader,
        Vector2 relative_billboard_ratio,
        Music* music) : MusicMenu::MusicMenu(music) {
  _songtitle = new OpenFF::Textbox(
          config,
          ressource_loader,
          relative_billboard_ratio,
          Vector2i(314,24),
          Vector2i(3,200));
  setTitle(_music->getCurrentTrackName());
}

void MusicMenu::setTitle(std::string title) {
  _songtitle->write(title);
}

void MusicMenu::draw() {
  _songtitle->draw();
}

// only as long as we don't have multi-threading
void MusicMenu::setRelativeBillboardRatio(Vector2 relative_billboard_ratio) {
  _songtitle->setRelativeBillboardRatio(relative_billboard_ratio);
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
  if( _music->isPaused() ) _songtitle->show();
  else _songtitle->hide();
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
