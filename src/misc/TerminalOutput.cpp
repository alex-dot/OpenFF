// Return values taken from: https://unix.stackexchange.com/a/254747
#include <Corrade/Utility/Debug.h>

#include <string>

#include "TerminalOutput.h"

void Err(std::string value) {
  DbgErr{} << Dbg::color(Dbg::Color::Yellow)
           << "Error:"
           << Dbg::resetColor
           << value.c_str();
}

void FatlSrcLogicError(std::string value) {
  DbgFatl{} << Dbg::color(Dbg::Color::Red)
            << "Fatal Source-Logic-Error:"
            << Dbg::resetColor
            << value.c_str();
}
void FatlIOError(std::string value) {
  DbgFatl{74} << Dbg::color(Dbg::Color::Red)
              << "Fatal IO-Error:"
              << Dbg::resetColor
              << value.c_str();
}
void FatlInputError(std::string value) {
  DbgFatl{66} << Dbg::color(Dbg::Color::Red)
              << "Fatal Input-Error:"
              << Dbg::resetColor
              << value.c_str();
}
