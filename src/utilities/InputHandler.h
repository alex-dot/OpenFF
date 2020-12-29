#pragma once

#include <Magnum/Platform/Sdl2Application.h>

#include <functional>
#include <map>
#include <initializer_list>

#include "../misc/EnumIterator.h"

class OpenFF_Main;

namespace OpenFF {

class Music;
class MusicMenu;

typedef Magnum::Platform::Sdl2Application::KeyEvent KeyEvent;

enum InputEvents {
  ie_undefined,
  app_close,
  music_increase_gain,
  music_decrease_gain,
  music_pause,
  menu_up,
  menu_down,
  menu_left,
  menu_right,
  menu_accept,
  menu_cancel,
  INPUT_EVENTS_MAX = menu_cancel
};
typedef Iterator<InputEvents, InputEvents::ie_undefined, InputEvents::INPUT_EVENTS_MAX> input_events_iterator;
enum InputType {
  key_press,
  key_release,
  INPUT_TYPE_MAX = key_release
};
enum ModifierType {
  unmodified,
  shift,
  ctrl,
  MODIFIER_TYPE_MAX = ctrl
};
enum ObjectType {
  ot_undefined,
  main_app,
  music,
  menu_music,
  OBJECT_TYPE_MAX = menu_music
};
struct CalledObject {
  explicit CalledObject() {};
  explicit CalledObject(InputEvents event) {
    _event = event;
  }
  explicit CalledObject(InputEvents event, ObjectType type, void* object) {
    _event = event;
    _type = type;
    _object = object;
  }
  void call();
  // For each ObjectType, define a call()-function
  void callMainApp();
  void callMusic();
  void callMenuMusic();

  InputEvents                       _event          = InputEvents::ie_undefined;
  ObjectType                        _type            = ObjectType::ot_undefined;
  void*                             _object                           = nullptr;
  // For each ObjectType, define a _callback member std::function
  std::function<void(OpenFF_Main&)> _callback_main_app                = nullptr;
  std::function<void(Music&)>       _callback_music                   = nullptr;
  std::function<void(MusicMenu&)>   _callback_menu_music              = nullptr;
};

class InputHandler{
  public:
    explicit InputHandler();

    bool processKeyReleaseEvent(KeyEvent& event);

    // For each ObjectType, define a setCallback()-function
    void setCallbacks(
            OpenFF_Main&, ObjectType,
            std::initializer_list<std::pair
                    <std::function<void(OpenFF_Main&)>,InputEvents>> events);
    void setCallbacks(
            Music&, ObjectType,
            std::initializer_list<std::pair
                    <std::function<void(Music&)>,InputEvents>> events);
    void setCallbacks(
            MusicMenu&, ObjectType,
            std::initializer_list<std::pair
                    <std::function<void(MusicMenu&)>,InputEvents>> events);

    void addKeyToInputEvents(KeyEvent::Key, ModifierType, InputEvents event);
    void removeKeyFromInputEvents(KeyEvent::Key, ModifierType);

  private:
    std::map<KeyEvent::Key,OpenFF::InputEvents>*
            determineInputEventMap(ModifierType modifier);

    std::map<OpenFF::InputEvents,CalledObject>  _callback_functions;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_unmodified;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_shift;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_ctrl;
};

}
