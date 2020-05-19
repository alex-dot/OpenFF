#pragma once

#include <Magnum/Platform/Sdl2Application.h>

#include <functional>
#include <map>
#include <initializer_list>

#include "../misc/EnumIterator.h"

class OpenFF_Main;

namespace OpenFF {

class Music;

typedef Magnum::Platform::Sdl2Application::KeyEvent KeyEvent;

enum InputEvents {
  app_close,
  music_increase_gain,
  music_decrease_gain,
  INPUT_EVENTS_MAX = music_decrease_gain
};
typedef Iterator<InputEvents, InputEvents::app_close, InputEvents::INPUT_EVENTS_MAX> input_events_iterator;
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
    explicit InputHandler(OpenFF_Main*);

    bool processKeyReleaseEvent(KeyEvent& event);

    // callback setters
    void setCallableObjects(void*, std::initializer_list<InputEvents>);
    void setMainExitCallback(std::function<void(OpenFF_Main&)> f) {_mainExit = f;};
    void setMusicIncreaseGainCallback(std::function<void(Music&)> f) {_musicIncreaseGain = f;};
    void setMusicDecreaseGainCallback(std::function<void(Music&)> f) {_musicDecreaseGain = f;};

    void addKeyToInputEvents(KeyEvent::Key, ModifierType, InputEvents event);
    void removeKeyFromInputEvents(KeyEvent::Key, ModifierType);

  private:
    // callbacks
    void doMainExit(void* app) {_mainExit(*static_cast<OpenFF_Main*>(app));};
    void doMusicIncreaseGain(void* music) {_musicIncreaseGain(*static_cast<Music*>(music));}
    void doMusicDecreaseGain(void* music) {_musicDecreaseGain(*static_cast<Music*>(music));}

    std::map<KeyEvent::Key,OpenFF::InputEvents>*
            determineInputEventMap(ModifierType modifier);

    typedef std::map<OpenFF::InputEvents,
                     std::function<void(InputHandler&,void*)>> ec_map;
    ec_map _event_callbacks;
    std::map<OpenFF::InputEvents,void*>         _callable_objects;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_unmodified;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_shift;
    std::map<KeyEvent::Key,OpenFF::InputEvents> _input_events_ctrl;

    std::function<void(OpenFF_Main&)>  _mainExit;
    std::function<void(Music&)>        _musicIncreaseGain;
    std::function<void(Music&)>        _musicDecreaseGain;
};

}
