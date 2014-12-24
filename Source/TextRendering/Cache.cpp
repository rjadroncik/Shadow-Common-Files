#include "Cache.h"

using namespace SCFTextRendering;

Graphics* CCache::pG  = NULL; 
HDC       CCache::hDC = NULL;

StringFormat* CCache::pStandardStringFormat	= NULL;

HFONT CCache::hFont = NULL;
Font* CCache::pFont = NULL;

TEXTMETRIC        CCache::TextMetrics;
OUTLINETEXTMETRIC CCache::OutlineMetrics;

Brush* CCache::pBrush = NULL;

SCF::DWORD CCache::dwRenderOptions = NULL;

CVector CCache::SelectionRectList;

SCF::TCHAR* CCache::szVisualText = NULL;
int         CCache::iMaxVisualTextLength = 0;
