#pragma once

#include <inicpp.h>

#include <string>
#include <map>

#include "InputHandler.h"
#include "../misc/EnumIterator.h"
#include "../misc/TerminalOutput.h"

class OpenFF_Main;

namespace OpenFF {

typedef Magnum::Platform::Sdl2Application::KeyEvent::Key Keycode;

enum ConfigurationSettings {
  background_location,
  CONFIGURATION_SETTINGS_MAX = background_location
};
typedef Iterator<ConfigurationSettings,
                 ConfigurationSettings::background_location,
                 ConfigurationSettings::CONFIGURATION_SETTINGS_MAX>
        configuration_settings_iterator;
enum SettingTypes {
  configuration_settings,
  input_event_settings,
  SETTING_TYPES_MAX = input_event_settings
};

std::string readFileData(const char* filename);


class Configuration{
  public:
    explicit Configuration(InputHandler*);

    void setBackgroundLocation(std::string);

    std::string getBackgroundLocation() const;

  private:
    void processIniConfigSections();
    template<typename settings_map> void processIniConfigOptions(
            settings_map*, inicpp::section, SettingTypes);
    void processIniConfigValues(SettingTypes, std::string, std::string);
    void processInputEventsFromConfig(std::string,std::string);
    void buildKeycodeLookupTable();

    InputHandler*   _input_handler;
    inicpp::config  _ini;

    std::string   _background_location;

    typedef std::map<OpenFF::ConfigurationSettings,
                     std::function<void(Configuration&,std::string)>> csc_map;
    csc_map  _configuration_setting_callbacks;
    std::map<std::string,OpenFF::ConfigurationSettings> _configuration_settings;
    std::map<std::string,OpenFF::InputEvents>           _input_event_settings;
    std::map<std::string,OpenFF::Keycode>               _keycodes;
};

template<typename enummap, typename iterator> bool validateEnumCallbackMaps(
        enummap* map, std::string name, iterator iter, unsigned int enum_size) {
  if( map->size() == enum_size ) {
    for( auto i=iter.begin(); i!=iter.end(); ++i ) {
      auto e_iter = map->find(*i);
      if( e_iter == map->end() ) {
        FatlSrcLogicError("Could not find case "+std::to_string(*i)+" in "+name+" map");
        return false;
      }
    }
  } else {
    FatlSrcLogicError("Not enough cases defined in "+name+" map");
    return false;
  }
  return true;
}
template<typename enummap> bool validateStringEnumMaps(
        enummap* map, std::string name, unsigned int enum_size) {
  if( map->size() != enum_size ) {
    FatlSrcLogicError("Not enough cases defined in "+name+" map");
    return false;
  }
  return true;
}

}
