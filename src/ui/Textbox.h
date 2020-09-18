#pragma once

#include "../ui/AbstractTextbox.h"

namespace OpenFF {

class Textbox: public AbstractTextbox{
  public:
    using AbstractTextbox::AbstractTextbox;

    Textbox& write(std::string);

  protected:
    void prepareText();

};

}
