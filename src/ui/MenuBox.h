#pragma once

#include "../ui/Textbox.h"
#include "../ui/FreeformTextbox.h"
#include "../utilities/Configuration.h"
#include "../utilities/RessourceLoader.h"

using namespace Magnum;

namespace OpenFF {

class MenuBox{
  public:
    explicit MenuBox(
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Vector2i textbox_size,
            Vector2i offset);

    MenuBox& selectionUp();
    MenuBox& selectionDown();
    MenuBox& selectionLeft();
    MenuBox& selectionRight();
    MenuBox& selectionAccept();
    MenuBox& selectionCancel();

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
    OpenFF::AbstractTextbox*   _textbox;
    OpenFF::Textbox*           _focus;
    Vector2i                   _textbox_size;
    Vector2i                   _offset;
    bool                       _focus_normal;
    float                      _selection_wobble_offset;
};

}
