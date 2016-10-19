#pragma once

#ifdef SCFGRAPHICS_EXPORTS
#define SCFGRAPHICS_API __declspec(dllexport)
#else
#define SCFGRAPHICS_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>
