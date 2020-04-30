#include <Magnum/Platform/Sdl2Application.h>

#include <map>

#include "InputHandler.h"
#include "Configuration.h"

using namespace OpenFF;

InputHandler::InputHandler() {
  // populate event callback map
  _event_callbacks[InputEvents::app_close] = &InputHandler::doMainExit;
  validateEnumCallbackMaps<ec_map>(&_event_callbacks,
                                   "event callback",
                                   input_events_iterator(),
                                   InputEvents::INPUT_EVENTS_MAX+1);
}

std::map<KeyEvent::Key,OpenFF::InputEvents>*
        InputHandler::determineInputEventMap(ModifierType modifier) {
  if( modifier == ModifierType::ctrl )
    return &_input_events_ctrl;
  else if ( modifier == ModifierType::shift )
    return &_input_events_shift;
  else
    return &_input_events_unmodified;
}

bool InputHandler::processKeyReleaseEvent(
                    KeyEvent& event,
                    void* object) {
  using namespace Magnum::Platform;

  ModifierType modifier;
  if( event.modifiers() & Sdl2Application::InputEvent::Modifier::Ctrl )
    modifier = ModifierType::ctrl;
  else if ( event.modifiers() & Sdl2Application::InputEvent::Modifier::Shift )
    modifier = ModifierType::shift;
  else
    modifier = ModifierType::unmodified;

  const std::map<KeyEvent::Key,OpenFF::InputEvents>*
          event_map = determineInputEventMap(modifier);

  auto e_iter = event_map->find(event.key());
  if( e_iter != event_map->end() ) {
    auto callback = _event_callbacks[e_iter->second];
    callback(*this,object);
    return true;
  }

  // Return False if the pressed key was not configured (might be accidental press)
  return false;
}

void InputHandler::addKeyToInputEvents(
        KeyEvent::Key key,
        ModifierType modifier,
        InputEvents event) {
  std::map<KeyEvent::Key,OpenFF::InputEvents>*
          event_map = determineInputEventMap(modifier);
  event_map->insert(std::pair<KeyEvent::Key,OpenFF::InputEvents>(key, event));
}
void InputHandler::removeKeyFromInputEvents(
        KeyEvent::Key key,
        ModifierType modifier) {
  std::map<KeyEvent::Key,OpenFF::InputEvents>*
          event_map = determineInputEventMap(modifier);
  event_map->erase(key);
}
