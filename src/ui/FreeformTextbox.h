#pragma once

#include "../ui/AbstractTextbox.h"

#include <utility>

namespace OpenFF {

struct FreeformTextboxCharacter {
  Text*     text_obj;
  Vector2i  offset;
  Vector2i  offset_mod;
};

class FreeformTextbox: public AbstractTextbox{
  public:
    using AbstractTextbox::AbstractTextbox;
    FreeformTextbox(
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Vector2i textbox_size,
            Vector2i offset);

    FreeformTextbox& write(std::string);

    FreeformTextbox& rewriteCharacter(
            unsigned int line_index,
            unsigned int character_index,
            Vector2i offset_mod,
            std::string character);
    FreeformTextbox& moveCharacter(
            unsigned int line_index,
            unsigned int character_index,
            Vector2i offset_mod);

    FreeformTextbox& moveText(Vector2i);

    FreeformTextbox& draw();

    FreeformTextbox& setBorderOffset(Vector2i);

  protected:
    virtual void prepareText();

  private:
    unsigned int calculateCharacterWidth(Text*, std::string);
    Vector2i     calculateCharacterOffset(
            unsigned int line_index,
            unsigned int character_index,
            unsigned int width);
    Vector2i     calculateCharacterOffset(
            Text*,
            std::string,
            unsigned int line_index,
            unsigned int character_index);

    std::vector<std::vector<FreeformTextboxCharacter>> _textmap;
    Vector2i                                           _border_offset;
    unsigned int                                       _max_char_width;
    unsigned int                                       _line_height;
};

}
