#pragma once

#include "Headers.h"

using namespace Gdiplus;
using namespace SCFGraphics;

namespace SCFTextRendering
{
	class CCache
	{
	public:
		static Graphics* pG; 
		static HDC       hDC;

		static StringFormat* pStandardStringFormat;

		static HFONT hFont;
		static Font* pFont;

		static TEXTMETRIC        TextMetrics;
		static OUTLINETEXTMETRIC OutlineMetrics;

		static Brush* pBrush;

		static SCF::DWORD dwRenderOptions;

		static CVector<CRectInt> SelectionRectList;

	//Used during measuring & rendering of runs to speed up processing
		static SCF::TCHAR* szVisualText;
		static int         iMaxVisualTextLength;

	private:
		CCache();
		~CCache();
	};
};
