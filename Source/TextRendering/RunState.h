#pragma once

#include "Headers.h"

using namespace Gdiplus;

namespace SCFTextRendering
{
	class CRunState
	{
	public:
		//Retrieves a new table of kerning pairs
		static bool PrepareKerning(); //Done

		//Returns the kerning amount for the specified 2 characters
		static int KerningOffset(_IN SCF::TCHAR cFirst, _IN SCF::TCHAR cSecond); //Done

		static void Initialize(); //Done

		//These functions operate on single runs - character ranges within words, which share the same style & are all part of the same uniscribe generated item
		static bool MeasureRun(_INOUT SCRIPT_ANALYSIS* pScriptAnalysis, _IN SCF::LPTSTR sText, _IN int iLength, _OUT int* iOutWidth, _OUT int* iOutLeading, _OUT int* iOutDescent, _OUT int* iOutAscent, _OUT int* ipOutCharWidths); //Done
		static bool RenderRun (_INOUT SCRIPT_ANALYSIS* pScriptAnalysis, _IN SCF::LPTSTR sText, _IN int iLength, _IN int iX, _IN int iY, _IN int* ipCharWidths); //Done

		static bool RenderWhitespaceRun(_IN int iX, _IN int iY, _IN int* ipCharWidths, _IN int iLength);

	private:
		struct tRunState;

	public:
		inline static tRunState& Run() { return s_Run; }

	private:
		CRunState();
		~CRunState();

		//Information used during run processing
		struct tRunState
		{
			//Temporary storage for run measurements
			int	iWidth;

			//Leading is not actual font ascent, but is adjusted to compensate for leading - can become negative
			int	iLeading;
			int	iDescent;

			//The actual ascent of the word - excluding space for diacritics
			int	iAscent;

			//Array of points, used to draw the string with different tracking values
			PointF*	pCharPos;
			int     usCharPosCount;

			//Array of kerning pair information structures, used to perform manual kerning
			KERNINGPAIR* pKerningPair;
			int          usKerningPairCount;
			int          usKerningPairCountUsed;

		} static s_Run; 
	};
};