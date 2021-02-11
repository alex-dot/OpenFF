#pragma once

#include "../ui/Textbox.h"
#include "../ui/FreeformTextbox.h"
#include "../utilities/Configuration.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

namespace OpenFF {

enum MenuSelectionReturns {
  success,
  end_of_line,
  error,
  MENU_SELECTION_RETURNS_MAX = error
};
enum MenuDirections {
  up,
  down,
  left,
  right,
  none,
  MENU_DIRECTIONS_MAX = none
};
enum MenuBoxType {
  regular,
  freeform,
  MENU_BOX_TYPES_MAX = freeform
};

class MenuBox{
  public:
    explicit MenuBox(
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Vector2i textbox_size,
            Vector2i offset);
    explicit MenuBox(
            MenuBoxType type,
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Vector2i textbox_size,
            Vector2i offset);

    MenuBox& enableSelection();
    MenuBox& disableSelection();

    MenuSelectionReturns selectionUp();
    MenuSelectionReturns selectionDown();
    MenuSelectionReturns selectionLeft();
    MenuSelectionReturns selectionRight();
    MenuBox& selectionAccept();
    MenuBox& selectionCancel();

    MenuBox& setSelectionTop();
    MenuBox& setSelectionBottom();
    MenuBox& setSelectionLeftmost();
    MenuBox& setSelectionRightmost();

    MenuBox& setLinkedBox(MenuDirections, OpenFF::MenuBox*);
    MenuBox& setLinkedBoxUp(OpenFF::MenuBox*);
    MenuBox& setLinkedBoxDown(OpenFF::MenuBox*);
    MenuBox& setLinkedBoxLeft(OpenFF::MenuBox*);
    MenuBox& setLinkedBoxRight(OpenFF::MenuBox*);

    MenuBox* getLinkedBox(MenuDirections);
    MenuBox* getLinkedBoxUp();
    MenuBox* getLinkedBoxDown();
    MenuBox* getLinkedBoxLeft();
    MenuBox* getLinkedBoxRight();

    MenuBox& updateSelectionWobble(float);
    MenuBox& setSelectionWobbleOffset(float);

    MenuBox& write();
    MenuBox& write(std::string);

    MenuBox& show();
    MenuBox& hide();

    // customize Textbox
    // customize font and text
    MenuBox& setFont(Font*);
    MenuBox& setFont(std::string, int);
    MenuBox& setTextShadowType(OpenFF::ShadowTypes);
    // customize window
    MenuBox& setTextboxBorderImageLocation(std::string);
    MenuBox& setFocusBorderImageLocation(std::string);
    // set uniform color
    MenuBox& setTextboxColor(Color3);
    MenuBox& setTextboxColor(Color4);
    MenuBox& setFocusColor(Color3);
    MenuBox& setFocusColor(Color4);
    // set color gradient, starting top-left going clockwise
    MenuBox& setTextboxColor(Color3, Color3, Color3, Color3);
    MenuBox& setTextboxColor(Color3, Color3, Color3, Color3, float);
    MenuBox& setFocusColor(Color3, Color3, Color3, Color3);
    MenuBox& setFocusColor(Color3, Color3, Color3, Color3, float);
    // set transparency
    MenuBox& setTextboxBodyTransparency(float);
    MenuBox& setFocusBodyTransparency(float);
    MenuBox& setTextboxBorderTransparency(float);
    MenuBox& setFocusBorderTransparency(float);
    // set generic textbox variables
    MenuBox& enableInstantRendering();
    MenuBox& disableInstantRendering();
    MenuBox& setRelativeBillboardRatio(Vector2);
    MenuBox& setViewportSize(Vector2i);

    MenuBox& setTextboxSize(Vector2i);
    MenuBox& setFocusSize(unsigned int);
    MenuBox& expandTextbox(int);
    MenuBox& expandFocus(int);
    MenuBox& shrinkTextbox(int);
    MenuBox& shrinkFocus(int);

    Vector2i getOffset();
    Vector2i getTextboxSize();

    MenuBox& rewriteCharacter(
            unsigned int line_index,
            unsigned int character_index,
            Vector2i offset_mod,
            std::string character);
    MenuBox& moveCharacter(
            unsigned int line_index,
            unsigned int character_index,
            Vector2i offset_mod);
    MenuBox& moveText(Vector2i);

    void draw();
    void draw(float current_time);

  private:
    MenuBoxType                _type;
    OpenFF::AbstractTextbox*   _textbox;
    OpenFF::Textbox*           _focus;
    Vector2i                   _textbox_size;
    Vector2i                   _offset;
    bool                       _focus_normal;
    bool                       _selection_active;
    float                      _selection_wobble_offset;
    unsigned int               _selection_current_position_horizontally;
    unsigned int               _selection_current_position_vertically;
    std::map<MenuDirections,OpenFF::MenuBox*>  _linked_boxes;
};

}
