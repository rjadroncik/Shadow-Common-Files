#pragma once

#ifdef SCFGRAPHICS_EXPORTS
#define SCFGRAPHICS_API __declspec(dllexport)
#else
#define SCFGRAPHICS_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>

//Reserved class keys: 0x0451 - 0x0500

namespace SCFGraphics
{
	enum Classes
	{
		//Basic values
		ClassRectInt = 0x0451,
		ClassRectFloat,

		ClassPointInt,
		ClassPointFloat,

		ClassColor,
	};
};