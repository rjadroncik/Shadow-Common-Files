#pragma once

#include "Headers.h"

namespace SCFTextRendering
{
	class CUniscribeState
	{
	public:
		//Resizes buffers so that uniscribe can process a run of a given length (in rare cases - buffers may have to be resized multiple times before the run can be processed)
		static bool PrepareUniscribeBuffers(_IN int iRunLength); //Done
		static void Initialize();

	private:
		struct tUniscribeState;

	public:
		inline static tUniscribeState& Uniscribe() { return s_Uniscribe; }

	private:
		CUniscribeState();
		~CUniscribeState();

		//Structure for keeping uniscribe related state
		struct tUniscribeState
		{
			//Script cache used as a complementary argument besides the hdc, to speed up operations
			SCRIPT_CACHE pScriptCache;

			//ScriptShape() stores the number of generated glyphs in this variable
			int iGeneratedGlyphs;

			//The length of the glyph buffer (ipGlyph) provided to ScriptShape()
			int iMaxGlyphs;

			//ScriptShape() identifies logical clusters - multiple letters producing a single visual appearance - glyph - and saves them into this buffer
			WORD* uspLogCluster;
			//Glyph buffer used by ScriptShape(), ScriptPlace() & ScriptTextOut()
			WORD* uspGlyph;

			//ScriptShape() stores visual attributes into this buffer
			SCRIPT_VISATTR*	pVisAttrib;

			//ScriptPlace() generates & stores widths between individual glyphs in a run into this buffer
			int* ipAdvanceWidth;

		} static s_Uniscribe;
	};
};
