// Return values taken from: https://unix.stackexchange.com/a/254747
#pragma once

#include <Corrade/Utility/DebugStl.h>

typedef Corrade::Utility::Debug Dbg;
typedef Corrade::Utility::Warning DbgWarn;
typedef Corrade::Utility::Error DbgErr;
typedef Corrade::Utility::Fatal DbgFatl;

template<typename val> void Err(val value) {
  DbgErr{} << Dbg::color(Dbg::Color::Yellow)
           << "Error:"
           << Dbg::resetColor
           << value;
}

template<typename val> void FatlSrcLogicError(val value) {
  DbgFatl{} << Dbg::color(Dbg::Color::Red)
            << "Fatal Source-Logic-Error:"
            << Dbg::resetColor
            << value;
}
template<typename val> void FatlIOError(val value) {
  DbgFatl{74} << Dbg::color(Dbg::Color::Red)
              << "Fatal IO-Error:"
              << Dbg::resetColor
              << value;
}
template<typename val> void FatlInputError(val value) {
  DbgFatl{66} << Dbg::color(Dbg::Color::Red)
              << "Fatal Input-Error:"
              << Dbg::resetColor
              << value;
}
