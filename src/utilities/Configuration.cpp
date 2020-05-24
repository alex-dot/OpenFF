#include <Corrade/Utility/Directory.h>
#include <inicpp.h>

#include <string>
#include <fstream>

#include "Configuration.h"
#include "InputHandler.h"
#include "../misc/TerminalOutput.h"

using namespace OpenFF;

std::string OpenFF::readFileData(const char* filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  FatlInputError<std::string>("Error: could not open file at location: "+std::string(filename));
  return "";
}

Configuration::Configuration(InputHandler* input_handler) :
        _input_handler(input_handler),
        _ini(),
        _background_location(""),
        _configuration_settings(),
        _input_event_settings() {

  this->buildKeycodeLookupTable();

  // Populate setting callbacks
  _configuration_setting_callbacks[ConfigurationSettings::background_location]
          = &Configuration::setBackgroundLocation;
  _configuration_setting_callbacks[ConfigurationSettings::music_location]
          = &Configuration::setMusicLocation;
  validateEnumCallbackMaps<csc_map>
          (&_configuration_setting_callbacks,
          "configuration settings callback",
          configuration_settings_iterator(),
          ConfigurationSettings::CONFIGURATION_SETTINGS_MAX+1);
  // All configuration settings, len(_configuration_settings) === len(ConfigurationSettings-1)
  _configuration_settings["background_location"] = ConfigurationSettings::background_location;
  _configuration_settings["music_location"] = ConfigurationSettings::music_location;
  validateStringEnumMaps<std::map<std::string,OpenFF::ConfigurationSettings>>
          (&_configuration_settings,
          "configuration settings",
          ConfigurationSettings::CONFIGURATION_SETTINGS_MAX+1);
  // All input event settings, len(_input_event_settings) === len(InputEvents-2)
  _input_event_settings["undefined"] = InputEvents::ie_undefined;
  _input_event_settings["app_close"] = InputEvents::app_close;
  _input_event_settings["music_increase_gain"] = InputEvents::music_increase_gain;
  _input_event_settings["music_decrease_gain"] = InputEvents::music_decrease_gain;
  _input_event_settings["music_pause"] = InputEvents::music_pause;
  validateStringEnumMaps<std::map<std::string,OpenFF::InputEvents>>
          (&_input_event_settings,
          "input event settings",
          InputEvents::INPUT_EVENTS_MAX+1);

  _ini = inicpp::parser::load(readFileData("../conf/openff.ini"));
  this->processIniConfigSections();

  this->buildMusicMap();

  // kept for posterity, should be moved once unit tests are implemented
  _input_handler->addKeyToInputEvents(
          KeyEvent::Key::X,
          ModifierType::shift,
          InputEvents::app_close);
  _input_handler->removeKeyFromInputEvents(
          KeyEvent::Key::X,
          ModifierType::shift);
}

// This is where valid section names for the ini-file are defined
void Configuration::processIniConfigSections() {
  for( auto &sect : _ini ) {
    if( sect.get_name() == "Display" || sect.get_name() == "Music" ) {
      processIniConfigOptions<std::map<std::string,OpenFF::ConfigurationSettings>>
              (&_configuration_settings, sect, SettingTypes::configuration_settings);
    } else if( sect.get_name() == "Controls" ) {
      processIniConfigOptions<std::map<std::string,OpenFF::InputEvents>>
              (&_input_event_settings, sect, SettingTypes::input_event_settings);
    }
	}
}
template<typename settings_map> void Configuration::processIniConfigOptions(
    settings_map* setting, inicpp::section options, SettingTypes type)
{
	for( auto &opt : options ) {
    auto sm_iter = setting->find(opt.get_name());
    if( sm_iter != setting->end() ) {
      // if the option is known, either parse single value or list of values
  		if (!opt.is_list()) {
        processIniConfigValues(type, opt.get_name(), opt.get<inicpp::string_ini_t>());
		  } else {
        // interpret list of values
			  for (auto &list_item : opt.get_list<inicpp::string_ini_t>()) {
				  processIniConfigValues(type, opt.get_name(), list_item);
        }
		  }
    } else {
      Err<std::string>("Parsing INI: Unknown Option \""+opt.get_name()+"\".");
    }
  }
}
void Configuration::processIniConfigValues(SettingTypes type, std::string option, std::string value) {
  switch( type ) {
    case SettingTypes::configuration_settings:
      _configuration_setting_callbacks[_configuration_settings[option]](*this, value);
      break;
    case SettingTypes::input_event_settings:
      processInputEventsFromConfig(option, value);
      break;
  }
}

void Configuration::processInputEventsFromConfig(std::string option, std::string input) {
  std::string key = input;
  ModifierType mod = ModifierType::unmodified;
  size_t pos = input.find("+");
  if( pos != std::string::npos ) {
    std::string modifier = input.substr(0,pos);
    key = input.substr(pos+1,input.length());
    if( modifier == "Ctrl" ) {
      mod = ModifierType::ctrl;
    } else if( modifier == "Shift" ) {
      mod = ModifierType::shift;
    } else {
      Err<std::string>("Parsing INI: Value  \""+modifier+"\" unsupported for "+option+".");
      Err<std::string>("Parsing INI: Valid values are \"Ctrl\", \"Shift\".");
    }
  }

  auto k_iter = _keycodes.find(key);
  if( k_iter != _keycodes.end() ) {
    _input_handler->addKeyToInputEvents(_keycodes[key], mod, _input_event_settings[option]);
  } else {
    Err<std::string>("Parsing INI: Value  \""+key+"\" unsupported for "+option+".");
    Err<std::string>("Parsing INI: Look at TODO for a list of valid keys.");
  }
}

void Configuration::setBackgroundLocation(std::string new_loc) {
  _background_location = new_loc;
}
void Configuration::setMusicLocation(std::string new_loc) {
  _music_location = new_loc;
}

std::string Configuration::getBackgroundLocation() const {
  return _background_location;
}
std::string Configuration::getMusicLocation() const {
  return _music_location;
}
bool Configuration::getMusicLocation(
        std::string& location,
        std::string short_name) const {
  auto it = _music_location_map.find(short_name);
  if( it != _music_location_map.end() ) {
    location = it->second;
    return true;
  }
  return false;
}
bool Configuration::getMusicName(
        std::string& name,
        std::string short_name) const {
  auto it = _music_name_map.find(short_name);
  if( it != _music_name_map.end() ) {
    name = it->second;
    return true;
  }
  return false;
}
std::string Configuration::getRandomMusic() const {
  return _music_name_map.find("tb")->first;
}

void Configuration::buildMusicMap() {
  // build names
  inicpp::config music_ini = inicpp::parser::load(readFileData("../conf/music.ini"));
  for( auto &sect : music_ini ) {
    if( sect.get_name() == "music" ) {
    	for( auto &opt : sect ) {
        _music_name_map[opt.get_name()] = opt.get<inicpp::string_ini_t>();
      }
    }
	}
  // build locations
  std::vector<std::string> dir = Corrade::Utility::Directory::list(this->getMusicLocation());
  for(auto it = dir.begin(); it != dir.end(); ++it) {
    if(it->length() > 4 && it->substr(it->length()-4) == ".ogg")
      _music_location_map[it->substr(0,it->length()-4)] = *it;
  }
}

void Configuration::buildKeycodeLookupTable() {
  // this currently only support US keyboards
  // we should probably use a lib like https://github.com/depp/keycode
  // to generalize this - controls will still be saved in quasi US-layout
  // but users could define them ingame to whatever
  _keycodes["Enter"] = OpenFF::Keycode::Enter;
  _keycodes["Esc"] = OpenFF::Keycode::Esc;
  _keycodes["Up"] = OpenFF::Keycode::Up;
  _keycodes["Down"] = OpenFF::Keycode::Down;
  _keycodes["Left"] = OpenFF::Keycode::Left;
  _keycodes["Right"] = OpenFF::Keycode::Right;
  _keycodes["Home"] = OpenFF::Keycode::Home;
  _keycodes["End"] = OpenFF::Keycode::End;
  _keycodes["PageUp"] = OpenFF::Keycode::PageUp;
  _keycodes["PageDown"] = OpenFF::Keycode::PageDown;
  _keycodes["Backspace"] = OpenFF::Keycode::Backspace;
  _keycodes["Insert"] = OpenFF::Keycode::Insert;
  _keycodes["Delete"] = OpenFF::Keycode::Delete;
  _keycodes["F1"] = OpenFF::Keycode::F1;
  _keycodes["F2"] = OpenFF::Keycode::F2;
  _keycodes["F3"] = OpenFF::Keycode::F3;
  _keycodes["F4"] = OpenFF::Keycode::F4;
  _keycodes["F5"] = OpenFF::Keycode::F5;
  _keycodes["F6"] = OpenFF::Keycode::F6;
  _keycodes["F7"] = OpenFF::Keycode::F7;
  _keycodes["F8"] = OpenFF::Keycode::F8;
  _keycodes["F9"] = OpenFF::Keycode::F9;
  _keycodes["F10"] = OpenFF::Keycode::F10;
  _keycodes["F11"] = OpenFF::Keycode::F11;
  _keycodes["F12"] = OpenFF::Keycode::F12;
  _keycodes["Space"] = OpenFF::Keycode::Space;
  _keycodes["Tab"] = OpenFF::Keycode::Tab;
  _keycodes["\""] = OpenFF::Keycode::Quote;
  _keycodes[","] = OpenFF::Keycode::Comma;
  _keycodes["."] = OpenFF::Keycode::Period;
  _keycodes["-"] = OpenFF::Keycode::Minus;
  _keycodes["Plus"] = OpenFF::Keycode::Plus;
  _keycodes["/"] = OpenFF::Keycode::Slash;
  _keycodes["%"] = OpenFF::Keycode::Percent;
  _keycodes[";"] = OpenFF::Keycode::Semicolon;
  _keycodes["="] = OpenFF::Keycode::Equal;
  _keycodes["["] = OpenFF::Keycode::LeftBracket;
  _keycodes["]"] = OpenFF::Keycode::RightBracket;
  _keycodes["\\"] = OpenFF::Keycode::Backslash;
  _keycodes["`"] = OpenFF::Keycode::Backquote;
  _keycodes["0"] = OpenFF::Keycode::Zero;
  _keycodes["1"] = OpenFF::Keycode::One;
  _keycodes["2"] = OpenFF::Keycode::Two;
  _keycodes["3"] = OpenFF::Keycode::Three;
  _keycodes["4"] = OpenFF::Keycode::Four;
  _keycodes["5"] = OpenFF::Keycode::Five;
  _keycodes["6"] = OpenFF::Keycode::Six;
  _keycodes["7"] = OpenFF::Keycode::Seven;
  _keycodes["8"] = OpenFF::Keycode::Eight;
  _keycodes["9"] = OpenFF::Keycode::Nine;
  _keycodes["A"] = OpenFF::Keycode::A;
  _keycodes["B"] = OpenFF::Keycode::B;
  _keycodes["C"] = OpenFF::Keycode::C;
  _keycodes["D"] = OpenFF::Keycode::D;
  _keycodes["E"] = OpenFF::Keycode::E;
  _keycodes["F"] = OpenFF::Keycode::F;
  _keycodes["G"] = OpenFF::Keycode::G;
  _keycodes["H"] = OpenFF::Keycode::H;
  _keycodes["I"] = OpenFF::Keycode::I;
  _keycodes["J"] = OpenFF::Keycode::J;
  _keycodes["K"] = OpenFF::Keycode::K;
  _keycodes["L"] = OpenFF::Keycode::L;
  _keycodes["M"] = OpenFF::Keycode::M;
  _keycodes["N"] = OpenFF::Keycode::N;
  _keycodes["O"] = OpenFF::Keycode::O;
  _keycodes["P"] = OpenFF::Keycode::P;
  _keycodes["Q"] = OpenFF::Keycode::Q;
  _keycodes["R"] = OpenFF::Keycode::R;
  _keycodes["S"] = OpenFF::Keycode::S;
  _keycodes["T"] = OpenFF::Keycode::T;
  _keycodes["U"] = OpenFF::Keycode::U;
  _keycodes["V"] = OpenFF::Keycode::V;
  _keycodes["W"] = OpenFF::Keycode::W;
  _keycodes["X"] = OpenFF::Keycode::X;
  _keycodes["Y"] = OpenFF::Keycode::Y;
  _keycodes["Z"] = OpenFF::Keycode::Z;
  _keycodes["Num0"] = OpenFF::Keycode::NumZero;
  _keycodes["Num1"] = OpenFF::Keycode::NumOne;
  _keycodes["Num2"] = OpenFF::Keycode::NumTwo;
  _keycodes["Num3"] = OpenFF::Keycode::NumThree;
  _keycodes["Num4"] = OpenFF::Keycode::NumFour;
  _keycodes["Num5"] = OpenFF::Keycode::NumFive;
  _keycodes["Num6"] = OpenFF::Keycode::NumSix;
  _keycodes["Num7"] = OpenFF::Keycode::NumSeven;
  _keycodes["Num8"] = OpenFF::Keycode::NumEight;
  _keycodes["Num9"] = OpenFF::Keycode::NumNine;
  _keycodes["Num."] = OpenFF::Keycode::NumDecimal;
  _keycodes["Num:"] = OpenFF::Keycode::NumDivide;
  _keycodes["Num*"] = OpenFF::Keycode::NumMultiply;
  _keycodes["Num-"] = OpenFF::Keycode::NumSubtract;
  _keycodes["NumAdd"] = OpenFF::Keycode::NumAdd;
  _keycodes["NumEnter"] = OpenFF::Keycode::NumEnter;
  _keycodes["Num="] = OpenFF::Keycode::NumEqual;
}
