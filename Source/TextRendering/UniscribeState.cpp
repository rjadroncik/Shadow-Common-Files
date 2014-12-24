#include "UniscribeState.h"

using namespace SCFBase;
using namespace SCFTextRendering;

CUniscribeState::tUniscribeState	CUniscribeState::s_Uniscribe;

CUniscribeState::CUniscribeState()
{
}

CUniscribeState::~CUniscribeState()
{
}

bool CUniscribeState::PrepareUniscribeBuffers(_IN int iRunLength)
{
	//Enlarge glyph dependent buffers if necessary
	if (((int)(iRunLength * 1.5) + 16) > (int)s_Uniscribe.iMaxGlyphs)
	{
		//Compute new buffer size (glyph count)
		s_Uniscribe.iMaxGlyphs		= (int)(iRunLength * 1.5) + 16;

		//Reallocate buffers
		s_Uniscribe.uspLogCluster	= (WORD*)CMemory::Reallocate(s_Uniscribe.uspLogCluster,		sizeof(WORD)				* s_Uniscribe.iMaxGlyphs);
		s_Uniscribe.uspGlyph		= (WORD*)CMemory::Reallocate(s_Uniscribe.uspGlyph,				sizeof(WORD)				* s_Uniscribe.iMaxGlyphs);
		s_Uniscribe.pVisAttrib		= (SCRIPT_VISATTR*)CMemory::Reallocate(s_Uniscribe.pVisAttrib,	sizeof(SCRIPT_VISATTR)	* s_Uniscribe.iMaxGlyphs);
		s_Uniscribe.ipAdvanceWidth	= (int*)CMemory::Reallocate(s_Uniscribe.ipAdvanceWidth,		sizeof(int)				* s_Uniscribe.iMaxGlyphs);
	}

	return TRUE;
}

void CUniscribeState::Initialize()
{
	//Uniscribe global, but private state
	s_Uniscribe.pScriptCache		= NULL;

	s_Uniscribe.iGeneratedGlyphs	= 0;
	s_Uniscribe.iMaxGlyphs			= 0;

	s_Uniscribe.uspLogCluster		= NULL;
	s_Uniscribe.uspGlyph			= NULL;

	s_Uniscribe.pVisAttrib			= NULL;
	s_Uniscribe.ipAdvanceWidth		= NULL;
}
