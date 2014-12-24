#include "Frame.h"
#include "Cache.h"

#include "CharacterStyleStack.h"
#include "ParagraphStyleStack.h"

using namespace Gdiplus;
using namespace SCFBase;
using namespace SCFGraphics;
using namespace SCFTextRendering;

CFrame::CFrame()
{
	//Default styles for frame contents
	m_pStyleCharacter = NULL;
	m_pStyleParagraph = NULL;

	//Frame style
	m_pStyle = NULL;

	m_uiParagraphVisibleCount = 0;

	//Reset text-frame layout/bounding rectangle
	m_Rect.iX = 0;
	m_Rect.iY = 0;

	m_Rect.iWidth  = 0;
	m_Rect.iHeight = 0;

	m_iColumnCount = 1;

	m_iTextLength = 0;

	m_dwRenderOptions = RO_QUALITY_NORMAL;

	m_iCharactersLaidOut      = 0;
	m_iFormattingCharsLaidOut = 0;
	m_iParagraphsLaidOut      = 0;
	m_iWordsLaidOut           = 0;

	m_bTextChanged = TRUE;
}

CFrame::~CFrame()
{
	//Clear paragraph list
	m_Paragraphs.AllDelete();
	m_Columns.AllDelete();
}

bool CFrame::Text(_IN CString& rText) 
{
	//Clear paragraph list
	m_Paragraphs.AllDelete();

	m_uiParagraphVisibleCount = 0;
	m_bTextChanged = TRUE;

	//Class pointer which is used to divide text into paragraphs
	CParagraph*	pNewParagraph = NULL;

	bool l_bExplicit = FALSE;
	int iStart = 0; 
	int iEnd = 0;

	m_iTextLength = rText.Length();

	//Don't process empty strings
	if (!m_iTextLength) { return TRUE; }

	//Cycle, which divides the text into paragraphs
	do
	{
		CStringRange  StringRange(rText, iStart, m_iTextLength - iStart);
		CStringSearch StringSearch(StringRange);

		//Search for the sub-strings that specify new paragraphs
		int iIndex = StringSearch.FindString(g_NewParagraph);
	//	int iIndex = FindSubStrings((SCF::LPTSTR)&szText[iStart], m_iTextLength - iStart, &g_szNewParagraph, 1, NULL);

		//Compute new ending character position for the new paragraph
		if (iIndex == -1) { iEnd = m_iTextLength; }
		else              { iEnd = iStart + iIndex; }

		//Create new paragraph (starting character position is set in the previous cycle, or before the cycle starts) 
		pNewParagraph = new CParagraph(l_bExplicit);
		pNewParagraph->Text(CStringRange(rText, iStart, iEnd - iStart));

		//Store information about the length of the formatting string used to specify this paragraph
		pNewParagraph->FormattingOverhead(g_NewParagraph.Length());

		//Store new paragraph
		m_Paragraphs.LastAdd(*pNewParagraph);

		//Compute starting character position for the next paragraph - skip formatting string
		iStart = iEnd + g_NewParagraph.Length();

		//The second & later paragraphs are always explicit
		l_bExplicit = TRUE;
	}
	while (iStart <= m_iTextLength);

	return TRUE;
}

void CFrame::Rect(_IN SCFGraphics::Rect4i& rRect) _SET
{
	//Copy left-top point
	m_Rect.iX = rRect.iX;
	m_Rect.iY = rRect.iY;

	//Copy rectangle dimensions
	m_Rect.iWidth  = rRect.iWidth;
	m_Rect.iHeight = rRect.iHeight;
}

SCFGraphics::Rect4i& CFrame::Rect() _GET
{
	return (SCFGraphics::Rect4i&)m_Rect;
}

void                 CFrame::Insets(_IN SCFGraphics::Rect4i& rInsets) _SET { CFrameStyleStack::Top().Inset = rInsets; }
SCFGraphics::Rect4i& CFrame::Insets()                                 _GET { return CFrameStyleStack::Top().Inset; } 

bool CFrame::ApplyStyle(_IN CStyle& rStyle, _IN int iStartPos, _IN int iLength)
{
	if (rStyle.Type() == STYLE_TEXTFRAME)
	{
		m_pStyle = &rStyle;
		return TRUE;
	}

	int l_iCurChar = 0;

	//Go trough all paragraphs
	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		CParagraph* pParagraph = &(CParagraph&)m_Paragraphs[i];

		//Skip paragraphs that the new style will not affect
		if ((iLength == -1) && (iStartPos >= (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead()))) 
		{
			//Prepare starting char for next paragraph
			l_iCurChar += pParagraph->TextLength();
			l_iCurChar += pParagraph->FormattingOverhead();
			continue; 
		}
		if ((iLength > 0)	&& ((iStartPos + iLength <= l_iCurChar) || (iStartPos >= (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead()))))
		{
			//Prepare starting char for next paragraph
			l_iCurChar += pParagraph->TextLength();
			l_iCurChar += pParagraph->FormattingOverhead();
			continue; 
		}

		//Find the paragraph, to which the style PUSH entry belongs 
		if ((iStartPos >= l_iCurChar) && (iStartPos < (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead())))
		{
			//Set a PUSH style entry
			pParagraph->StyleEntry(rStyle, iStartPos - l_iCurChar, TRUE);

			//Quit, if there is no POP entry
			if (iLength == -1) { return TRUE; }
		}

		//If there is a POP entry..
		if (iLength > 0)
		{
			//..find the paragraph, to which the style POP entry belongs 
			if (((iStartPos + iLength - 1) >= l_iCurChar) && ((iStartPos + iLength - 1) < (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead())))
			{
				//Set a POP style entry, add 1 to place pop entry after last character 
				pParagraph->StyleEntry(rStyle, (iStartPos + iLength - 1) - l_iCurChar + 1, FALSE);
				return TRUE;
			}
		}

		//Prepare starting char for next paragraph
		l_iCurChar += pParagraph->TextLength();
		l_iCurChar += pParagraph->FormattingOverhead();
	}

	SCFError(ErrorUnexpectedCodeExecution); return FALSE;
}

bool CFrame::ClearStyle(_IN CStyle& rStyle, _IN int iStartPos, _IN int iLength)
{
	//Check arguments
	if (iLength < -1) { return FALSE; }

	if (rStyle.Type() == STYLE_TEXTFRAME)
	{
		m_pStyle = NULL;
		return TRUE;
	}

	int l_iCurChar = 0;

	//Go trough all paragraphs
	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		CParagraph* pParagraph = &(CParagraph&)m_Paragraphs[i];

		//Skip paragraphs that the new style delete will not affect
		if ((iLength == -1) && (iStartPos >= (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead()))) 
		{
			//Prepare starting char for next paragraph
			l_iCurChar += pParagraph->TextLength();
			l_iCurChar += pParagraph->FormattingOverhead();

			continue; 
		}
		if ((iLength > 0)	&& ((iStartPos + iLength <= l_iCurChar) || (iStartPos >= (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead()))))
		{
			//Prepare starting char for next paragraph
			l_iCurChar += pParagraph->TextLength();
			l_iCurChar += pParagraph->FormattingOverhead();

			continue; 
		}

		//Compute absolute start & end point !!!! IF [iLength] is negative, [iClearTo] always becomes [l_iCurChar + pParagraph->TextLength()], that is the end pos fo the current paragraph
		int iClearFrom = __max(l_iCurChar, iStartPos);
		int iClearTo   = __min(l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead(), iStartPos + (int)iLength);

		//If there is nothing more to clean
		if ((iLength > 0) && (iStartPos + iLength) < l_iCurChar) { return TRUE; }

		for (int iCharToClear = (iClearFrom - l_iCurChar); iCharToClear <= (iClearTo - iClearFrom); iCharToClear++)	
		{
			//Clear all style entries at the given position
			pParagraph->StyleEntryClear(rStyle, iCharToClear, TRUE, TRUE);
		}

		//Prepare starting char for next paragraph
		l_iCurChar += pParagraph->TextLength();
		l_iCurChar += pParagraph->FormattingOverhead();
	}

	SCFError(ErrorUnexpectedCodeExecution); return FALSE;
}

bool CFrame::ClearAllStyles(_IN int iStartPos, _IN int iLength, _IN SCF::ENUM eType)
{
	//Check arguments
	if (iLength == 0) { return TRUE; }
	if (iLength < -1) { return FALSE; }

	if (eType == STYLE_TEXTFRAME)
	{
		m_pStyle = NULL; 
		return TRUE;
	}

	int l_iCurChar = 0;

	//Go trough all paragraphs
	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		CParagraph* pParagraph = &(CParagraph&)m_Paragraphs[i];

		//Skip paragraphs that the new style delete will not affect
		if ((iLength == -1) && (iStartPos >= (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead()))) 
		{
			//Prepare starting char for next paragraph
			l_iCurChar += pParagraph->TextLength();
			l_iCurChar += pParagraph->FormattingOverhead();

			continue; 
		}
		if ((iLength > 0)	&& ((iStartPos + iLength <= l_iCurChar) || (iStartPos >= (l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead()))))
		{
			//Prepare starting char for next paragraph
			l_iCurChar += pParagraph->TextLength();
			l_iCurChar += pParagraph->FormattingOverhead();

			continue; 
		}

		//Compute absolute start & end point !!!! IF [iLength] is negative, [iClearTo] always becomes [l_iCurChar + pParagraph->TextLength()], that is the end pos fo the current paragraph
		int iClearFrom	= __max(l_iCurChar, iStartPos);
		int iClearTo		= __min(l_iCurChar + pParagraph->TextLength() + pParagraph->FormattingOverhead(), iStartPos + (int)iLength);

		//If there is nothing more to clean
		if ((iLength > 0)	&& (iStartPos + iLength) < l_iCurChar) { return TRUE; }

		for (int iCharToClear = (iClearFrom - l_iCurChar); iCharToClear <= (iClearTo - iClearFrom); iCharToClear++)	
		{
			//Clear all style entries at the given position
			pParagraph->StyleEntriesClear(iCharToClear, TRUE, TRUE, eType);
		}

		//Prepare starting char for next paragraph
		l_iCurChar += pParagraph->TextLength();
		l_iCurChar += pParagraph->FormattingOverhead();
	}

	return TRUE;
}

bool CFrame::DefaultStyle(_IN CStyle& rStyle)
{
	switch (rStyle.Type())
	{
	case STYLE_CHARACTER:	{ m_pStyleCharacter = &rStyle; return TRUE; }
	case STYLE_PARAGRAPH:	{ m_pStyleCharacter = &rStyle; return TRUE; }
	}
	SCFError(ErrorInvalidArgument); return FALSE;
}

CStyle* CFrame::DefaultStyle(_IN SCF::ENUM eType)
{
	switch (eType)
	{
	case STYLE_CHARACTER: { return (CStyle*)m_pStyleCharacter; }
	case STYLE_PARAGRAPH: { return (CStyle*)m_pStyleParagraph; }
	}
	return 0;
}

bool CFrame::ClearDefaultStyle(_IN SCF::ENUM eType)
{
	switch (eType)
	{
	case STYLE_CHARACTER:	{ m_pStyleCharacter = NULL; return TRUE; }
	case STYLE_PARAGRAPH:	{ m_pStyleParagraph = NULL; return TRUE; }
	}
	SCFError(ErrorInvalidArgument); return FALSE;
}

bool CFrame::LayOut(_IN HDC hDC, _IN UpdateOptions dwUpdateOptions, _IN bool bNewObjects, _IN SCF::DWORD dwLayoutOptions)
{
	if (dwUpdateOptions & UO_UPDATE_LAYOUT)
	{
		CWord::s_iCharactersLaidOut = 0;
		CWord::s_iFormattingCharsLaidOut = 0;

		CWord::s_iWordsLaidOut = 0;
		CParagraph::s_iParagraphsLaidOut = 0;

		if (!PrepareForLayout(hDC, bNewObjects)) { SCFError(ErrorOutOfMemory); return FALSE; }

		for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
		{
			((CParagraph&)m_Paragraphs[i]).ResetLayout();
		}

		m_Columns.AllDelete();

		//Prepare variables
		SCFGraphics::Rect4i	curRect;

		bool bStop = FALSE;
		bool bContinueLayout = FALSE;

		SCF::DWORD dwFirstBaseline = LO_FIRST_BASELINE;
		int iColumnWidth = 0;

		SCF::DWORD dwParagraphLayoutResults = NULL;
		bool bLayoutResult = 0;

		//Set the width of the columns based on current style
		if (CFrameStyleStack::Top().Column.bFixedWidth)	
		{ 
			iColumnWidth = CFrameStyleStack::Top().Column.iWidth; 
			m_iColumnCount = 0;

			//Compute the number of columns that will fit into the text-frame rectangle
			int fCurrentWidth = 0;

			while ((fCurrentWidth + CFrameStyleStack::Top().Column.iWidth) < (m_Rect.iWidth - (CFrameStyleStack::Top().Inset.iX +  CFrameStyleStack::Top().Inset.iWidth)))
			{
				fCurrentWidth += CFrameStyleStack::Top().Column.iWidth + CFrameStyleStack::Top().Column.iGutter;
				m_iColumnCount++;
			}
		}
		else								
		{
			iColumnWidth = ((m_Rect.iWidth - (CFrameStyleStack::Top().Inset.iX +  CFrameStyleStack::Top().Inset.iWidth)) - (CFrameStyleStack::Top().Column.iGutter * (__max(0, CFrameStyleStack::Top().Column.iCount - 1)))) / CFrameStyleStack::Top().Column.iCount; 
			m_iColumnCount = CFrameStyleStack::Top().Column.iCount;
		}

		SCF::UINT uiParagraph = 0;

		//Go trough all columns
		for (int iColumn = 0; iColumn < m_iColumnCount; iColumn++)
		{
			dwFirstBaseline = LO_FIRST_BASELINE;

			//Prepare paragraph layout/bounding rectangle
			curRect.iX     = (iColumnWidth * iColumn) + (CFrameStyleStack::Top().Column.iGutter * iColumn) + (m_Rect.iX + CFrameStyleStack::Top().Inset.iX);
			curRect.iWidth = iColumnWidth;

			//Fixed height & position
			curRect.iY      = m_Rect.iY + CFrameStyleStack::Top().Inset.iY;
			curRect.iHeight = m_Rect.iHeight - (CFrameStyleStack::Top().Inset.iY + CFrameStyleStack::Top().Inset.iHeight);

			//Create a new column structure & fill in the rectangle information
			CColumn* pCurColumn = new CColumn();
			pCurColumn->BoundingBox(curRect);

			//Go trough all or the rest of the paragraphs
			while (!bStop && (uiParagraph < m_Paragraphs.Size()))
			{
				SCF::DWORD dwAdditionalLayoutOptions = m_bTextChanged ? NULL : LO_SKIP_CHARACTER_MEASUREMENT;

				//Decide whether to apply space before & after the paragraph [PP_SPACE_BEFORE, PP_SPACE_AFTER]
				if (!pCurColumn->VisualParts().Size())    { dwAdditionalLayoutOptions |= LO_IGNORE_SPACE_BEFORE; }
				if ((++uiParagraph) == m_Paragraphs.Size()) { dwAdditionalLayoutOptions |= LO_IGNORE_SPACE_AFTER; }
				uiParagraph--;

				if (!CFrameStyleStack::Top().bIgnoreLineLastDescent)
				{
					dwAdditionalLayoutOptions |= LO_FIT_USING_DESCENT;
				}

				if (CFrameStyleStack::Top().bContinueParagraph && (uiParagraph == 0) && !bContinueLayout)
				{
											bLayoutResult = ((CParagraph&)m_Paragraphs[uiParagraph]).LayOut(&curRect, dwFirstBaseline | dwLayoutOptions | dwAdditionalLayoutOptions | LO_AS_CONTINUED, CFrameStyleStack::Top().eFirstBaseline, CFrameStyleStack::Top().iFirstBaselineOffset, &dwParagraphLayoutResults);
				}
				else
				{
					//Lay out current paragraph
					if (bContinueLayout) {	bLayoutResult = ((CParagraph&)m_Paragraphs[uiParagraph]).LayOut(&curRect, dwFirstBaseline | dwLayoutOptions | dwAdditionalLayoutOptions | LO_CONTINUE,     CFrameStyleStack::Top().eFirstBaseline, CFrameStyleStack::Top().iFirstBaselineOffset, &dwParagraphLayoutResults); }	
					else {					bLayoutResult = ((CParagraph&)m_Paragraphs[uiParagraph]).LayOut(&curRect, dwFirstBaseline | dwLayoutOptions | dwAdditionalLayoutOptions,                   CFrameStyleStack::Top().eFirstBaseline, CFrameStyleStack::Top().iFirstBaselineOffset, &dwParagraphLayoutResults); }
				}

				dwFirstBaseline = NULL;

				//Stop if failed to properly process paragraph
				if (!bLayoutResult) { bStop = TRUE; break; }

				//Create a new column entry
				CColumnVisualPart* pColumnEntry = new CColumnVisualPart((CParagraph&)m_Paragraphs[uiParagraph], (int)((CParagraph&)m_Paragraphs[uiParagraph]).VisualParts().Size() - 1);

				//Add new column entry to the list of paragraph visual parts, that belong to this column
				if (dwParagraphLayoutResults & LR_NOTHING_FINISHED)
				{
					((CParagraph&)m_Paragraphs[uiParagraph]).DeleteLastVisualPart();

					delete pColumnEntry;

					if (pCurColumn->VisualParts().Size())
					{
						pCurColumn->VisualPartLast().VisualPart().SubtractSpaceAfter();
					}
				}
				else
				{
					pCurColumn->VisualParts().LastAdd(*pColumnEntry);
				}

				//Stop layout process, if layout was aborted
				if (dwParagraphLayoutResults & LR_LAYOUT_ABORTED) { bStop = TRUE; }

				//Break out of the cycle & skip variable updating, so that the rest of current paragraph is laid out into the next column (if there is one ..)
				if (!bStop && ((dwParagraphLayoutResults & LR_PARTIALY_FINISHED) || (dwParagraphLayoutResults & LR_NOTHING_FINISHED))) 
				{
					//(*paragraph_I)->SubtractLineLastDescent();

					//If we are attempting to split the last visible paragraph, we must increment the [paragraph_I] variable here, so that it does not disappear
					if (((iColumn + 1) == m_iColumnCount)) 
					{ 
						uiParagraph++; 
						bStop = TRUE; 
					}

					bContinueLayout = TRUE;
					break;
				}

				//Obtain the rectangle from the latest layout
				if (((CParagraph&)m_Paragraphs[uiParagraph]).VisualParts().Size())
				{
					//Move vertically down the column
					curRect.iY		+= ((CParagraph&)m_Paragraphs[uiParagraph]).VisualPartLast().Height();
					curRect.iHeight	-= ((CParagraph&)m_Paragraphs[uiParagraph]).VisualPartLast().Height();
				}

				bContinueLayout = FALSE;

				//Prepare variables for next paragraph
				uiParagraph++;
			}

			//Add column to the column list
			m_Columns.LastAdd(*pCurColumn);
		}

		if (CFrameStyleStack::Top().bIgnoreLineLastDescent)
		{
			for (SCF::UINT j = 0; j < m_Columns.Size(); j++)
			{
				if (((CColumn&)m_Columns[j]).VisualParts().Size())
				{
					((CColumn&)m_Columns[j]).VisualPartLast().VisualPart().SubtractLineLastDescent();
				}
			}
		}

		//Go trough the rest of the paragraphs (those that did not fit into the frame) & reset their layout to prevent any anomalies in rendering from previous layout 
		for (SCF::UINT i = uiParagraph; i < m_Paragraphs.Size(); i++)
		{
			((CParagraph&)m_Paragraphs[uiParagraph]).ResetLayout();
		}

		//Store iterator used during rendering to render only visible paragraphs
		m_uiParagraphVisibleCount = uiParagraph;

		//Update statistics
		m_iCharactersLaidOut      = CWord::s_iCharactersLaidOut;
		m_iFormattingCharsLaidOut = CWord::s_iFormattingCharsLaidOut;
		m_iWordsLaidOut           = CWord::s_iWordsLaidOut;
		m_iParagraphsLaidOut      = CParagraph::s_iParagraphsLaidOut;

		//If the last paragraph was not laid out, correct statistics
		if (dwParagraphLayoutResults & LR_NOTHING_FINISHED)  { m_iParagraphsLaidOut--; }
	}

	if (dwUpdateOptions & UO_UPDATE_SELECTION)
	{
		CCache::SelectionRectList.AllDelete();

		//Turn the character ranges into visual rectangles for rendering
		for (SCF::UINT i = 0; i < m_Selections.Size(); i++)
		{
			RealizeSelection(((CSelection&)m_Selections[i]).iStartChar, ((CSelection&)m_Selections[i]).iLength);
		}
	}

	AlignVertically();

	CleanAfterLayout();

	m_bTextChanged = FALSE;
	return TRUE;
}

bool CFrame::AlignVertically()
{
	for (SCF::UINT i = 0; i < m_Columns.Size(); i++)
	{
		CColumn* pColumn = &(CColumn&)m_Columns[i];

		SCFGraphics::Rect4i	l_CurParagraphRect;
		int l_iTotalHeight = 0;
		int l_iOffsetY = 0;
		int l_iNonEmptyPartCount = 0;

		for (SCF::UINT j = 0; j < pColumn->VisualParts().Size(); j++)
		{
			if (pColumn->VisualPart(i).VisualPart().LineCount() > 0)
			{
				//list<CColumnVisualPart*>::iterator tmpEntry_I = entry_I;
				//tmpEntry_I++;

				l_CurParagraphRect = pColumn->VisualPart(i).VisualPart().BoundingBox();
				l_iTotalHeight += l_CurParagraphRect.iHeight;
				l_iNonEmptyPartCount++;
			}
		}

		if (CFrameStyleStack::Top().eAlignVertical & ALIGN_JUSTIFY_VERTICAL)
		{
			if (l_iNonEmptyPartCount == 1)
			{
				switch (CFrameStyleStack::Top().eAlignVertical)
				{
				case ALIGN_JUSTIFY_VERTICAL | ALIGN_CENTER_VERTICAL:	{ l_iOffsetY = (pColumn->Height() - l_iTotalHeight) / 2; break; }
				case ALIGN_JUSTIFY_VERTICAL | ALIGN_BOTTOM:				{ l_iOffsetY =  pColumn->Height() - l_iTotalHeight;      break; }
				}
				pColumn->VisualPart(0).VisualPart().TranslateY(l_iOffsetY);
			}

			if (l_iNonEmptyPartCount > 1)
			{
				int l_iStepp = (pColumn->Height() - l_iTotalHeight) / (l_iNonEmptyPartCount - 1);

				//Move the rest of the parts downwards
				for (SCF::UINT j = 0; j < m_Columns.Size(); j++)
				{
					if (pColumn->VisualPart(j).VisualPart().LineCount() > 0)
					{
						pColumn->VisualPart(j).VisualPart().TranslateY(l_iOffsetY);
						l_iOffsetY += l_iStepp;
					}
				}
			}
		}
		else
		{
			switch (CFrameStyleStack::Top().eAlignVertical)
			{
			case ALIGN_CENTER_VERTICAL:	{ l_iOffsetY = (pColumn->Height() - l_iTotalHeight) / 2;	break; }
			case ALIGN_BOTTOM:			{ l_iOffsetY =  pColumn->Height() - l_iTotalHeight;		break; }
			}

			for (SCF::UINT j = 0; j < pColumn->VisualParts().Size(); j++)
			{
				pColumn->VisualPart(j).VisualPart().TranslateY(l_iOffsetY);
			}
		}
	}

	return TRUE;
}

bool CFrame::PrepareForLayout(_IN HDC hDC, _IN bool bNewObjects)
{
	if (bNewObjects)
	{
		//Store GDI hdc
		CCache::hDC = hDC;

		if (CCache::pG) { delete CCache::pG; CCache::pG = NULL; }

		//Prepare GDI+ [Graphics] object
		CCache::pG = new Graphics(hDC);
		if (!CCache::pG) { SCFError(ErrorOutOfMemory); return FALSE; }
	}

	//Flush all style stacks
	CFrameStyleStack::FlushStyleStack();
	CParagraphStyleStack::FlushStyleStack();
	CCharacterStyleStack::FlushStyleStack();
	
	//Reset all style states
	//ResetStyleState();
	CParagraphStyleStack::ResetStyleState();
	CCharacterStyleStack::ResetStyleState();

	//Prepare style state
	if (m_pStyle)
	{
		//Execute text-frame style
		CFrameStyleStack::PushStyleStack();
		CFrameStyleStack::ExecuteStyle(*m_pStyle);
	}

	if (m_pStyleParagraph)
	{
		//Execute paragraph style
		CParagraphStyleStack::PushStyleStack(*m_pStyleParagraph);
		CParagraphStyleStack::ExecuteStyle  (*m_pStyleParagraph);
	}

	if (m_pStyleCharacter)
	{
		//Execute paragraph style
		CCharacterStyleStack::PushStyleStack(*m_pStyleCharacter);
		CCharacterStyleStack::ExecuteStyle  (*m_pStyleCharacter);
	}

	CCache::pG->SetPageUnit(UnitPoint);
	CCache::pG->SetPageScale(1.0f);

	switch (m_dwRenderOptions & RO_QUALITY_PRINT)
	{
	case RO_QUALITY_DRAFT:
		{
			CCache::pG->SetTextRenderingHint(TextRenderingHintSingleBitPerPixel);
			CCache::pG->SetSmoothingMode(SmoothingModeNone); 
			CCache::pG->SetCompositingQuality(CompositingQualityHighSpeed); 
			CCache::pG->SetPixelOffsetMode(PixelOffsetModeNone); 
			break;
		}
	default:
	case RO_QUALITY_NORMAL:
		{
			CCache::pG->SetTextRenderingHint(TextRenderingHintAntiAlias);
			CCache::pG->SetSmoothingMode(SmoothingModeHighSpeed);
			CCache::pG->SetCompositingQuality(CompositingQualityHighSpeed); 
			CCache::pG->SetPixelOffsetMode(PixelOffsetModeHighSpeed); 
			CCache::pG->SetTextContrast(7); //10 - for whiter text, 0 - for darker
			break;
		}
	case RO_QUALITY_PRINT:
		{
			CCache::pG->SetTextRenderingHint(TextRenderingHintAntiAlias);
			CCache::pG->SetSmoothingMode(SmoothingModeHighQuality);
			CCache::pG->SetCompositingQuality(CompositingQualityHighQuality);
			CCache::pG->SetPixelOffsetMode(PixelOffsetModeHighQuality);
			CCache::pG->SetTextContrast(0); //10 - for whiter text, 0 - for darker
			break;
		}
	}

	//Apply output scale
	CCache::pG->ResetTransform();
	CCache::pG->ScaleTransform(CFrameStyleStack::Top().iOutputScale * 0.001f, CFrameStyleStack::Top().iOutputScale * 0.001f);

	//Update drawing options
	CCache::dwRenderOptions = m_dwRenderOptions;

	return TRUE;
}

bool CFrame::CleanAfterLayout()
{
	//Pop style stacks, if needed
	if (m_pStyleCharacter)	{ CCharacterStyleStack::PopStyleStack(*m_pStyleCharacter); }
	if (m_pStyleParagraph)	{ CParagraphStyleStack::PopStyleStack(*m_pStyleParagraph); }
	if (m_pStyle)			{ CFrameStyleStack::PopStyleStack(); }

	return TRUE;
}

bool CFrame::Render(_IN HDC hDC, _IN SCFGraphics::Rect4i* pUpdateRect)
{
	//Reset all variables anew
	if (!PrepareForLayout(hDC)) { SCFError(ErrorOutOfMemory); return FALSE; }

	//Create a pen
	Pen pen(Color(SCF_ARGB(255, 0, 255, 0)));

	//Set clipping rectangle
	if (pUpdateRect)
	{
		CCache::pG->SetClip(RectF(pUpdateRect->iX * 0.001f, pUpdateRect->iY * 0.001f, pUpdateRect->iWidth * 0.001f, pUpdateRect->iHeight * 0.001f));
	}

	//Draw text-frame outline
	if (CCache::dwRenderOptions & RO_DRAW_TEXTFRAME_OUTLINE)
	{
		pen.SetDashStyle(DashStyleDash);
		CCache::pG->DrawRectangle(&pen, m_Rect.iX * 0.001f, m_Rect.iY * 0.001f, m_Rect.iWidth * 0.001f, m_Rect.iHeight * 0.001f);
	}

	int	iColumnWidth = 0;

	//Set the width of the columns based on current style
	if (CFrameStyleStack::Top().Column.bFixedWidth)	{ iColumnWidth = CFrameStyleStack::Top().Column.iWidth; }
	else												{ iColumnWidth = ((m_Rect.iWidth - (CFrameStyleStack::Top().Inset.iX +  CFrameStyleStack::Top().Inset.iWidth)) - (CFrameStyleStack::Top().Column.iGutter * (__max(0, CFrameStyleStack::Top().Column.iCount - 1)))) / CFrameStyleStack::Top().Column.iCount; }

	if (CCache::dwRenderOptions & RO_DRAW_COLUMN_OUTLINES)
	{
		//Draw column outlines
		pen.SetColor(Color(SCF_ARGB(255, 128, 128, 255)));
		pen.SetDashStyle(DashStyleDash);

		for (int iIndex = 0; iIndex < m_iColumnCount; iIndex++)
		{
			CCache::pG->DrawRectangle(&pen, (m_Rect.iX + CFrameStyleStack::Top().Inset.iX + (iColumnWidth + CFrameStyleStack::Top().Column.iGutter) * iIndex) * 0.001f, (m_Rect.iY + CFrameStyleStack::Top().Inset.iY) * 0.001f, iColumnWidth * 0.001f, (m_Rect.iHeight - (CFrameStyleStack::Top().Inset.iY + CFrameStyleStack::Top().Inset.iHeight)) * 0.001f);
		}
	}

	//Go trough all visible paragraphs
	for (SCF::UINT i = 0; i < m_uiParagraphVisibleCount; i++)
	{
		((CParagraph&)m_Paragraphs[i]).ResetRenderStatus();
	}

	//Go trough all visible paragraphs
	for (SCF::UINT i = 0; i < m_uiParagraphVisibleCount; i++)
	{
		//Call [Render(..)] on each paragraph, to draw its contents
		if (!((CParagraph&)m_Paragraphs[i]).Render(pUpdateRect)) { return FALSE; }
	}

	if (CCache::dwRenderOptions & RO_DRAW_SELECTION)
	{
		if (CCache::SelectionRectList.Size())
		{
			int		iDPI = GetDeviceCaps(CCache::hDC, LOGPIXELSY);
			float	fPoint2Screen = (int)iDPI / 72.0f;

			//Create a second device context & a bitmap to paint to
			HBITMAP	hBitmap		= CreateCompatibleBitmap(CCache::hDC, (int)(m_Rect.iWidth / 1000 * fPoint2Screen + 0.5), (int)(m_Rect.iHeight / 1000 * fPoint2Screen + 0.5));
			HDC		hBitmapDC	= CreateCompatibleDC(CCache::hDC);

			HBITMAP	hOldBitmap	= (HBITMAP)SelectObject(hBitmapDC, hBitmap);

			//Draw background for the text-frame
			SelectObject(hBitmapDC, GetStockObject(BLACK_PEN));
			SelectObject(hBitmapDC, GetStockObject(BLACK_BRUSH));

			Rectangle(hBitmapDC, 0, 0, (int)(m_Rect.iWidth / 1000 * fPoint2Screen + 0.5), (int)(m_Rect.iHeight / 1000 * fPoint2Screen + 0.5));

			SelectObject(hBitmapDC, GetStockObject(WHITE_PEN));
			SelectObject(hBitmapDC, GetStockObject(WHITE_BRUSH));

			XFORM xForm;

			//Zero translation
			xForm.eDx = 0;
			xForm.eDy = 0;

			//Setup transformation matrix to produce a scale transformation
			xForm.eM11 = CFrameStyleStack::Top().iOutputScale * 0.001f;
			xForm.eM12 = 0;
			xForm.eM21 = 0;
			xForm.eM22 = CFrameStyleStack::Top().iOutputScale * 0.001f;

			//Render all selection rectangles
			for (SCF::UINT i = 0; i < CCache::SelectionRectList.Size(); i++)
			{
				Rect4i& rRect = ((CRectInt&)CCache::SelectionRectList[i]).Value();

				Rectangle(hBitmapDC, (int)((rRect.iX - m_Rect.iX) / 1000 * fPoint2Screen + 0.5f), (int)((rRect.iY - m_Rect.iX) / 1000 * fPoint2Screen + 0.5f), (int)((rRect.iX + rRect.iWidth - m_Rect.iX) / 1000 * fPoint2Screen + 0.5f), (int)((rRect.iY + rRect.iHeight - m_Rect.iX) / 1000 * fPoint2Screen + 0.5f));
			}

			int OldGraphicMode = 0;
			if ((OldGraphicMode = SetGraphicsMode(CCache::hDC, GM_ADVANCED)) != 0) { SetWorldTransform(CCache::hDC, &xForm); }

			BitBlt(CCache::hDC, (int)(m_Rect.iX / 1000 * fPoint2Screen + 0.5f), (int)(m_Rect.iY / 1000 * fPoint2Screen + 0.5f), (int)(m_Rect.iWidth / 1000 * fPoint2Screen + 0.5), (int)(m_Rect.iHeight / 1000 * fPoint2Screen + 0.5), hBitmapDC, 0, 0, SRCINVERT);

			if (OldGraphicMode)
			{
				xForm.eDx = 0;
				xForm.eDy = 0;

				xForm.eM11 = 1;
				xForm.eM12 = 0;
				xForm.eM21 = 0;
				xForm.eM22 = 1;

			 SetWorldTransform(CCache::hDC, &xForm);
			 SetGraphicsMode  (CCache::hDC, OldGraphicMode);
			}

			//Clean up
			SelectObject(hBitmapDC, hOldBitmap);
			DeleteObject(hBitmap);

			DeleteDC(hBitmapDC);
		}
	}

	CleanAfterLayout();
	return TRUE;
}

int CFrame::ContentStats(_IN SCF::ENUM eStatsType)
{
	switch (eStatsType)
	{
	case STATS_CHARACTERS: 
		{
			int iLetterCount = 0;

			for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
			{
				iLetterCount += ((CParagraph&)m_Paragraphs[i]).TextLength();// - ((CParagraph&)m_Paragraphs[i]).FormattingCharCount();
			}

			return iLetterCount;
		}

	case STATS_WORDS: 
		{
			int iWordCount = 0;

			for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
			{
				iWordCount += ((CParagraph&)m_Paragraphs[i]).WordCount(WORD_NORMAL);
			}

			return iWordCount;
		}

	case STATS_LINES: 
		{
			int iLineCount = 0;
			int iCurParagraph = 0;

			for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
			{
				//Make sure to go only through visible/laid-out paragraphs
				if (iCurParagraph >= m_iParagraphsLaidOut) { break; }

				for (SCF::UINT j = 0; j < ((CParagraph&)m_Paragraphs[i]).VisualParts().Size(); j++)
				{
					iLineCount += ((CParagraph&)m_Paragraphs[i]).VisualPart(j).LineCount();
				}

				iCurParagraph++;
			}

			return iLineCount;
		}

	case STATS_PARAGRAPHS:		{ return (int)m_Paragraphs.Size(); }
	case STATS_CHARACTERS_FIT:	{ return __max(0, (int)(m_iCharactersLaidOut + m_iFormattingCharsLaidOut + m_iParagraphsLaidOut) - 1); }
	case STATS_WORDS_FIT:		{ return m_iWordsLaidOut; }
	case STATS_PARAGRAPHS_FIT:	{ return m_iParagraphsLaidOut; }
	}

	return 0;
}

bool CFrame::ApproximateWidth(_IN HDC hDC, _INOUT SCFGraphics::Rect4i* pInOuRect4i, _IN int iMinWidth, _IN int iMaxWidth)
{
	if (!pInOuRect4i || !hDC || (iMinWidth < 0) || (iMaxWidth < 0)) { SCFError(ErrorInvalidArgument); return FALSE; }

	int l_iHigh	= iMaxWidth + 1;
	int l_iLow	= iMinWidth;
	int l_iMid	= 0;

	SCFGraphics::Rect4i	l_Rect;
	SCFGraphics::Rect4i	l_LastGood = m_Rect;

	while (l_iHigh > (l_iLow + 1))
	{
		l_iMid = (l_iHigh + l_iLow) / 2;

		RectBasedOnFrameWidth(hDC, l_iMid, &l_Rect);

		if (l_Rect.iHeight > pInOuRect4i->iHeight)
		{
			l_iLow = __max(l_iMid, l_Rect.iWidth);
		}
		else
		{
			l_iHigh    = l_Rect.iWidth;
			l_LastGood = l_Rect;
		}
	}

	pInOuRect4i->iWidth	 = l_LastGood.iWidth;
	pInOuRect4i->iHeight = l_LastGood.iHeight;
	return TRUE;
}

bool CFrame::EmptyFrameRect(_IN HDC hDC, _OUT SCFGraphics::Rect4i* pOuRect4i)
{
	//Check arguments
	if (!pOuRect4i) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Prepare for layout 
	if (!PrepareForLayout(hDC)) { SCFError(ErrorOutOfMemory); return FALSE; }

	CCharacterStyleStack::UpdateStyleState();

	pOuRect4i->iWidth = CFrameStyleStack::Top().Inset.iX + CFrameStyleStack::Top().Inset.iWidth;

	switch (CFrameStyleStack::Top().eFirstBaseline)
	{
	case BL_ASCENT:
		{
			pOuRect4i->iHeight = CFrameStyleStack::Top().Inset.iY + CFrameStyleStack::Top().Inset.iHeight + (CCache::OutlineMetrics.otmAscent + CCache::OutlineMetrics.otmMacAscent) * 5;
			break;
		}
	case BL_LEADING:
		{
			int l_iLeading = 1000;
			CCharacterStyleStack::QueryIntegerValue(CP_LEADING, &l_iLeading);
			pOuRect4i->iHeight = CFrameStyleStack::Top().Inset.iY + CFrameStyleStack::Top().Inset.iHeight + CCache::TextMetrics.tmHeight * l_iLeading / 100 - CCache::TextMetrics.tmDescent * 10;
			break;
		}
	case BL_FIXED:
		{
			pOuRect4i->iHeight = CFrameStyleStack::Top().Inset.iY + CFrameStyleStack::Top().Inset.iHeight + CFrameStyleStack::Top().iFirstBaselineOffset;
			break;
		}
	}

	CleanAfterLayout();
	return TRUE;	
}

bool CFrame::RectBasedOnFrameHeight(_IN HDC hDC, _IN int iDesiredFrameHeight, _OUT SCFGraphics::Rect4i* pOuRect4i)
{
	//Check arguments
	if (!pOuRect4i || !hDC) { SCFError(ErrorInvalidArgument); return FALSE; }
	if (!m_iTextLength)	{ return EmptyFrameRect(hDC, pOuRect4i); }

	SCFGraphics::Rect4i	l_TempRect;
	int l_iMinWidth;
	int l_iMaxWidth;

	RectBasedOnFrameWidth(hDC, -1, &l_TempRect);
	l_iMaxWidth	= l_TempRect.iWidth;

	//If we are trying to get width for a height that is too small, return rectangle for smallest height
	if (iDesiredFrameHeight <= l_TempRect.iHeight)
	{
		pOuRect4i->iWidth  = l_TempRect.iWidth;
		pOuRect4i->iHeight = l_TempRect.iHeight;
		return TRUE;
	}

	RectBasedOnMaxWordWidth(hDC, &l_TempRect);
	l_iMinWidth = l_TempRect.iWidth;

	//If we are trying to get width for a height that is too small, return rectangle for smallest height
	if (l_iMinWidth >= l_iMaxWidth)
	{
		pOuRect4i->iWidth  = l_TempRect.iWidth;
		pOuRect4i->iHeight = l_TempRect.iHeight;
		return TRUE;
	}

	pOuRect4i->iWidth  = 0;
	pOuRect4i->iHeight = iDesiredFrameHeight;

	return ApproximateWidth(hDC, pOuRect4i, l_iMinWidth, l_iMaxWidth);
}

//Measures the minimal needed width & height in order to display all text within the frame, given a desired paragraph width ([iDesiredParagraphWidth] can be specified as -1 to indicate that maximum line width should be used)
bool CFrame::RectBasedOnFrameWidth(_IN HDC hDC, _IN int iDesiredFrameWidth, _OUT SCFGraphics::Rect4i* pOuRect4i)
{
	if (!pOuRect4i || !hDC) { SCFError(ErrorInvalidArgument); return FALSE; }
	if (!m_iTextLength)	{ return EmptyFrameRect(hDC, pOuRect4i); }

	SCFGraphics::Rect4i l_Rect = m_Rect;

	int l_iMaxWordWidth = MaxWordWidth(hDC);
	SCF::DWORD l_dwLayoutOptions = LO_SHRINK_HORIZONTALLY;

	//Prepare style state
	if (iDesiredFrameWidth != -1)
	{
		l_dwLayoutOptions |= LO_FIT_HORIZONTALLY;

		if (m_pStyle)
		{
			//Execute text-frame style
			CFrameStyleStack::PushStyleStack();
			CFrameStyleStack::ExecuteStyle(*m_pStyle);

			int iColumnWidth = ((iDesiredFrameWidth - (CFrameStyleStack::Top().Inset.iX +  CFrameStyleStack::Top().Inset.iWidth)) - (CFrameStyleStack::Top().Column.iGutter * (__max(0, CFrameStyleStack::Top().Column.iCount - 1)))) / CFrameStyleStack::Top().Column.iCount; 
		
			CFrameStyleStack::PopStyleStack();

			if (l_iMaxWordWidth > iColumnWidth)
			{
				return RectBasedOnParagraphWidth(hDC, l_iMaxWordWidth, pOuRect4i);
			}
		}
		else
		{
			if (l_iMaxWordWidth > iDesiredFrameWidth)
			{
				return RectBasedOnFrameWidth(hDC, l_iMaxWordWidth, pOuRect4i);
			}
		}
	}

	m_Rect.iWidth = iDesiredFrameWidth;

	if (!LayOut(hDC, UO_UPDATE_LAYOUT, TRUE, l_dwLayoutOptions)) { return FALSE; }

	pOuRect4i->iWidth  = 0;
	pOuRect4i->iHeight = 0;

	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		if (((CParagraph&)m_Paragraphs[i]).VisualParts().Size())
		{
			//Update maximum width with regard to first-line indent
			pOuRect4i->iWidth   = __max(pOuRect4i->iWidth, ((CParagraph&)m_Paragraphs[i]).VisualPartLast().Width());
			pOuRect4i->iHeight +=                          ((CParagraph&)m_Paragraphs[i]).VisualPartLast().Height();
		}
	}

	//Prepare style state
	if (m_pStyle)
	{
		//Execute text-frame style
		CFrameStyleStack::PushStyleStack();
		CFrameStyleStack::ExecuteStyle(*m_pStyle);

		if (CFrameStyleStack::Top().Column.iCount > 1)
		{
			pOuRect4i->iWidth =	(pOuRect4i->iWidth * CFrameStyleStack::Top().Column.iCount) +
								(CFrameStyleStack::Top().Column.iGutter * (CFrameStyleStack::Top().Column.iCount - 1)) +
								CFrameStyleStack::Top().Inset.iX + CFrameStyleStack::Top().Inset.iWidth;

			m_Rect.iWidth = pOuRect4i->iWidth;

			int l_iHigh	= pOuRect4i->iHeight + 1;
			int l_iLow	= 0;

			int l_iTotalCharacterCount = ContentStats(STATS_CHARACTERS);

			while (l_iHigh > (l_iLow + 1))
			{
				m_Rect.iHeight = (l_iHigh + l_iLow) / 2;

				LayOut(hDC, UO_UPDATE_LAYOUT, FALSE, LO_FIT_HORIZONTALLY | LO_FIT_VERTICALLY);

				if (ContentStats(STATS_CHARACTERS_FIT) < l_iTotalCharacterCount)
				{
					l_iLow = (l_iHigh + l_iLow) / 2;
				}
				else
				{
					l_iHigh	= (l_iHigh + l_iLow) / 2;
				}
			}

			LayOut(hDC, UO_UPDATE_LAYOUT, FALSE, LO_FIT_HORIZONTALLY | LO_FIT_VERTICALLY);
	
			if (ContentStats(STATS_CHARACTERS_FIT) < l_iTotalCharacterCount)
			{
				m_Rect.iHeight++;
			}

			pOuRect4i->iHeight = m_Rect.iHeight;
		}
		else
		{
			pOuRect4i->iWidth += CFrameStyleStack::Top().Inset.iX + CFrameStyleStack::Top().Inset.iWidth;
		}

		pOuRect4i->iHeight += CFrameStyleStack::Top().Inset.iY + CFrameStyleStack::Top().Inset.iHeight;
		CFrameStyleStack::PopStyleStack();
	}

	m_Rect = l_Rect;
	return TRUE;
}

//Measures the minimal needed width & height in order to display all text within the frame, given a desired paragraph width ([iDesiredParagraphWidth] can be specified as -1 to indicate that maximum line width should be used)
bool CFrame::RectBasedOnParagraphWidth(_IN HDC hDC, _IN int iDesiredParagraphWidth, _OUT SCFGraphics::Rect4i* pOuRect4i)
{
	//Check arguments
	if (!pOuRect4i || !hDC) { SCFError(ErrorInvalidArgument); return FALSE; }
	if (!m_iTextLength)	{ return EmptyFrameRect(hDC, pOuRect4i); }

	int l_iDesiredFrameWidth = iDesiredParagraphWidth;

	if (iDesiredParagraphWidth != -1)
	{
		//Prepare style state
		if (m_pStyle)
		{
			//Execute text-frame style
			CFrameStyleStack::PushStyleStack();
			CFrameStyleStack::ExecuteStyle(*m_pStyle);

			l_iDesiredFrameWidth =	(iDesiredParagraphWidth * CFrameStyleStack::Top().Column.iCount) +
									(CFrameStyleStack::Top().Column.iGutter * (CFrameStyleStack::Top().Column.iCount - 1)) +
									CFrameStyleStack::Top().Inset.iX +
									CFrameStyleStack::Top().Inset.iWidth;

			CFrameStyleStack::PopStyleStack();
		}
	}

	return RectBasedOnFrameWidth(hDC, l_iDesiredFrameWidth, pOuRect4i);
}

int CFrame::MaxWordWidth(_IN HDC hDC)
{
	//Prepare for layout 
	if (!PrepareForLayout(hDC)) { SCFError(ErrorOutOfMemory); return FALSE; }

	//Prepare temporary rectangle
	SCFGraphics::Rect4i l_Rect02;
	l_Rect02.iX			= 0;
	l_Rect02.iY			= 0;
	l_Rect02.iWidth		= 0;
	l_Rect02.iHeight	= 0;

	int l_iFinalWidestWord = 0;

	SCF::DWORD l_dwLayoutResults = NULL;

	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		int l_iWidestWord = 0;

		//Perform paragraph layout, while requesting the widest word output (by specifying the last optional parameter)
		if (!((CParagraph&)m_Paragraphs[i]).LayOut(&l_Rect02, LO_SHRINK_HORIZONTALLY, CFrameStyleStack::Top().eFirstBaseline, CFrameStyleStack::Top().iFirstBaselineOffset, &l_dwLayoutResults, &l_iWidestWord)) { return FALSE; }

		//Store new widest word, if found 
		l_iFinalWidestWord = __max(l_iFinalWidestWord, l_iWidestWord);
	}

	CleanAfterLayout();

	return l_iFinalWidestWord;
}

bool CFrame::RectBasedOnMaxWordWidth(_IN HDC hDC, _OUT SCFGraphics::Rect4i* pOuRect4i)
{
	//Check arguments
	if (!pOuRect4i || !hDC) { SCFError(ErrorInvalidArgument); return FALSE; }
	if (!m_iTextLength)	{ return EmptyFrameRect(hDC, pOuRect4i); }

	return RectBasedOnParagraphWidth(hDC, MaxWordWidth(hDC), pOuRect4i);
}

bool CFrame::CursorPosToCharacterPos(_IN int iX, _IN int iY, _OUT int* ipOutCharPos)
{
	//Check arguments
	if (!ipOutCharPos) { SCFError(ErrorInvalidArgument); return FALSE; }

	int l_iCurChar = 0;

	//Prepare style state
	if (m_pStyle) { CFrameStyleStack::PushStyleStack(); CFrameStyleStack::ExecuteStyle(*m_pStyle); }

	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		if (((CParagraph&)m_Paragraphs[i]).CursorPosToCharacterPos((iX * 1000) / CFrameStyleStack::Top().iOutputScale, (iY * 1000) / CFrameStyleStack::Top().iOutputScale, ipOutCharPos))
		{
			//Pop style stack
			if (m_pStyle) { CFrameStyleStack::PopStyleStack(); }

			*ipOutCharPos += l_iCurChar;
			return TRUE;
		}

		l_iCurChar += ((CParagraph&)m_Paragraphs[i]).TextLength() + ((CParagraph&)m_Paragraphs[i]).FormattingOverhead();
	}

	//Pop style stack
	if (m_pStyle) { CFrameStyleStack::PopStyleStack(); }

	return FALSE;
}

bool CFrame::AddSelectionRange(_IN int iStartChar, _IN int iLength)
{
	CSelection* pSelection = new CSelection();
	pSelection->iStartChar = iStartChar;
	pSelection->iLength = iLength;

	m_Selections.LastAdd(*pSelection);

	return TRUE;
}

bool CFrame::ClearSelection() { m_Selections.AllDelete(); return TRUE; }

bool CFrame::RealizeSelection(_IN int iStartPos, _IN int iLength)
{
	//Check arguments
	if (!iLength) { return FALSE; }

	int l_iCurChar = 0;

	bool bSuccess = FALSE;

	//Go trough all paragraphs
	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		CParagraph* pParagraph = &(CParagraph&)m_Paragraphs[i];

		if ((iStartPos + iLength <= l_iCurChar) || (iStartPos >= (l_iCurChar + pParagraph->TextLength())))
		{
			//Prepare starting char for next paragraph
			l_iCurChar += pParagraph->TextLength();
			l_iCurChar += pParagraph->FormattingOverhead();

			continue; 
		}

		//Compute absolute start & end point
		int iClearFrom = __max(l_iCurChar, iStartPos);
		int iClearTo   = __min(l_iCurChar + pParagraph->TextLength(), iStartPos + iLength);

		//Quit, if there we went past the requested range
		if ((iStartPos + iLength) < l_iCurChar) { return TRUE; }

		//Create rectangle(s) for the given character range
		if (SUCCEEDED(pParagraph->RealizeSelectionRange(iClearFrom - l_iCurChar, iClearTo - iClearFrom)))
		{
			bSuccess = TRUE;
		}

		//Prepare starting char for next paragraph
		l_iCurChar += pParagraph->TextLength();
		l_iCurChar += pParagraph->FormattingOverhead();
	}

	if (bSuccess) { return TRUE; }
	return FALSE;
}

bool CFrame::CarreRect(_IN int iCharPos, _OUT SCFGraphics::Rect4i& rOutCarerect)
{
	if (RealizeSelection(iCharPos, 1))
	{
		//Prepare style state
		if (m_pStyle) { CFrameStyleStack::PushStyleStack(); CFrameStyleStack::ExecuteStyle(*m_pStyle); }
		
		rOutCarerect = ((CRectInt&)CCache::SelectionRectList.Last()).Value();

		//Apply scale to output
		rOutCarerect.iX      *= CFrameStyleStack::Top().iOutputScale;
		rOutCarerect.iY      *= CFrameStyleStack::Top().iOutputScale;
		rOutCarerect.iWidth  *= CFrameStyleStack::Top().iOutputScale;
		rOutCarerect.iHeight *= CFrameStyleStack::Top().iOutputScale;
	
		//Pop style stack
		if (m_pStyle) { CFrameStyleStack::PopStyleStack(); }

		//WTF???????
		//CCache::SelectionRectList.pop_back();
		CCache::SelectionRectList.LastDelete();
		return TRUE;
	}

	return FALSE;
}

bool CFrame::IsCursorInSelection(_IN int iX, _IN int iY)
{
	//Prepare style state
	if (m_pStyle) { CFrameStyleStack::PushStyleStack(); CFrameStyleStack::ExecuteStyle(*m_pStyle); }

	for (SCF::UINT i = 0; i < CCache::SelectionRectList.Size(); i++)
	{
		_PENDING;
		//if (PointInRect(iX / CFrameStyleStack::Top().iOutputScale, iY / CFrameStyleStack::Top().iOutputScale, *rect_I))
		{
			//Pop style stack
			if (m_pStyle) { CFrameStyleStack::PopStyleStack(); }

			return TRUE; 
		}
	}

	//Pop style stack
	if (m_pStyle) { CFrameStyleStack::PopStyleStack(); }

	return FALSE;
}

bool CFrame::IsCursorInText(_IN int iX, _IN int iY)
{
	//Prepare style state
	if (m_pStyle) { CFrameStyleStack::PushStyleStack(); CFrameStyleStack::ExecuteStyle(*m_pStyle); }

	//Go trough all paragraphs
	for (SCF::UINT i = 0; i < m_Paragraphs.Size(); i++)
	{
		if (((CParagraph&)m_Paragraphs[i]).IsCursorInText(iX / CFrameStyleStack::Top().iOutputScale, iY / CFrameStyleStack::Top().iOutputScale))
		{ 
			//Pop style stack
			if (m_pStyle) { CFrameStyleStack::PopStyleStack(); }

			return TRUE; 
		}
	}

	//Pop style stack
	if (m_pStyle) { CFrameStyleStack::PopStyleStack(); }

	return FALSE;
}