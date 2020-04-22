#pragma once

#include <iostream>

#include "InputHandler.h"

class OpenFF_Main;

namespace OpenFF {

class Configuration{
  public:
    explicit Configuration(InputHandler*);

    void setBackgroundLocation(std::string);

    std::string getBackgroundLocation() const;

  private:
    InputHandler* _input_handler;

    std::string   _background_location;
};

}
