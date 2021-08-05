#pragma once

#include "../audio/Music.h"
#include "../ui/Textbox.h"
#include "../ui/MenuBox.h"
#include "../ui/FreeformTextbox.h"
#include "../utilities/Configuration.h"
#include "../utilities/InputHandler.h"
#include "../utilities/RessourceLoader.h"

#include <list>

using namespace Magnum;

namespace OpenFF {

struct MenuBoxElement {
  int      weight = 0;
  int      rowid;
  int      colid;
  int      row_pixels;
  int      col_pixels;
  MenuBox* ptr = nullptr;
};

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
    MusicMenu& menuUp();
    MusicMenu& menuDown();
    MusicMenu& menuLeft();
    MusicMenu& menuRight();
    MusicMenu& moveSelection(MenuDirections);

    MusicMenu& menuAcceptSelected();
    MusicMenu& menuExit();

    MusicMenu&            sortMenuboxes();
    Vector2i              findMenubox(MenuBox*);
    std::vector<Vector2i> findAllMenuboxListLocations(MenuBox*);
    MenuBox*              getMenubox(Vector2i);
    void                  getMenuboxElement(MenuBoxElement&, Vector2i);
    MenuBox*              getNextMenubox(MenuDirections);
    MenuBox*              getNextMenubox(MenuDirections, Vector2i);

    void nearestNeighbourMenubox(
            MenuBoxElement& menubox,
            MenuBoxElement& interdimensional_menubox,
            Vector2i location,
            unsigned int depth,
            MenuDirections dir);

    MusicMenu& increaseGain();
    MusicMenu& decreaseGain();
    MusicMenu& pauseResume();

    MusicMenu& drawVisualiser();
    MusicMenu& prepareVisualiser();

    void magnitudeSlicer();

    // for testing and posterity
            Configuration* _config;
            RessourceLoader* _ressource_loader;
            Vector2 _relative_billboard_ratio;


    OpenFF::Music*                                        _music;
    OpenFF::Textbox*                                      _songtitle;
    OpenFF::MenuBox*                                      _player;
    OpenFF::MenuBox*                                      _active_box;
    Vector2i                                              _active_box_location;
    std::vector<OpenFF::MenuBox*>                         _unsorted_menu_boxes;
    std::vector<std::vector<std::vector<MenuBoxElement>>> _menu_boxes;

    // Visualiser variables
    OpenFF::MusicVisualiser*                        _visualiser;
    unsigned int                                    _visualiser_loaded;
    bool                                            _visualiser_preparing;
    bool                                            _focus_big;
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
