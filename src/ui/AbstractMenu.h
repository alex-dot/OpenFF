#pragma once

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

class AbstractMenu{
  public:
    explicit AbstractMenu();
    virtual ~AbstractMenu();

    virtual void setRelativeBillboardRatio(Vector2) = 0;

    virtual void draw() = 0;
    virtual void bindCallbacks(InputHandler*) = 0;

  protected:
    AbstractMenu& menuUp();
    AbstractMenu& menuDown();
    AbstractMenu& menuLeft();
    AbstractMenu& menuRight();
    AbstractMenu& moveSelection(MenuDirections);

    AbstractMenu& menuAcceptSelected();
    AbstractMenu& menuExit();

    void bindCallbacksAbstract(InputHandler*);

    AbstractMenu&         sortMenuboxes();
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

    OpenFF::MenuBox*                                      _active_box;
    Vector2i                                              _active_box_location;
    std::vector<OpenFF::MenuBox*>                         _unsorted_menu_boxes;
    std::vector<std::vector<std::vector<MenuBoxElement>>> _menu_boxes;
};

}
