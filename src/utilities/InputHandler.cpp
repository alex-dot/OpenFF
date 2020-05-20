#include <Magnum/Platform/Sdl2Application.h>

#include <map>

#include "InputHandler.h"
#include "Configuration.h"

using namespace OpenFF;

InputHandler::InputHandler() {
  // populate event callback map
  _event_callbacks[InputEvents::app_close] = &InputHandler::doMainExit;
  _event_callbacks[InputEvents::music_increase_gain] = &InputHandler::doMusicIncreaseGain;
  _event_callbacks[InputEvents::music_decrease_gain] = &InputHandler::doMusicDecreaseGain;
  _event_callbacks[InputEvents::music_pause] = &InputHandler::doMusicPause;
  validateEnumCallbackMaps<ec_map>(&_event_callbacks,
                                   "event callback",
                                   input_events_iterator(),
                                   InputEvents::INPUT_EVENTS_MAX+1);
  // populate callable objects map
  _callable_objects[InputEvents::app_close] = nullptr;
  _callable_objects[InputEvents::music_increase_gain] = nullptr;
  _callable_objects[InputEvents::music_decrease_gain] = nullptr;
  _callable_objects[InputEvents::music_pause] = nullptr;
  validateEnumCallbackMaps<std::map<OpenFF::InputEvents,void*>>(
                                   &_callable_objects,
                                   "callable objects",
                                   input_events_iterator(),
                                   InputEvents::INPUT_EVENTS_MAX+1);
}
InputHandler::InputHandler(OpenFF_Main* main_application)
        : InputHandler::InputHandler() {
  _callable_objects[InputEvents::app_close] = static_cast<void*>(main_application);
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

bool InputHandler::processKeyReleaseEvent(KeyEvent& event) {
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
    void* object = _callable_objects[e_iter->second];
    if( object == nullptr )
      Dbg{} << "DEBUG: Object not defined, not calling Event Nr" << e_iter->second;
    else
      callback(*this,object);
    return true;
  }

  // Return False if the pressed key was not configured (might be accidental press)
  return false;
}

void InputHandler::setCallableObjects(
        void* object,
        std::initializer_list<InputEvents> events) {
  for(auto it = events.begin(); it != events.end(); ++it)
    _callable_objects[*it] = object;
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
