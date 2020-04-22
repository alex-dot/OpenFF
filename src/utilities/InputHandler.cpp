#include <Magnum/Platform/Sdl2Application.h>
#include <Corrade/Utility/Debug.h>

#include <map>

#include "InputHandler.h"
#include "Configuration.h"

using namespace OpenFF;

InputHandler::InputHandler() {
  // populate event callback map
  _event_callbacks[InputEvents::closeApp] = &InputHandler::doMainExit;
}

std::map<Magnum::Platform::Sdl2Application::KeyEvent::Key,OpenFF::InputEvents>*
        InputHandler::determineInputEventMap(ModifierType modifier) {
  using namespace Magnum::Platform;

  if( modifier == ModifierType::ctrl )
    return &_input_events_ctrl;
  else if ( modifier == ModifierType::shift )
    return &_input_events_shift;
  else
    return &_input_events_unmodified;
}

bool InputHandler::processKeyReleaseEvent(
                    Magnum::Platform::Sdl2Application::KeyEvent& event,
                    OpenFF_Main* main_application) {
  using namespace Magnum::Platform;

  ModifierType modifier;
  if( event.modifiers() & Sdl2Application::InputEvent::Modifier::Ctrl )
    modifier = ModifierType::ctrl;
  else if ( event.modifiers() & Sdl2Application::InputEvent::Modifier::Shift )
    modifier = ModifierType::shift;
  else
    modifier = ModifierType::unmodified;

  const std::map<Sdl2Application::KeyEvent::Key,OpenFF::InputEvents>*
          event_map = determineInputEventMap(modifier);

  auto e_iter = event_map->find(event.key());
  if( e_iter != event_map->end() ) {
    auto callback = _event_callbacks[e_iter->second];
    callback(*this,main_application);
    return true;
  }

  // Return False if the pressed key was not configured (might be accidental press)
  return false;
}

void InputHandler::addKeyToInputEvents(
        Magnum::Platform::Sdl2Application::KeyEvent::Key key,
        ModifierType modifier,
        InputEvents event) {
  using namespace Magnum::Platform;

  std::map<Sdl2Application::KeyEvent::Key,OpenFF::InputEvents>*
          event_map = determineInputEventMap(modifier);

  event_map->insert(std::pair<Sdl2Application::KeyEvent::Key,
                              OpenFF::InputEvents>
                              (key, event));
}
void InputHandler::removeKeyFromInputEvents(
        Magnum::Platform::Sdl2Application::KeyEvent::Key key,
        ModifierType modifier) {
  using namespace Magnum::Platform;

  std::map<Sdl2Application::KeyEvent::Key,OpenFF::InputEvents>*
          event_map = determineInputEventMap(modifier);

  event_map->erase(key);
}
