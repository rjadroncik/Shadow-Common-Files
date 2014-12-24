#include "Paint.h"

using namespace SCFUI;

CPaint::SColors  CPaint::SysColors; 
CPaint::SPens    CPaint::SysPens;
CPaint::SBrushes CPaint::SysBrushes;

CPaint::SFont  CPaint::SysFonts;

bool Paint_bInitDone = FALSE;

CPaint::CPaint()
{	
}

CPaint::~CPaint()
{
}

void CPaint::Draw3DRectangle(_INOUT Graphics* pGraphics, _IN SCFGraphics::Rect4i& rRect, _IN UINT uiBevel, _IN Pen* pPenLeftTop, _IN Pen* pPenRightBottom, _IN Brush* pBrush)
{
	Point point[9];

	point[0].X = rRect.iX + uiBevel;
	point[0].Y = rRect.iHeight  - 1;

	point[1].X = rRect.iX;
	point[1].Y = rRect.iHeight - uiBevel - 1;

	point[2].X = rRect.iX;
	point[2].Y = rRect.iY + uiBevel;

	point[3].X = rRect.iX + uiBevel;
	point[3].Y = rRect.iY;

	point[4].X = rRect.iWidth - uiBevel - 1;
	point[4].Y = rRect.iY;

	point[5].X = rRect.iWidth - 1;
	point[5].Y = rRect.iY + uiBevel;

	point[6].X = rRect.iWidth - 1;
	point[6].Y = rRect.iHeight - uiBevel - 1;

	point[7].X = rRect.iWidth - uiBevel - 1;
	point[7].Y = rRect.iHeight - 1;

	point[8].X = rRect.iX + uiBevel;
	point[8].Y = rRect.iHeight  - 1;

	if (pBrush)				{ pGraphics->FillPolygon(pBrush, point, 8, FillModeWinding); }
	if (pPenLeftTop)		{ pGraphics->DrawLines(pPenLeftTop, point, 5); }
	if (pPenRightBottom)	{ pGraphics->DrawLines(pPenRightBottom, &point[4], 5); }
}


void CPaint::Draw3DCheckMark(_INOUT Graphics* pGraphics, _IN SCFGraphics::Rect4i& rRect, _IN Pen* pPenLeftTop, _IN Pen* pPenRightBottom, _IN Brush* pBrush)
{
	Point point[5];

	point[0].X = rRect.iX; //+ rRect.iWidth  * 0.0;
	point[0].Y = rRect.iY + (int)(rRect.iHeight * 0.4);

	point[1].X = rRect.iX + (int)(rRect.iWidth  * 0.5);
	point[1].Y = rRect.iY + (int)(rRect.iHeight * 0.5);

	point[2].X = rRect.iX + rRect.iWidth;  //* 1;
	point[2].Y = rRect.iY; //+ rRect.iHeight * 0.0;

	point[3].X = rRect.iX + (int)(rRect.iWidth  * 0.5);
	point[3].Y = rRect.iY + rRect.iHeight; //* 1;

	point[4].X = rRect.iX; //+ rRect.iWidth  * 0.0;
	point[4].Y = rRect.iY + (int)(rRect.iHeight * 0.4);

	if (pBrush)				{ pGraphics->FillPolygon(pBrush, point, 4, FillModeWinding); }
	if (pPenLeftTop)		{ pGraphics->DrawLines(pPenLeftTop, point, 3); }
	if (pPenRightBottom)	{ pGraphics->DrawLines(pPenRightBottom, &point[2], 3); }
}

void CPaint::Draw3DCircle(_INOUT Graphics* pGraphics, _IN SCFGraphics::Rect4i& rRect, _IN Pen* pPenLeftTop, _IN Pen* pPenRightBottom, _IN Brush* pBrush)
{
	if (pBrush)				{ pGraphics->FillEllipse(pBrush, rRect.iX, rRect.iY, rRect.iWidth - rRect.iX - 1, rRect.iHeight - rRect.iY - 1); }

	if (pPenLeftTop)		{ pGraphics->DrawArc(pPenLeftTop,     rRect.iX, rRect.iY, rRect.iWidth - rRect.iX - 1, rRect.iHeight - rRect.iY - 1, 135, 180); }
	if (pPenRightBottom)	{ pGraphics->DrawArc(pPenRightBottom, rRect.iX, rRect.iY, rRect.iWidth - rRect.iX - 1, rRect.iHeight - rRect.iY - 1, 315, 180); }
}

void CPaint::RectMoveSides(_INOUT SCFGraphics::Rect4i& rRect, _IN int iLeft, _IN int iTop, _IN int iRight, _IN int iBottom)
{
	rRect.iX += iLeft;
	rRect.iY += iTop;

	rRect.iWidth  += iRight - iLeft;
	rRect.iHeight += iBottom - iTop;
}

void CPaint::RectResize(_INOUT SCFGraphics::Rect4i& rRect, _IN int iOffset)
{
	rRect.iX -= iOffset;
	rRect.iY -= iOffset;

	rRect.iWidth  += iOffset;
	rRect.iHeight += iOffset;
}

bool CPaint::Initialize()
{
	if (Paint_bInitDone) { return TRUE; } Paint_bInitDone = TRUE;

//Fetch system colors -> Done only once -> SpEEEED.... 
	SysColors.pLight			= new Color(SCF_WINRGB_TO_ARGB(GetSysColor(COLOR_BTNHIGHLIGHT))); 
	SysColors.pShadow			= new Color(SCF_WINRGB_TO_ARGB(GetSysColor(COLOR_BTNSHADOW))); 
	SysColors.pFace				= new Color(SCF_WINRGB_TO_ARGB(GetSysColor(COLOR_BTNFACE))); 

//	SysColors.pLightFace		= RGBA(255, 255, 255, 64));
	SysColors.pLightFace		= new Color(SCF_ARGB(255, SysColors.pFace->GetR() + 8, SysColors.pFace->GetG() + 8, SysColors.pFace->GetB() + 12)); 

	SysColors.pDarkShadow		= new Color(SCF_WINRGB_TO_ARGB(GetSysColor(COLOR_3DDKSHADOW))); 
	SysColors.pLightShadow		= new Color(SCF_ARGB(255, SysColors.pShadow->GetR() + 30, SysColors.pShadow->GetG() + 30, SysColors.pShadow->GetB() + 30)); 

	SysColors.pOrange			= new Color(SCF_ARGB(196, 255, 204, 0));
	SysColors.pBlue				= new Color(SCF_ARGB(64,  0,   0,   255));
	SysColors.pRed				= new Color(SCF_ARGB(255, 255, 0,   0));

	SysColors.pBlack			= new Color(SCF_ARGB(255, 0,   0,   0));
	SysColors.pWhite			= new Color(SCF_ARGB(255, 255, 255, 255));
	SysColors.pVoid				= new Color(SCF_ARGB(0,   0,   0,   0));

	SysPens.pLight				= new Pen(*SysColors.pLight, 1);
	SysPens.pShadow				= new Pen(*SysColors.pShadow, 1);
	SysPens.pFace				= new Pen(*SysColors.pFace, 1);
	SysPens.pLightFace			= new Pen(*SysColors.pLightFace, 1);
	SysPens.pLightShadow		= new Pen(*SysColors.pLightShadow, 1);
	SysPens.pDarkShadow			= new Pen(*SysColors.pDarkShadow, 1);

	SysPens.pOrange				= new Pen(*SysColors.pOrange, 1);
	SysPens.pBlue				= new Pen(*SysColors.pBlue, 1);
	SysPens.pRed				= new Pen(*SysColors.pRed, 1);

	SysPens.pBlue_3px			= new Pen(*SysColors.pBlue, 3);
	SysPens.pFace_3px			= new Pen(*SysColors.pFace, 3);
	SysPens.pLightFace_3px		= new Pen(*SysColors.pLightFace, 3);
	SysPens.pLightShadow_3px	= new Pen(*SysColors.pLightShadow, 3); 
	SysPens.pShadow_3px			= new Pen(*SysColors.pShadow, 3); 
	SysPens.pOrange_3px			= new Pen(*SysColors.pOrange, 3);

	SysPens.pBlack				= new Pen(*SysColors.pBlack, 1);
	SysPens.pWhite				= new Pen(*SysColors.pWhite, 1);
	SysPens.pVoid				= new Pen(*SysColors.pVoid , 0);

	SysPens.pDottedGray			= new Pen(Color(196, 0, 0, 0), 1);
	SysPens.pDottedGray->SetDashStyle(DashStyleDot);

	SysBrushes.pLightShadow		= new SolidBrush(*SysColors.pLightShadow);
	SysBrushes.pDarkShadow		= new SolidBrush(*SysColors.pDarkShadow);
	SysBrushes.pShadow			= new SolidBrush(*SysColors.pShadow);
	SysBrushes.pFace			= new SolidBrush(*SysColors.pFace);
	SysBrushes.pLightFace		= new SolidBrush(*SysColors.pLightFace);

	SysBrushes.pOrange			= new SolidBrush(*SysColors.pOrange);
	SysBrushes.pBlue			= new SolidBrush(*SysColors.pBlue);
	SysBrushes.pRed				= new SolidBrush(*SysColors.pRed);

	SysBrushes.pBlack			= new SolidBrush(*SysColors.pBlack);
	SysBrushes.pWhite			= new SolidBrush(*SysColors.pWhite);
	SysBrushes.pVoid			= new SolidBrush(*SysColors.pVoid);

	//Brush.pToolTip = new SolidBrush(RGBA(255, 255, 225));

	SysFonts.pArial_11px      = new Font(TEXT("Arial"), 11, FontStyleRegular, UnitPixel, NULL);
	SysFonts.pArial_11px_Bold = new Font(TEXT("Arial"), 11, FontStyleBold,    UnitPixel, NULL);
	
	SysFonts.pArial_14px      = new Font(TEXT("Arial"), 14, FontStyleRegular, UnitPixel, NULL);
	SysFonts.pArial_14px_Bold = new Font(TEXT("Arial"), 14, FontStyleBold,    UnitPixel, NULL);

/*
	hImageList16 = ImageList_Create(16, 16, ILC_MASK | ILC_COLOR8, 32, 32); 
	hImageList24 = ImageList_Create(24, 24, ILC_MASK | ILC_COLOR8, 32, 32); 

	Font.Arial_14px      = new Font(14, 5, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_ROMAN, TEXT("Arial"));
	Font.Arial_14px_Bold = new Font(15, 6, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_ROMAN, TEXT("Arial"));
*/
	return TRUE;
}
