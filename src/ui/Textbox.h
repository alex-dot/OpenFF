#pragma once

#include "../ui/AbstractTextbox.h"

namespace OpenFF {

class Textbox: public AbstractTextbox{
  public:
    using AbstractTextbox::AbstractTextbox;
    Textbox(
            Configuration* config,
            RessourceLoader* ressource_loader,
            Vector2 relative_billboard_ratio,
            Vector2i textbox_size,
            Vector2i offset);

    Textbox& write(std::string);
    Textbox& write();

  protected:
    void prepareText();

};

}
