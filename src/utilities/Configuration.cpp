#include <inicpp.h>

#include <string>
#include <fstream>

#include "Configuration.h"
#include "InputHandler.h"

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
  // TODO add corrade file load error handling
  std::cout << "Error: could not open file at location: " << filename << std::endl;
  return "";
}

Configuration::Configuration(InputHandler* input_handler) :
        _input_handler(input_handler),
        _ini(),
        _background_location("../../ff7/data/flevel/ancnt1.png"),
        _configuration_settings(),
        _input_event_settings() {

  this->buildKeycodeLookupTable();

  // All configuration settings, len(_configuration_settings) === len(ConfigurationSettings-1)
  _configuration_settings["background_location"] = ConfigurationSettings::background_location;
  // All input event settings, len(_input_event_settings) === len(InputEvents-2)
  _input_event_settings["close_app"] = InputEvents::close_app;
  // TODO (compiler?) error handling for size mismatches

  _ini = inicpp::parser::load(readFileData("../conf/openff.ini"));
  this->processIniConfigSections();

  // configure all input events
  _input_handler->addKeyToInputEvents(
          KeyEvent::Key::Esc,
          ModifierType::unmodified,
          InputEvents::close_app);
  _input_handler->addKeyToInputEvents(
          KeyEvent::Key::Q,
          ModifierType::ctrl,
          InputEvents::close_app);
  _input_handler->addKeyToInputEvents(
          KeyEvent::Key::X,
          ModifierType::shift,
          InputEvents::close_app);
  _input_handler->removeKeyFromInputEvents(
          KeyEvent::Key::X,
          ModifierType::shift);
}

// This is where valid section names for the ini-file are defined
void Configuration::processIniConfigSections() {
  for( auto &sect : _ini ) {
    if( sect.get_name() == "Display" ) {
      processIniConfigOptions<std::map<std::string,OpenFF::ConfigurationSettings>>(&_configuration_settings,sect);
    } else if( sect.get_name() == "Controls" ) {
      processIniConfigOptions<std::map<std::string,OpenFF::InputEvents>>(&_input_event_settings,sect);
    }
	}
}
template<typename settings_map> void Configuration::processIniConfigOptions(
    settings_map* setting, inicpp::section options)
{
	for( auto &opt : options ) {
    auto sm_iter = setting->find(opt.get_name());
    if( sm_iter != setting->end() ) {
      // if the option is known, either parse single value or list of values
  		if (!opt.is_list()) {
	  		processInputEventsFromConfig(opt.get_name(), opt.get<inicpp::string_ini_t>());
		  } else {
        // interpret list of values
			  for (auto &list_item : opt.get_list<inicpp::string_ini_t>())
				  processInputEventsFromConfig(opt.get_name(), list_item);
		  }
    } else {
      // TODO add config file error handling (option not found)
      Corrade::Utility::Debug{} << opt.get_name().c_str();
    }
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
      // TODO add config file error handling (value invalid)
      Corrade::Utility::Debug{} << "nope";
    }
  }

  auto k_iter = _keycodes.find(key);
  if( k_iter != _keycodes.end() ) {
    _input_handler->addKeyToInputEvents(_keycodes[key], mod, _input_event_settings[option]);
  }
  // TODO add config file error handling (value invalid)
  else { Corrade::Utility::Debug{} << "nope"; }
}

void Configuration::setBackgroundLocation(std::string new_loc) {
  _background_location = new_loc;
}

std::string Configuration::getBackgroundLocation() const {
  return _background_location;
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
