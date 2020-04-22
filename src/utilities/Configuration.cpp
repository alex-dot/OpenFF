#include <iostream>

#include "Configuration.h"
#include "InputHandler.h"

using namespace OpenFF;

Configuration::Configuration(InputHandler* input_handler) :
        _input_handler(input_handler),
        _background_location("../../ff7/data/flevel/ancnt1.png") {

  // configure all input events
  _input_handler->addKeyToInputEvents(
          Magnum::Platform::Sdl2Application::KeyEvent::Key::Esc,
          ModifierType::unmodified,
          InputEvents::closeApp);
  _input_handler->addKeyToInputEvents(
          Magnum::Platform::Sdl2Application::KeyEvent::Key::Q,
          ModifierType::ctrl,
          InputEvents::closeApp);
  _input_handler->addKeyToInputEvents(
          Magnum::Platform::Sdl2Application::KeyEvent::Key::X,
          ModifierType::shift,
          InputEvents::closeApp);
  _input_handler->removeKeyFromInputEvents(
          Magnum::Platform::Sdl2Application::KeyEvent::Key::X,
          ModifierType::shift);
}

void Configuration::setBackgroundLocation(std::string new_loc) {
  _background_location = new_loc;
}

std::string Configuration::getBackgroundLocation() const {
  return _background_location;
}
