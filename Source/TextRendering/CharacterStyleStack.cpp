#include "CharacterStyleStack.h"
#include "Cache.h"
#include "RunState.h"

using namespace SCFBase;
using namespace SCFTextRendering;

CCharacterStyleStack::CStyleState CCharacterStyleStack::s_Style;

CStack CCharacterStyleStack::s_Styles;
CStack CCharacterStyleStack::s_PopQueue;
bool CCharacterStyleStack::s_bFontChanged = TRUE;

void CCharacterStyleStack::Initialize()
{
	CRunState::Initialize();
	ResetStyleState();
}

bool CCharacterStyleStack::PushStyleStack(_IN CStyle& rStyle)
{
	s_Styles.Push(*(new CStyleState(s_Style)));

	if (s_Style.Font.Family.Length()) 
	{ 
		s_Style.Font.Family = ((CStyleState*)s_Styles.Top())->Font.Family;
	}

	if (s_Style.Font.AlternateFamily.Length()) 
	{
		s_Style.Font.AlternateFamily = ((CStyleState*)s_Styles.Top())->Font.AlternateFamily;
	}

	s_Style.pStyle = &rStyle;

	return TRUE;
}

void CCharacterStyleStack::FlushStyleStack()
{
	while (!s_Styles.IsEmpty()) 
	{
		s_Styles.PopAndDelete(); 
	}
}

bool CCharacterStyleStack::PopStyleStack(_IN CStyle& rStyle)
{
	if (!s_Styles.Size()) { return FALSE; }

	//If we tray to pop a style that is not at the top..
	if (s_Style.pStyle != &rStyle)
	{
		//..add the pop request into a queue
		s_PopQueue.Push(rStyle);
		return TRUE;
	}

	//Pop stack
	s_Style = *(CStyleState*)s_Styles.Top();
	s_Styles.PopAndDelete();

	s_bFontChanged = TRUE;

	//If pop queue is not empty..
	if (s_PopQueue.Size())
	{
		//..check whether the current entry in the queue matches the style id after popping
		if (s_PopQueue.Top() == s_Style.pStyle)
		{
			//Remove entry from queue
			s_PopQueue.Pop();

			//Recursively pop stack 
			PopStyleStack(*s_Style.pStyle);
		}
	}

	return TRUE;
}

void CCharacterStyleStack::ResetStyleState()
{
	//Empty style stack to flush open/unsettled styles - that is styles that have an infinite length (specified as -1), these styles have only a PUSH entry & no matched POP entry, this prevents the stack to infinitely increase its size 
	while (!s_Styles.IsEmpty())
	{
		//Pop stack
		s_Styles.PopAndDelete();
	}

	//Clear pop queue
	while (!s_PopQueue.IsEmpty()) { s_PopQueue.PopAndDelete(); }

	//Character style - global, but private state
	s_Style.Color = SCF_RGBA(0, 0, 0, 255);

	s_Style.Font.dwStyle = FontStyleRegular;
	s_Style.Font.iSize   = 14 * PT_2_INTERNAL;

	s_Style.iLeading  = 1200;
	s_Style.iTracking = 0;

	s_Style.ePosition = POS_NORMAL;
	s_Style.eKerning  = KERNING_METRICS;

	s_bFontChanged = TRUE;;

	s_Style.Underline.eType  = US_NONE;
	s_Style.Underline.Color  = 0x000000ff;
	s_Style.Underline.iWidth = 1000;
	
	s_Style.StrikeThrough.eType  = US_NONE;
	s_Style.StrikeThrough.Color  = 0x000000ff;
	s_Style.StrikeThrough.iWidth = 1000;

	UpdateStyleState();
}

bool CCharacterStyleStack::QueryIntegerValue(_IN SCF::ENUM eProperty, _OUT int* fpOutInteger)
{
	//Check arguments
	if (!fpOutInteger) { SCFError(ErrorInvalidArgument); return FALSE; }

	switch (eProperty)
	{
	case CP_SIZE:		{ *fpOutInteger = s_Style.Font.iSize;	return TRUE; }
	case CP_LEADING:	{ *fpOutInteger = s_Style.iLeading;		return TRUE; }
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

//This function filters the style properties it recognizes & updates state accordingly
bool CCharacterStyleStack::ExecuteStyle(_IN CStyle& rStyle)
{
	//Recursively execute base styles
	if (rStyle.HasBaseStyle() && (rStyle.BaseID() != rStyle.ID())) 
	{
		ExecuteStyle(rStyle.BaseID()); 
	}

	CEnumeratorDictionaryInt64 Enumerator(rStyle.Properties());

	//Update state, when a recognized property is found
	while (Enumerator.Next())
	{
		CProperty* pProperty = (CProperty*)Enumerator.Current();
		switch (pProperty->ID())
		{
		case CP_FONT_FAMILY:
			{
				s_Style.Font.Family = ((CString&)pProperty->Value());
				s_bFontChanged = TRUE;
				break;
			}	
		case CP_ALTERNATE_FAMILY:
			{
				s_Style.Font.AlternateFamily = ((CString&)pProperty->Value());
				s_bFontChanged = TRUE;
				break;
			}

		case CP_FONT_STYLE:
			{
				if ((STRING("Regular")) == ((CString&)pProperty->Value())) 
				{
					//Clear styles
					s_Style.Font.dwStyle |= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;
					s_Style.Font.dwStyle ^= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;

					s_Style.Font.dwStyle |= FontStyleRegular;
				}

				if ((STRING("Bold")) == ((CString&)pProperty->Value()))
				{
					//Clear styles
					s_Style.Font.dwStyle |= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;
					s_Style.Font.dwStyle ^= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;

					s_Style.Font.dwStyle |= FontStyleBold;
				}

				if ((STRING("Italic")) == ((CString&)pProperty->Value()))
				{
					//Clear styles
					s_Style.Font.dwStyle |= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;
					s_Style.Font.dwStyle ^= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;

					s_Style.Font.dwStyle |= FontStyleItalic;
				}

				if ((STRING("Bold Italic")) == ((CString&)pProperty->Value())) 
				{
					//Clear styles
					s_Style.Font.dwStyle |= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;
					s_Style.Font.dwStyle ^= FontStyleRegular | FontStyleBold | FontStyleItalic | FontStyleBoldItalic;

					s_Style.Font.dwStyle |= FontStyleBoldItalic;
				}

				s_bFontChanged = TRUE;
				break;
			}

		case CP_SIZE:				{ s_Style.Font.iSize = ((CInt&)pProperty->Value()).Value(); if (s_Style.Font.iSize <= 0) { s_Style.Font.iSize = 1000; } s_bFontChanged = TRUE; break; }
		case CP_LEADING:			{ s_Style.iLeading   = ((CInt&)pProperty->Value()).Value(); break; }
		case CP_TRACKING:			{ s_Style.iTracking  = ((CInt&)pProperty->Value()).Value(); break; }
	//	case CP_COLOR:				{ s_Style.Color      = ((CInt&)pProperty->Value()).Value(); s_bFontChanged = TRUE; break; }
		case CP_POSITION:			{ s_Style.ePosition  = ((CEnum&)pProperty->Value()).Value(); break; }
		case CP_KERNING_TYPE:		{ s_Style.eKerning   = ((CEnum&)pProperty->Value()).Value(); s_bFontChanged = TRUE; break; }
		case CP_CASE:				{ s_Style.eCase      = ((CEnum&)pProperty->Value()).Value(); break; }

		case CP_UNDERLINE_TYPE:		{ s_Style.Underline.eType  = ((CEnum&)pProperty->Value()).Value(); break; }
		case CP_UNDERLINE_WIDTH:	{ s_Style.Underline.iWidth = ((CInt&)pProperty->Value()).Value(); break; }
	//	case CP_UNDERLINE_COLOR:	{ s_Style.Underline.Color  = ((CInt&)pProperty->Value()).Value(); break; }
	
		case CP_STRIKETHROUGH_TYPE:	{ s_Style.StrikeThrough.eType  = ((CEnum&)pProperty->Value()).Value(); break; }
		case CP_STRIKETHROUGH_WIDTH:{ s_Style.StrikeThrough.iWidth = ((CInt&)pProperty->Value()).Value(); break; }
	//	case CP_STRIKETHROUGH_COLOR:{ s_Style.StrikeThrough.Color  = ((CInt&)pProperty->Value()).Value(); break; }
		}
	}

	return TRUE;
}


bool CCharacterStyleStack::UpdateStyleState()
{
	if (!s_bFontChanged) { return TRUE; }

	if (!CCache::pG)                    { return TRUE; }
	if (!CCache::pStandardStringFormat) { return TRUE; }
	if (!CCache::hDC)                   { return TRUE; }

	//Delete gdi+ objects
	if (CCache::pBrush) { delete CCache::pBrush; CCache::pBrush = NULL; }

	//Delete gdi objects
	if (CCache::hFont)
	{
		SelectObject(CCache::hDC, GetStockObject(SYSTEM_FONT));
		DeleteObject(CCache::hFont);
		CCache::hFont = NULL;
	}

	//Delete gdi+ objects
	if (CCache::pFont) { delete CCache::pFont; CCache::pFont = NULL; }

	//Create primary font family
	FontFamily* pFontFamily = new FontFamily(s_Style.Font.Family.Value());
	if (!pFontFamily) { SCFError(ErrorOutOfMemory); return FALSE; }

	//Check if such a family is available
	if (pFontFamily->GetLastStatus() != Ok)
	{
		delete pFontFamily;

		//If not, create alternate font family
		pFontFamily = new FontFamily(s_Style.Font.AlternateFamily.Value());
		if (!pFontFamily) { SCFError(ErrorOutOfMemory); return FALSE; }

		//Check if such a family is available
		if (pFontFamily->GetLastStatus() != Ok)
		{
			//If not, create default font
			CCache::pFont = new Font(FontFamily::GenericSansSerif(), s_Style.Font.iSize * 0.001f, s_Style.Font.dwStyle, UnitPoint);
			if (!CCache::pFont) { SCFError(ErrorOutOfMemory); return FALSE; }
		}
		else 
		{
			CCache::pFont = new Font(pFontFamily, s_Style.Font.iSize * 0.001f, s_Style.Font.dwStyle, UnitPoint); 
			if (!CCache::pFont) { SCFError(ErrorOutOfMemory); return FALSE; }
		}
	}
	else
	{
		CCache::pFont = new Font(pFontFamily, s_Style.Font.iSize * 0.001f, s_Style.Font.dwStyle, UnitPoint); 
		if (!CCache::pFont) { SCFError(ErrorOutOfMemory); return FALSE; }
	}

	delete pFontFamily;

	//Create a GDI font
	LOGFONT	logFont;
	CCache::pFont->GetLogFontW(CCache::pG, &logFont);

	logFont.lfHeight = (int)(s_Style.Font.iSize / 10);
	logFont.lfWidth = 0;

	CCache::hFont = CreateFontIndirect(&logFont);
	if (!CCache::hFont) { SCFError(ErrorOutOfMemory); return FALSE; }
	SelectObject(CCache::hDC, CCache::hFont);

	//Rebuild kerning table if necessary
	CRunState::PrepareKerning();

	//Obtain text metrics
	GetTextMetrics(CCache::hDC, &CCache::TextMetrics);

	CCache::OutlineMetrics.otmSize = sizeof(CCache::OutlineMetrics);
	GetOutlineTextMetrics(CCache::hDC, sizeof(CCache::OutlineMetrics), &CCache::OutlineMetrics);

	CCache::TextMetrics.tmHeight  = abs(CCache::TextMetrics.tmHeight);
	CCache::TextMetrics.tmAscent  = abs(CCache::TextMetrics.tmAscent);
	CCache::TextMetrics.tmDescent = abs(CCache::TextMetrics.tmDescent);

	//Create new brush
	CCache::pBrush = new SolidBrush(Color(SCF_RGBA_TO_ARGB(s_Style.Color)));
	if (!CCache::pBrush) { SCFError(ErrorOutOfMemory); return FALSE; }

	s_bFontChanged = FALSE;
	return TRUE;
}

