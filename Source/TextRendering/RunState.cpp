#include "Cache.h"
#include "RunState.h"
#include "CharacterStyleStack.h"
#include "Frame.h"

using namespace SCFTextRendering;

CRunState::tRunState CRunState::s_Run;

void CRunState::Initialize()
{
	//Reset run state
	s_Run.iWidth = 0;

	s_Run.iLeading = 0;
	s_Run.iDescent = 0;

	s_Run.iAscent = 0;

	s_Run.usCharPosCount = 16;
	s_Run.pCharPos = (PointF*)CMemory::Allocate(sizeof(PointF) * s_Run.usCharPosCount);

	s_Run.usKerningPairCount = 1024;
	s_Run.usKerningPairCountUsed = 0;
	s_Run.pKerningPair = (KERNINGPAIR*)CMemory::Allocate(sizeof(KERNINGPAIR) * s_Run.usKerningPairCount);
}

//Adjusts a given current character offset, by the kerning amount for the specified 2 characters
int CRunState::KerningOffset(_IN SCF::TCHAR cFirst, _IN SCF::TCHAR cSecond)
{
	if (!s_Run.usKerningPairCountUsed) { return 0; }

	switch (CCharacterStyleStack::Top().eKerning)
	{
	case KERNING_METRICS:
		{
			int	usCurPair = 0;
			do
			{
				//Compare character pair with kerning pairs
				if ((s_Run.pKerningPair[usCurPair].wFirst == cFirst) && (s_Run.pKerningPair[usCurPair].wSecond == cSecond))
				{
					//Update current character 
					return (int)s_Run.pKerningPair[usCurPair].iKernAmount * 10;
				}

			} while (++usCurPair < s_Run.usKerningPairCountUsed);

			return 0;
		}
	case KERNING_NONE: { return 0; }
	}

	return 0;
}

//Retrieves a new table of kerning pairs
bool CRunState::PrepareKerning()
{
	switch (CCharacterStyleStack::Top().eKerning)
	{
	case KERNING_METRICS:
		{
			while (s_Run.usKerningPairCount == (s_Run.usKerningPairCountUsed = (int)GetKerningPairs(CCache::hDC, s_Run.usKerningPairCount, s_Run.pKerningPair)))
			{
				s_Run.usKerningPairCount *= 2;
				s_Run.pKerningPair = (KERNINGPAIR*)CMemory::Reallocate(s_Run.pKerningPair, sizeof(KERNINGPAIR) * s_Run.usKerningPairCount);
			}

			return TRUE;
		}
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}
bool CRunState::MeasureRun(_INOUT SCRIPT_ANALYSIS* pScriptAnalysis, _IN SCF::LPTSTR sText, _IN int iLength, _OUT int* iOutWidth, _OUT int* iOutLeading, _OUT int* iOutDescent, _OUT int* iOutAscent, _OUT int* ipOutCharWidths)
{
	//Check arguments
	if (!pScriptAnalysis || !sText) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Don't process empty runs
	if (!iLength) { return TRUE; } 

	//Temporary variables used for string measurement
	int l_iTotalWidth = 0;
	int l_iScale = 1000;

	PointF l_PointF(0, 0);
	RectF l_RectF;

	switch (CCharacterStyleStack::Top().ePosition)
	{
	case POS_NORMAL: { break; }
	case POS_SUPERSCRIPT:	
		{
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUPERSCRIPT_SIZE, &l_iScale);

			//Apply temporary scale transformation
			CCache::pG->ScaleTransform(l_iScale * 0.001f, l_iScale * 0.001f);
			break; 
		}
	case POS_SUBSCRIPT:		
		{
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUBSCRIPT_SIZE, &l_iScale);

			//Apply temporary scale transformation
			CCache::pG->ScaleTransform(l_iScale * 0.001f, l_iScale * 0.001f);
			break; 
		}
	}

	//Resize text cache if necessary
	if (CCache::iMaxVisualTextLength < iLength)
	{
		CCache::iMaxVisualTextLength = iLength * 4;
		CCache::szVisualText = (SCF::TCHAR*)CMemory::Reallocate(CCache::szVisualText, sizeof(SCF::TCHAR) * (CCache::iMaxVisualTextLength + 1));
	}

	//Cache text
	wcsncpy_s(CCache::szVisualText, CCache::iMaxVisualTextLength + 1, sText, iLength);

	//Modify cached text prior to measuring
	if ((CCharacterStyleStack::Top().eCase == CASE_ALLCAPS) || (CCharacterStyleStack::Top().eCase == CASE_SMALLCAPS))	{ _wcsupr_s(CCache::szVisualText, CCache::iMaxVisualTextLength + 1); }
	if (CCharacterStyleStack::Top().eCase == CASE_ALLSMALL)											{ _wcslwr_s(CCache::szVisualText, CCache::iMaxVisualTextLength + 1); }

	if (CCharacterStyleStack::Top().eCase == CASE_SMALLCAPS)
	{
		int l_iSmallScale = 1000;
		CFrameStyleStack::QueryIntegerValue(FP_TEXT_SMALLCAPS_SIZE, &l_iSmallScale);

		//Measure string width - use custom tracking & kerning
		for (int iChar = 0; iChar < iLength; iChar++)
		{
			CCache::pG->MeasureString(&CCache::szVisualText[iChar], 1, CCache::pFont, l_PointF, CCache::pStandardStringFormat, &l_RectF);
			if (iswlower(sText[iChar])) { l_RectF.Width = (l_RectF.Width * l_iSmallScale) / 1000; }

			//Apply kerning offset for each character pair -- USE ORIGINAL STRING FOR SMALL CAPS
			if (iChar) { ipOutCharWidths[iChar - 1] += CRunState::KerningOffset(sText[iChar - 1], sText[iChar]); }

			//Add width of current character to the total width
			if (ipOutCharWidths) { ipOutCharWidths[iChar] = (l_iScale * ((int)(l_RectF.Width * 1000) + (CCharacterStyleStack::Top().iTracking * CCharacterStyleStack::Top().Font.iSize) / 1000)) / 1000; l_iTotalWidth += ipOutCharWidths[iChar]; }
			else                 { l_iTotalWidth         += (l_iScale * ((int)(l_RectF.Width * 1000) + (CCharacterStyleStack::Top().iTracking * CCharacterStyleStack::Top().Font.iSize) / 1000)) / 1000; }
		}
	}
	else
	{
		//Measure string width - use custom tracking & kerning
		for (int iChar = 0; iChar < iLength; iChar++)
		{
			CCache::pG->MeasureString(&CCache::szVisualText[iChar], 1, CCache::pFont, l_PointF, CCache::pStandardStringFormat, &l_RectF);

			//Apply kerning offset for each character pair
			if (iChar) { ipOutCharWidths[iChar - 1] += CRunState::KerningOffset(CCache::szVisualText[iChar - 1], CCache::szVisualText[iChar]); }

			//Add width of current character to the total width
			if (ipOutCharWidths) { ipOutCharWidths[iChar] = (l_iScale * ((int)(l_RectF.Width * 1000) + (CCharacterStyleStack::Top().iTracking * CCharacterStyleStack::Top().Font.iSize) / 1000)) / 1000; l_iTotalWidth += ipOutCharWidths[iChar]; }
			else                 { l_iTotalWidth         += (l_iScale * ((int)(l_RectF.Width * 1000) + (CCharacterStyleStack::Top().iTracking * CCharacterStyleStack::Top().Font.iSize) / 1000)) / 1000; }
		}
	}

	if (iOutWidth) { *iOutWidth	= l_iTotalWidth; }

	//Output full-ascent is an adjusted value, to compensate for leading - could actually become negative
	if (iOutLeading) { *iOutLeading	= (CCache::TextMetrics.tmHeight * CCharacterStyleStack::Top().iLeading) / 100 - CCache::TextMetrics.tmDescent * 10; }
	if (iOutDescent) { *iOutDescent	= CCache::TextMetrics.tmDescent * 10; }

	if (iOutAscent) { *iOutAscent = (CCache::OutlineMetrics.otmAscent + CCache::OutlineMetrics.otmMacAscent) * 5; }

	return TRUE;
}

bool CRunState::RenderRun(_INOUT SCRIPT_ANALYSIS* pScriptAnalysis, _IN SCF::LPTSTR sText, _IN int iLength, _IN int iX, _IN int iY, _IN int* ipCharWidths)
{
	//Check arguments
	if (!pScriptAnalysis || !sText) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Don't process empty runs
	if (!iLength) { return TRUE; } 

	//Resize character position array, if necessary
	if (iLength > s_Run.usCharPosCount)
	{
		s_Run.usCharPosCount = iLength + 10;
		s_Run.pCharPos = (PointF*)CMemory::Reallocate(s_Run.pCharPos, sizeof(PointF) * s_Run.usCharPosCount);
	}

	//Store transformation state
	GraphicsState StateBefore = CCache::pG->Save();

	int	iScale = 1000;
	int	iPositionOffset	= 0;

	switch (CCharacterStyleStack::Top().ePosition)
	{
	case POS_NORMAL: { break; }

	case POS_SUPERSCRIPT:
		{
			//Obtain sizing & positioning values
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUPERSCRIPT_SIZE,		&iScale);
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUPERSCRIPT_POSITION,	&iPositionOffset);

			//Change offset sign to shift subscript in the opposite direction as superscript
			iPositionOffset = -iPositionOffset;
			break;
		}
	case POS_SUBSCRIPT:
		{
			//Obtain sizing & positioning values
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUBSCRIPT_SIZE,		&iScale);
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUBSCRIPT_POSITION,	&iPositionOffset);

			break;
		}
	}

	//Compute initial position
	s_Run.pCharPos[0].X = (float)iX / iScale;
	s_Run.pCharPos[0].Y = (float)(iY + (CCharacterStyleStack::Top().Font.iSize * iPositionOffset) / 1000) / iScale;

	//Fill the character position array
	for (int iChar = 1; iChar < iLength; iChar++)
	{
		//Calculate character/glyph position
		s_Run.pCharPos[iChar].X = s_Run.pCharPos[iChar - 1].X + ((float)ipCharWidths[iChar - 1] / iScale);
		s_Run.pCharPos[iChar].Y = s_Run.pCharPos[iChar - 1].Y;
	}

	//Cache text
	wcsncpy_s(CCache::szVisualText, CCache::iMaxVisualTextLength + 1, sText, iLength);

	//Modify cached text prior to measuring
	if ((CCharacterStyleStack::Top().eCase == CASE_ALLCAPS) || (CCharacterStyleStack::Top().eCase == CASE_SMALLCAPS)) { _wcsupr_s(CCache::szVisualText, CCache::iMaxVisualTextLength + 1); }
	if (CCharacterStyleStack::Top().eCase == CASE_ALLSMALL) { _wcslwr_s(CCache::szVisualText, CCache::iMaxVisualTextLength + 1); }

	//Perform temporary scaling
	CCache::pG->ScaleTransform(iScale * 0.001f, iScale * 0.001f);

	if (CCharacterStyleStack::Top().eCase == CASE_SMALLCAPS)
	{
		int iSmallScale = 1000;
		CFrameStyleStack::QueryIntegerValue(FP_TEXT_SMALLCAPS_SIZE, &iSmallScale);

		for (int iChar = 0; iChar < iLength; iChar++)
		{
			if (iswlower(sText[iChar]))
			{
				//Store transformation state
				GraphicsState StateBefore = CCache::pG->Save();

				s_Run.pCharPos[iChar].X /= iSmallScale * 0.001f;
				s_Run.pCharPos[iChar].Y /= iSmallScale * 0.001f;

				//Perform temporary scaling
				CCache::pG->ScaleTransform(iSmallScale * 0.001f, iSmallScale * 0.001f);

				CCache::pG->DrawDriverString((UINT16*)&CCache::szVisualText[iChar], 1, CCache::pFont, CCache::pBrush, &s_Run.pCharPos[iChar], DriverStringOptionsCmapLookup, NULL);

				//Restore transformation state
				CCache::pG->Restore(StateBefore);
			}
			else { CCache::pG->DrawDriverString((UINT16*)&CCache::szVisualText[iChar], 1, CCache::pFont, CCache::pBrush, &s_Run.pCharPos[iChar], DriverStringOptionsCmapLookup, NULL); }
		}
	}
	else { CCache::pG->DrawDriverString((UINT16*)CCache::szVisualText, iLength, CCache::pFont, CCache::pBrush, s_Run.pCharPos, DriverStringOptionsCmapLookup, NULL); }

	if ((CCharacterStyleStack::Top().Underline.eType >= US_NORMAL) && (CCharacterStyleStack::Top().Underline.eType <= US_DASH_DOT_DOT))
	{
		Pen pen(Color(SCF_RGBA_TO_ARGB(CCharacterStyleStack::Top().Underline.Color)));

		pen.SetWidth(CCharacterStyleStack::Top().Underline.iWidth * 0.001f);

		switch (CCharacterStyleStack::Top().Underline.eType)
		{
		case US_DASHED:       { pen.SetDashStyle(DashStyleDash);       break; }
		case US_DOTTED:       { pen.SetDashStyle(DashStyleDot);        break; }
		case US_DASH_DOT:     { pen.SetDashStyle(DashStyleDashDot);    break; }
		case US_DASH_DOT_DOT: { pen.SetDashStyle(DashStyleDashDotDot); break; }
		}

		CCache::pG->DrawLine(&pen, s_Run.pCharPos[0].X, s_Run.pCharPos[0].Y, s_Run.pCharPos[iLength - 1].X + ipCharWidths[iLength - 1] * 0.001f, s_Run.pCharPos[0].Y);
	}

	if ((CCharacterStyleStack::Top().StrikeThrough.eType >= US_NORMAL) && (CCharacterStyleStack::Top().StrikeThrough.eType <= US_DASH_DOT_DOT))
	{
		Pen pen(Color(SCF_RGBA_TO_ARGB(CCharacterStyleStack::Top().StrikeThrough.Color)));

		pen.SetWidth(CCharacterStyleStack::Top().StrikeThrough.iWidth * 0.001f);

		switch (CCharacterStyleStack::Top().StrikeThrough.eType)
		{
		case US_DASHED:       { pen.SetDashStyle(DashStyleDash);       break; }
		case US_DOTTED:       { pen.SetDashStyle(DashStyleDot);        break; }
		case US_DASH_DOT:     { pen.SetDashStyle(DashStyleDashDot);    break; }
		case US_DASH_DOT_DOT: { pen.SetDashStyle(DashStyleDashDotDot); break; }
		}

		CCache::pG->DrawLine(&pen, s_Run.pCharPos[0].X, s_Run.pCharPos[0].Y - (CCache::OutlineMetrics.otmsStrikeoutPosition * 0.01f), s_Run.pCharPos[iLength - 1].X + ipCharWidths[iLength - 1] * 0.001f, s_Run.pCharPos[0].Y - (CCache::OutlineMetrics.otmsStrikeoutPosition * 0.01f));
	}

	//Restore transformation state
	CCache::pG->Restore(StateBefore);

	return TRUE;
}

bool CRunState::RenderWhitespaceRun(_IN int iX, _IN int iY, _IN int* ipCharWidths, _IN int iLength)
{
	//Don't process empty runs
	if (!iLength) { return TRUE; } 

	//Resize character position array, if necessary
	if (iLength > s_Run.usCharPosCount)
	{
		s_Run.usCharPosCount = iLength + 10;
		s_Run.pCharPos = (PointF*)CMemory::Reallocate(s_Run.pCharPos, sizeof(PointF) * s_Run.usCharPosCount);
	}

	//Store transformation state
	GraphicsState StateBefore = CCache::pG->Save();

	int	iScale = 1000;
	int	iPositionOffset	= 0;

	switch (CCharacterStyleStack::Top().ePosition)
	{
	case POS_NORMAL: { break; }

	case POS_SUPERSCRIPT:
		{
			//Obtain sizing & positioning values
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUPERSCRIPT_SIZE,		&iScale);
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUPERSCRIPT_POSITION,	&iPositionOffset);

			//Change offset sign to shift subscript in the opposite direction as superscript
			iPositionOffset = -iPositionOffset;
			break;
		}
	case POS_SUBSCRIPT:
		{
			//Obtain sizing & positioning values
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUBSCRIPT_SIZE,		&iScale);
			CFrameStyleStack::QueryIntegerValue(FP_TEXT_SUBSCRIPT_POSITION,	&iPositionOffset);

			break;
		}
	}

	//Compute initial position
	s_Run.pCharPos[0].X = (float)iX / iScale;
	s_Run.pCharPos[0].Y = (float)(iY + (CCharacterStyleStack::Top().Font.iSize * iPositionOffset) / 1000) / iScale;

	//Fill the character position array
	for (int iChar = 1; iChar < iLength; iChar++)
	{
		//Calculate character/glyph position
		s_Run.pCharPos[iChar].X = s_Run.pCharPos[iChar - 1].X + ((float)ipCharWidths[iChar - 1] / iScale);
		s_Run.pCharPos[iChar].Y = s_Run.pCharPos[iChar - 1].Y;
	}

	//Perform temporary scaling
	CCache::pG->ScaleTransform(iScale * 0.001f, iScale * 0.001f);

	if ((CCharacterStyleStack::Top().Underline.eType >= US_NORMAL) && (CCharacterStyleStack::Top().Underline.eType <= US_DASH_DOT_DOT))
	{
		Pen pen(Color(SCF_RGBA_TO_ARGB(CCharacterStyleStack::Top().Underline.Color)));

		pen.SetWidth(CCharacterStyleStack::Top().Underline.iWidth * 0.001f);

		switch (CCharacterStyleStack::Top().Underline.eType)
		{
		case US_DASHED:       { pen.SetDashStyle(DashStyleDash);       break; }
		case US_DOTTED:       { pen.SetDashStyle(DashStyleDot);        break; }
		case US_DASH_DOT:     { pen.SetDashStyle(DashStyleDashDot);    break; }
		case US_DASH_DOT_DOT: { pen.SetDashStyle(DashStyleDashDotDot); break; }
		}

		CCache::pG->DrawLine(&pen, s_Run.pCharPos[0].X, s_Run.pCharPos[0].Y, s_Run.pCharPos[iLength - 1].X + ipCharWidths[iLength - 1] * 0.001f, s_Run.pCharPos[0].Y);
	}

	if ((CCharacterStyleStack::Top().StrikeThrough.eType >= US_NORMAL) && (CCharacterStyleStack::Top().StrikeThrough.eType <= US_DASH_DOT_DOT))
	{
		Pen pen(Color(SCF_RGBA_TO_ARGB(CCharacterStyleStack::Top().StrikeThrough.Color)));

		pen.SetWidth(CCharacterStyleStack::Top().StrikeThrough.iWidth * 0.001f);

		switch (CCharacterStyleStack::Top().StrikeThrough.eType)
		{
		case US_DASHED:       { pen.SetDashStyle(DashStyleDash);       break; }
		case US_DOTTED:       { pen.SetDashStyle(DashStyleDot);        break; }
		case US_DASH_DOT:     { pen.SetDashStyle(DashStyleDashDot);    break; }
		case US_DASH_DOT_DOT: { pen.SetDashStyle(DashStyleDashDotDot); break; }
		}

		CCache::pG->DrawLine(&pen, s_Run.pCharPos[0].X, s_Run.pCharPos[0].Y - (CCache::OutlineMetrics.otmsStrikeoutPosition * 0.01f), s_Run.pCharPos[iLength - 1].X + ipCharWidths[iLength - 1] * 0.001f, s_Run.pCharPos[0].Y - (CCache::OutlineMetrics.otmsStrikeoutPosition * 0.01f));
	}

	//Restore transformation state
	CCache::pG->Restore(StateBefore);

	return TRUE;
}