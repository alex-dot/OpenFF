#pragma once

#include <inicpp.h>

#include <string>
#include <map>

#include "InputHandler.h"

class OpenFF_Main;

namespace OpenFF {

typedef Magnum::Platform::Sdl2Application::KeyEvent::Key Keycode;

enum ConfigurationSettings {
  background_location,
  CONFIGURATION_SETTINGS_MAX = background_location
};
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

    std::map<OpenFF::ConfigurationSettings,
             std::function<void(Configuration&,std::string)>> _configuration_setting_callbacks;
    std::map<std::string,OpenFF::ConfigurationSettings> _configuration_settings;
    std::map<std::string,OpenFF::InputEvents>           _input_event_settings;
    std::map<std::string,OpenFF::Keycode>               _keycodes;
};

}
