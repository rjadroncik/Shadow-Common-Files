#pragma once

#define SCF_INTEROP_CLI_1

#pragma managed(push, off)

#include <SCFObjectExtensions.h>

#pragma managed(pop)

#define StringCopy(dest, source) dest.Resize(source->Length); for (int fdtser = 0; fdtser < source->Length; fdtser++) { dest.AtPut(fdtser, source[fdtser]); }


