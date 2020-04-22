#pragma once

#include <Magnum/Platform/Sdl2Application.h>

#include <functional>
#include <map>

class OpenFF_Main;

namespace OpenFF {

enum InputEvents {
  none,
  closeApp
};
enum InputType {
  keyPress,
  keyRelease
};
enum ModifierType {
  unmodified,
  shift,
  ctrl
};

class InputHandler{
  public:
    explicit InputHandler();

    bool processKeyReleaseEvent(
            Magnum::Platform::Sdl2Application::KeyEvent& event,
            OpenFF_Main* main_application);

    // callback setters
    void setMainExitCallback(std::function<void(OpenFF_Main&)> f) {_mainExit = f;};

    void addKeyToInputEvents(
            Magnum::Platform::Sdl2Application::KeyEvent::Key,
            ModifierType,
            InputEvents event);
    void removeKeyFromInputEvents(
            Magnum::Platform::Sdl2Application::KeyEvent::Key,
            ModifierType);

  private:
    // callbacks
    void doMainExit(OpenFF_Main* app) {_mainExit(*app);};

    std::map<OpenFF::InputEvents,
             std::function<void(InputHandler&,OpenFF_Main*)>> _event_callbacks;
    std::map<Magnum::Platform::Sdl2Application::KeyEvent::Key,
             OpenFF::InputEvents> _input_events_unmodified;
    std::map<Magnum::Platform::Sdl2Application::KeyEvent::Key,
             OpenFF::InputEvents> _input_events_shift;
    std::map<Magnum::Platform::Sdl2Application::KeyEvent::Key,
             OpenFF::InputEvents> _input_events_ctrl;

    std::function<void(OpenFF_Main&)>  _mainExit;
};

}
