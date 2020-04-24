#pragma once

#include <Magnum/Platform/Sdl2Application.h>

#include <functional>
#include <map>

class OpenFF_Main;

namespace OpenFF {

typedef Magnum::Platform::Sdl2Application::KeyEvent KeyEvent;

enum InputEvents {
  none,
  close_app,
  INPUT_EVENTS_MAX = close_app
};
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

class InputHandler{
  public:
    explicit InputHandler();

    bool processKeyReleaseEvent(KeyEvent& event, OpenFF_Main* main_application);

    // callback setters
    void setMainExitCallback(std::function<void(OpenFF_Main&)> f) {_mainExit = f;};

    void addKeyToInputEvents(KeyEvent::Key, ModifierType, InputEvents event);
    void removeKeyFromInputEvents(KeyEvent::Key, ModifierType);

  private:
    // callbacks
    void doMainExit(OpenFF_Main* app) {_mainExit(*app);};

    std::map<KeyEvent::Key,OpenFF::InputEvents>*
            determineInputEventMap(ModifierType modifier);

    std::map<OpenFF::InputEvents,
             std::function<void(InputHandler&,OpenFF_Main*)>> _event_callbacks;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_unmodified;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_shift;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_ctrl;

    std::function<void(OpenFF_Main&)>  _mainExit;
};

}
