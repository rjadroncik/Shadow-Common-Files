#pragma once

#include "Definitions.h"

//These are internal definitions intentionally not revealed to the DLL client
#define	Win32ClassCButton  TEXT("ShadowControlsButton")
#define	Win32ClassCSpinner TEXT("ShadowControlsSpinner")
#define	Win32ClassCWindow  TEXT("ShadowControlsWindow")

using namespace Gdiplus;
using namespace SCFMathematics;
using namespace SCFBase;

namespace SCFUI
{
	class CPaint
	{
	public:
		static bool Initialize();

		static void Draw3DRectangle(_INOUT Graphics* pGraphics, _IN SCFGraphics::Rect4i& rRect, _IN UINT uiBevel, _IN Pen* pPenLeftTop, _IN Pen* pPenRightBottom, _IN Brush* pBrush);
		static void Draw3DCircle   (_INOUT Graphics* pGraphics, _IN SCFGraphics::Rect4i& rRect,                   _IN Pen* pPenLeftTop, _IN Pen* pPenRightBottom, _IN Brush* pBrush);
		static void Draw3DCheckMark(_INOUT Graphics* pGraphics, _IN SCFGraphics::Rect4i& rRect,                   _IN Pen* pPenLeftTop, _IN Pen* pPenRightBottom, _IN Brush* pBrush);
	  	
		static void RectMoveSides(_INOUT SCFGraphics::Rect4i& rRect, _IN int iLeft, _IN int iTop, _IN int iRight, _IN int iBottom);
		static void RectResize   (_INOUT SCFGraphics::Rect4i& rRect, _IN int iOffset);

	public:
		struct SColors
		{
			Color* pLight; 
			Color* pShadow; 
			Color* pFace; 
			Color* pLightFace;
			Color* pDarkShadow; 
			Color* pLightShadow; 

			Color* pOrange;
			Color* pBlue;
			Color* pRed;

			Color* pBlack;
			Color* pWhite;
			Color* pVoid;

		} static SysColors;

		struct SPens
		{
			Pen* pLight;
			Pen* pShadow;
			Pen* pFace;
			Pen* pLightFace;
			Pen* pDarkShadow;
			Pen* pLightShadow; 

			Pen* pFace_3px;
			Pen* pLightFace_3px;
			Pen* pLightShadow_3px; 
			Pen* pShadow_3px; 
			Pen* pOrange_3px;
			Pen* pBlue_3px;

			Pen* pOrange;
			Pen* pBlue;
			Pen* pRed;
			
			Pen* pBlack;
			Pen* pWhite;
			Pen* pVoid;

			Pen* pDottedGray;

		} static SysPens;

		struct SBrushes
		{
			SolidBrush* pLight;
			SolidBrush* pShadow;
			SolidBrush* pFace;
			SolidBrush* pLightFace;
			SolidBrush* pDarkShadow;
			SolidBrush* pLightShadow; 

			SolidBrush* pOrange;
			SolidBrush* pBlue;
			SolidBrush* pRed;
		
			SolidBrush* pBlack;
			SolidBrush* pWhite;
			SolidBrush* pVoid;

		//	SolidBrush* pToolTip;

		} static SysBrushes;

		struct SFont
		{
			Font* pArial_11px;
			Font* pArial_11px_Bold;
			Font* pArial_14px;
			Font* pArial_14px_Bold;

		} static SysFonts;

	private:
		CPaint();
		~CPaint();
	};
};