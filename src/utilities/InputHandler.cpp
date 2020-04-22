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

bool InputHandler::processKeyReleaseEvent(
                    Magnum::Platform::Sdl2Application::KeyEvent& event,
                    OpenFF_Main* main_application) {
  using namespace Magnum::Platform;

  const std::map<Sdl2Application::KeyEvent::Key,OpenFF::InputEvents>* event_map;

  if( event.modifiers() & Sdl2Application::InputEvent::Modifier::Ctrl )
    event_map = &_input_events_ctrl;
  else if ( event.modifiers() & Sdl2Application::InputEvent::Modifier::Shift )
    event_map = &_input_events_shift;
  else
    event_map = &_input_events_unmodified;

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

  std::map<Sdl2Application::KeyEvent::Key,OpenFF::InputEvents>* event_map;

  if( modifier == ModifierType::ctrl )
    event_map = &_input_events_ctrl;
  else if ( modifier == ModifierType::shift )
    event_map = &_input_events_shift;
  else if ( modifier == ModifierType::unmodified )
    event_map = &_input_events_unmodified;

  event_map->insert(std::pair<Sdl2Application::KeyEvent::Key,
                              OpenFF::InputEvents>
                              (key, event));
}
void InputHandler::removeKeyFromInputEvents(
        Magnum::Platform::Sdl2Application::KeyEvent::Key key,
        ModifierType modifier) {
  using namespace Magnum::Platform;

  std::map<Sdl2Application::KeyEvent::Key,OpenFF::InputEvents>* event_map;

  if( modifier == ModifierType::ctrl )
    event_map = &_input_events_ctrl;
  else if ( modifier == ModifierType::shift )
    event_map = &_input_events_shift;
  else if ( modifier == ModifierType::unmodified )
    event_map = &_input_events_unmodified;

  event_map->erase(key);
}
