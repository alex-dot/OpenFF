#pragma once

#include "../audio/Music.h"
#include "../ui/Textbox.h"
#include "../utilities/Configuration.h"
#include "../utilities/InputHandler.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

namespace OpenFF {

class MusicMenu{
  public:
    explicit MusicMenu(
            Configuration* config,
            RessourceLoader* ressource_loader,
            InputHandler* input_handler,
            Vector2 relative_billboard_ratio);

    void setTitle(std::string);

    void setMusic(Music*);

    void setRelativeBillboardRatio(Vector2);

    void draw();
    void bindCallbacks(InputHandler*);

  private:
    MusicMenu& increaseGain();
    MusicMenu& decreaseGain();
    MusicMenu& pauseResume();

    OpenFF::Music*        _music;
    OpenFF::Textbox*      _songtitle;
};

}
