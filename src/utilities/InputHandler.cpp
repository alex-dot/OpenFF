#include <Magnum/Platform/Sdl2Application.h>

#include <map>

#include "InputHandler.h"
#include "Configuration.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

// Add calls for each ObjectType
void CalledObject::call() {
    switch(_type) {
      case(ObjectType::ot_undefined):
        FatlSrcLogicError<std::string>("Callback on undefined object.");
        break;
      case(ObjectType::main_app):
        callMainApp();
        break;
      case(ObjectType::music):
        callMusic();
        break;
      case(ObjectType::menu_music):
        callMenuMusic();
        break;
      default:
        FatlSrcLogicError<std::string>("Callback on undefined object.");
        break;
    }
}
// Each ObjectType has its own switch-cases; thus this is where the
// pseudo state-machine logic is defined
void CalledObject::callMainApp() {
  switch(_event) {
    case(InputEvents::app_close):
      _callback_main_app(*static_cast<OpenFF_Main*>(_object));
      break;
    default:
      FatlSrcLogicError<std::string>("Callback on undefined event for object.");
      break;
  }
}
void CalledObject::callMusic() {
  switch(_event) {
    case(InputEvents::music_increase_gain):
    case(InputEvents::music_decrease_gain):
    case(InputEvents::music_pause):
      _callback_music(*static_cast<Music*>(_object));
      break;
    default:
      FatlSrcLogicError<std::string>("Callback on undefined event for object.");
  }
}
void CalledObject::callMenuMusic() {
  switch(_event) {
    case(InputEvents::music_increase_gain):
    case(InputEvents::music_decrease_gain):
    case(InputEvents::music_pause):
      _callback_menu_music(*static_cast<MusicMenu*>(_object));
      break;
    default:
      FatlSrcLogicError<std::string>("Callback on undefined event for object.");
  }
}

InputHandler::InputHandler() {
  // populate callable objects map
  // for each InputEvent define a CalledObject
  _callback_functions[InputEvents::ie_undefined] =
          CalledObject(InputEvents::ie_undefined);
  _callback_functions[InputEvents::app_close] =
          CalledObject(InputEvents::app_close);
  _callback_functions[InputEvents::music_increase_gain] =
          CalledObject(InputEvents::music_increase_gain);
  _callback_functions[InputEvents::music_decrease_gain] =
          CalledObject(InputEvents::music_decrease_gain);
  _callback_functions[InputEvents::music_pause] =
          CalledObject(InputEvents::music_pause);
  validateEnumCallbackMaps<std::map<OpenFF::InputEvents,CalledObject>>(
                                   &_callback_functions,
                                   "callable objects",
                                   input_events_iterator(),
                                   InputEvents::INPUT_EVENTS_MAX+1);
}

// For each ObjectType, define a setCallback()-function
void InputHandler::setCallbacks(
        OpenFF_Main& object, ObjectType type,
        std::initializer_list<std::pair
                <std::function<void(OpenFF_Main&)>,InputEvents>> events) {
  for(auto it = events.begin(); it != events.end(); ++it) {
    _callback_functions[it->second]._type = type;
    _callback_functions[it->second]._object = static_cast<void*>(&object);
    _callback_functions[it->second]._callback_main_app = it->first;
    _callback_functions[it->second]._callback_music = nullptr;
    _callback_functions[it->second]._callback_menu_music = nullptr;
  }
}
void InputHandler::setCallbacks(
        Music& object, ObjectType type,
        std::initializer_list<std::pair
                <std::function<void(Music&)>,InputEvents>> events) {
  for(auto it = events.begin(); it != events.end(); ++it) {
    _callback_functions[it->second]._type = type;
    _callback_functions[it->second]._object = static_cast<void*>(&object);
    _callback_functions[it->second]._callback_main_app = nullptr;
    _callback_functions[it->second]._callback_music = it->first;
    _callback_functions[it->second]._callback_menu_music = nullptr;
  }
}
void InputHandler::setCallbacks(
        MusicMenu& object, ObjectType type,
        std::initializer_list<std::pair
                <std::function<void(MusicMenu&)>,InputEvents>> events) {
  for(auto it = events.begin(); it != events.end(); ++it) {
    _callback_functions[it->second]._type = type;
    _callback_functions[it->second]._object = static_cast<void*>(&object);
    _callback_functions[it->second]._callback_main_app = nullptr;
    _callback_functions[it->second]._callback_music = nullptr;
    _callback_functions[it->second]._callback_menu_music = it->first;
  }
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
    _callback_functions[e_iter->second].call();
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
