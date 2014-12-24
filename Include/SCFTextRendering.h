#pragma once

//Reserved class keys: 0x0401 - 0x0450

//Description: Adds text layout/rendering/printing features to the SCF.
//
//Version: 1.000

#include <SCFObjectExtensions.h>

#define SCF_TEXT_RENDERING_1

#include "../Source/TextRendering/Headers.h"

#include "../Source/TextRendering/Property.h"
#include "../Source/TextRendering/Style.h"

#include "../Source/TextRendering/Word.h"
#include "../Source/TextRendering/Line.h"
#include "../Source/TextRendering/Column.h"
#include "../Source/TextRendering/Paragraph.h"
#include "../Source/TextRendering/Frame.h"

namespace SCFTextRendering
{
	bool TEXT_RENDERING_API Initialize();
	bool TEXT_RENDERING_API Shutdown();
};
