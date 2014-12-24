#include <SCFObjectExtensions.h>
#include <SCFGraphics.h>

#include "Cache.h"
#include "Frame.h"
#include "Constants.h"

#include "CharacterStyleStack.h"
#include "ParagraphStyleStack.h"
#include "FrameStyleStack.h"
#include "UniscribeState.h"

using namespace Gdiplus;
using namespace SCFTextRendering;

//GDI+ startup/shutdown parameters
GdiplusStartupInput g_GdiPlusStartupInput;
ULONG_PTR           g_GdiPlusToken;

CString g_NewLine     (SCFTEXT("\x0a"));
CString g_NewParagraph(SCFTEXT("\x0d")); //TEXT("\x0d\x0a");
CString g_NewColumn   (SCFTEXT("\x0b"));

CString g_Tab    (SCFTEXT("\x09"));
CString g_NBSpace(SCFTEXT("\xa0"));

CString g_Space(SCFTEXT(" "));

bool APIENTRY DllMain(HANDLE hModule, SCF::DWORD ul_reason_for_call, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	UNREFERENCED_PARAMETER(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		{
			break;
		}
	case DLL_THREAD_ATTACH: 
		{
			break;
		}
	case DLL_THREAD_DETACH: 
		{
			break;
		}
	case DLL_PROCESS_DETACH: 
		{
			break;
		}
	}
	return TRUE;
}

namespace SCFTextRendering
{
	//Functions needed for proper dll functioning
	bool TEXT_RENDERING_API Initialize()
	{
		if (GdiplusStartup(&g_GdiPlusToken, &g_GdiPlusStartupInput, NULL) == Ok)
		{
			CCache::pStandardStringFormat = new StringFormat(StringFormat::GenericTypographic());
			if (!CCache::pStandardStringFormat) { SCFError(ErrorOutOfMemory); return FALSE; }
				
			CCache::pStandardStringFormat->SetFormatFlags(StringFormatFlagsLineLimit | StringFormatFlagsNoClip | StringFormatFlagsNoFitBlackBox | StringFormatFlagsMeasureTrailingSpaces);

			CCache::pBrush = new SolidBrush(Color(SCF_ARGB(255, 0, 0, 0))); 
			if (!CCache::pBrush) { SCFError(ErrorOutOfMemory); return FALSE; }

			CCharacterStyleStack::Initialize();
			CParagraphStyleStack::Initialize();
			CFrameStyleStack::Initialize();
			CUniscribeState::Initialize();

			return TRUE; 
		}

		return FALSE;
	}

	bool TEXT_RENDERING_API Shutdown()
	{
		if (CCache::pStandardStringFormat) { delete CCache::pStandardStringFormat; CCache::pStandardStringFormat	= NULL; }
		if (CCache::pBrush)                { delete CCache::pBrush;                CCache::pBrush					= NULL; }

		GdiplusShutdown(g_GdiPlusToken);
		return TRUE; 
	}
};