#pragma once

#ifdef TEXT_RENDERING_EXPORTS
#define TEXT_RENDERING_API __declspec(dllexport)
#else
#define TEXT_RENDERING_API __declspec(dllimport)
#endif

#include <SCFStandard.h>

//Reserved class keys: 0x0401 - 0x0450

namespace SCFTextRendering
{
	enum Classes
	{
		//Basic values
		ClassProperty = 0x0401,
		ClassStyle,
		ClassWord,
		ClassLine,
		ClassColumn,
		ClassParagraph,
		ClassFrame,

		ClassBoundaryStyle,
		ClassBoundaryItem,
		ClassTabEntry,
	};
};