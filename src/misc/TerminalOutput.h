#include <Corrade/Utility/Debug.h>

typedef Corrade::Utility::Debug Dbg;
typedef Corrade::Utility::Warning DbgWarn;
typedef Corrade::Utility::Error DbgErr;
typedef Corrade::Utility::Fatal DbgFatl;

void Err(std::string);

void FatlSrcLogicError(std::string);
void FatlIOError(std::string);
void FatlInputError(std::string);
