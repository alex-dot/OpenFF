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

    MusicMenu& drawVisualiser();
    MusicMenu& prepareVisualiser();

    OpenFF::Music*                                  _music;
    OpenFF::Textbox*                                _songtitle;
    OpenFF::MusicVisualiser*                        _visualiser;
    bool                                            _visualiser_loaded;
    Timeline                                        _timeline;
    float                                           _current_time;
    unsigned int                                    _visualiser_bar_count;
    unsigned int                                    _maximum_frame_count;
    float                                           _frame_window_size;
    std::vector<std::vector<float>>                 _magnitude_bin_matrix_left;
    std::vector<std::vector<float>>                 _magnitude_bin_matrix_right;
    unsigned int                                    _frame_slice_left;
    unsigned int                                    _frame_slice_right;
    unsigned int                                    _frame_slice_window;
    std::future<std::vector<std::vector<float>>>    _magnitude_bin_matrix_left_future;
    std::future<std::vector<std::vector<float>>>    _magnitude_bin_matrix_right_future;
    bool                                            _magnitude_bin_matrix_left_fully_loaded;
    bool                                            _magnitude_bin_matrix_right_fully_loaded;
};

}
