#pragma once

#include "../audio/Music.h"
#include "../ui/Textbox.h"
#include "../utilities/InputHandler.h"

// only as long as we don't have multi-threading
#include "../utilities/Configuration.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

namespace OpenFF {

class MusicMenu{
  public:
    explicit MusicMenu();
    explicit MusicMenu(OpenFF::Music*);
    // only as long as we don't have multi-threading
    explicit MusicMenu(
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Music* music);

    void setTitle(std::string);

    void draw();
    void bindCallbacks(InputHandler*);

    // only as long as we don't have multi-threading
    void setRelativeBillboardRatio(Vector2);

  private:
    MusicMenu& increaseGain();
    MusicMenu& decreaseGain();
    MusicMenu& pauseResume();

    OpenFF::Music*        _music;
    OpenFF::Textbox*      _songtitle;
};

}
