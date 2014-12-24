#pragma once

#ifdef TEXT_RENDERING_EXPORTS
#define TEXT_RENDERING_API __declspec(dllexport)
#else
#define TEXT_RENDERING_API __declspec(dllimport)
#endif

#include <SCFStandard.h>

//Reserved error codes: 0x0401 - 0x0450

namespace SCFTextRendering
{
	enum Errors
	{
		//Generic Errors, should not be used as they are not very descriptive
		//ErrorBufferOverrun = 0x0401,
	};
};