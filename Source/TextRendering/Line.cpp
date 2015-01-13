#include "Line.h"
#include "Paragraph.h"
#include "ParagraphStyleStack.h"
#include "Constants.h"
#include "Frame.h"
#include "Cache.h"

using namespace SCFTextRendering;

CLine::CLine()
{
	//Reset object variables
	m_BoundingBox.iX = 0;
	m_BoundingBox.iY = 0;

	m_BoundingBox.iWidth = 0;
	m_BoundingBox.iHeight = 0;

	m_iLeading = 0;
	m_iAscent = 0;
	m_iDescent = 0;
}

CLine::~CLine()
{
	m_VisualParts.AllDelete();
}

bool CLine::Rect(_OUT SCFGraphics::Rect4i* pOuRect4i)
{
	//Check arguments
	if (!pOuRect4i) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Copy rectangle 
	*pOuRect4i = m_BoundingBox;

	return TRUE;
}

bool CLine::RenderBackground(_IN SCFGraphics::ColorRGBA backgroundColor)
{
	//If background is not transparent..
	if (SCF_RGBA_A(backgroundColor))
	{
		//..draw it
		SolidBrush brush(Color(SCF_RGBA_TO_ARGB(backgroundColor)));

		CCache::pG->FillRectangle(&brush, m_BoundingBox.iX * 0.001f, m_BoundingBox.iY * 0.001f, m_BoundingBox.iWidth * 0.001f, m_BoundingBox.iHeight * 0.001f);
	}

	if (CCache::dwRenderOptions & RO_DRAW_LINE_OUTLINES)
	{
		//Draw line outline
		Pen pen(Color(SCF_ARGB(255, 255, 0, 0)));
		pen.SetDashStyle(DashStyleDash);

		CCache::pG->DrawRectangle(&pen, m_BoundingBox.iX * 0.001f, m_BoundingBox.iY * 0.001f, m_BoundingBox.iWidth * 0.001f, m_BoundingBox.iHeight * 0.001f);
	}

	return TRUE;
}

bool CLine::Render(_IN SCFGraphics::Rect4i* pUpdateRect)
{
	//Go trough line words
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		//Call [Render(..)] on each word, to draw its contents
		if (!((CLineVisualPart&)m_VisualParts[i]).Word().Render(pUpdateRect)) { return FALSE; }
	}

	return TRUE;
}

DWORD CLine::FitVertically(_INOUT CWord& rWord, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eBaselineType, _IN int iBaselineOffset, _IN int iHeight)
{
	DWORD dwResult = NULL;

	if (dwLayoutOptions & LO_FIT_VERTICALLY)
	{
		if (dwLayoutOptions & LO_FIT_USING_DESCENT)
		{
			//If the current word is higher than the layout rectangle for the line, mark result & stop
			if ((eBaselineType == BL_LEADING) && ((__max(m_iLeading, rWord.VisualPartLast().Leading()) + __max(m_iDescent, rWord.VisualPartLast().Descent())) > iHeight)) { dwResult |= LR_FIT_FAILED_VERTICAL; }
			if ((eBaselineType == BL_ASCENT)  && ((__max(m_iAscent,  rWord.VisualPartLast().Ascent())  + __max(m_iDescent, rWord.VisualPartLast().Descent())) > iHeight)) { dwResult |= LR_FIT_FAILED_VERTICAL; }
			if ((eBaselineType == BL_FIXED)   && ((iBaselineOffset                                                             + __max(m_iDescent, rWord.VisualPartLast().Descent())) > iHeight)) { dwResult |= LR_FIT_FAILED_VERTICAL; }
		}
		else
		{
			//If the current word is higher than the layout rectangle for the line, mark result & stop
			if ((eBaselineType == BL_LEADING) && (__max(m_iLeading, rWord.VisualPartLast().Leading()) > iHeight)) { dwResult |= LR_FIT_FAILED_VERTICAL; }
			if ((eBaselineType == BL_ASCENT)  && (__max(m_iAscent,  rWord.VisualPartLast().Ascent())  > iHeight)) { dwResult |= LR_FIT_FAILED_VERTICAL; }
			if ((eBaselineType == BL_FIXED)   && (iBaselineOffset                                                             > iHeight)) { dwResult |= LR_FIT_FAILED_VERTICAL; }
		}

		if (dwResult & LR_FIT_FAILED_VERTICAL)
		{
			for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
			{
				((CLineVisualPart&)m_VisualParts[i]).Word().RollBackLastLayout(); 
			}

			rWord.RollBackLastLayout(); 
			return dwResult;
		}
	}

	//Update maximum vertical word proportions
	m_iLeading		= __max(m_iLeading,	rWord.VisualPartLast().Leading());
	m_iDescent		= __max(m_iDescent,	rWord.VisualPartLast().Descent());
	m_iAscent		= __max(m_iAscent,  rWord.VisualPartLast().Ascent());

	return dwResult;
}

bool CLine::LayOutTab(_IN SCFGraphics::Rect4i* pLineLayoutRect, _IN SCFGraphics::Rect4i* pTabLayoutRect, _IN CWord& rWordTab, _IN CVector<CWord>& rWordsRemanining, _IN SCF::DWORD dwLayoutOptions, _INOUT int* ipCurrentTab)
{
	//If there is a word after the current tab..
	if (rWordsRemanining.Size())
	{
		//..do layout for next word
		SCF::DWORD l_dwWordLayoutResults = NULL;
		if (!((CWord&)rWordsRemanining[0]).LayOut(pTabLayoutRect, dwLayoutOptions, pLineLayoutRect->iWidth / 2, &l_dwWordLayoutResults)) { return FALSE; }
	
		//If next word is a whitespace..
		if (((CWord&)rWordsRemanining[0]).IsWhiteSpace())
		{
			//..reset it's dimensions
			((CWordVisualPart&)((CWord&)rWordsRemanining[0]).VisualPartLast()).Width(0);
			((CWordVisualPart&)((CWord&)rWordsRemanining[0]).VisualPartLast()).Height(0);
		}
	}

	CTabEntry* l_pCurrentTab = CParagraphStyleStack::QueryTabEntry(*ipCurrentTab);

	int l_iLineFirstIndent = 0;
	if (dwLayoutOptions & LO_FIRSTLINE) { CParagraphStyleStack::QueryIntegerValue(PP_FIRSTLINE_INDENT, &l_iLineFirstIndent); }

	//Convert tab width to internal units
	switch (l_pCurrentTab->eUnit)
	{
	case UNIT_POINT:      { l_pCurrentTab->iPosition *= PT_2_INTERNAL;        break; }
	case UNIT_INCH:       { l_pCurrentTab->iPosition *= INCH_2_INTERNAL;      break; }
	case UNIT_MILLIMETER: { l_pCurrentTab->iPosition *= MILIMETER_2_INTERNAL; break; }
	}

	//Compute absolute tab position
	l_pCurrentTab->iPosition += pLineLayoutRect->iX - l_iLineFirstIndent;

	//Skip user tabs that are positioned before the tab entry/char in the text
	while ((CParagraphStyleStack::QueryTabCount() > *ipCurrentTab) && (l_pCurrentTab->iPosition < ((CWordVisualPart&)rWordTab.VisualPartLast()).X()))
	{
		(*ipCurrentTab)++;
		l_pCurrentTab = CParagraphStyleStack::QueryTabEntry(*ipCurrentTab);

		//Convert tab width to internal units
		switch (l_pCurrentTab->eUnit)
		{
		case UNIT_POINT:      { l_pCurrentTab->iPosition *= PT_2_INTERNAL;        break; }
		case UNIT_INCH:       { l_pCurrentTab->iPosition *= INCH_2_INTERNAL;      break; }
		case UNIT_MILLIMETER: { l_pCurrentTab->iPosition *= MILIMETER_2_INTERNAL; break; }
		}

		//Compute absolute tab position
		l_pCurrentTab->iPosition += pLineLayoutRect->iX - l_iLineFirstIndent;
	}

	//If there are no user tabs, or we got past them, use standard tabs
	if (!CParagraphStyleStack::QueryTabCount() || (*ipCurrentTab >= CParagraphStyleStack::QueryTabCount()))
	{
		//Obtain current standard tab width
		int	l_iStandardTabWidth = 0;
		CFrameStyleStack::QueryIntegerValue(FP_TAB_WIDTH, &l_iStandardTabWidth);

		//Calculate tab width based on standard tab width
		((CWordVisualPart&)rWordTab.VisualPartLast()).Width(((int)(((((CWordVisualPart&)rWordTab.VisualPartLast()).X() - pLineLayoutRect->iX + l_iLineFirstIndent) / l_iStandardTabWidth) * 1.001) + 1) * l_iStandardTabWidth - (((CWordVisualPart&)rWordTab.VisualPartLast()).X() - pLineLayoutRect->iX + l_iLineFirstIndent)); 
	}
	else
	{
		switch (l_pCurrentTab->eAlign)
		{
		case ALIGN_LEFT:   { ((CWordVisualPart&)rWordTab.VisualPartLast()).Width(__max(0, l_pCurrentTab->iPosition - (((CWordVisualPart&)((CWord&)rWordsRemanining[0]).VisualPartLast()).X())));                                                                   (*ipCurrentTab)++; break; }
		case ALIGN_CENTER: { ((CWordVisualPart&)rWordTab.VisualPartLast()).Width(__max(0, l_pCurrentTab->iPosition - (((CWordVisualPart&)rWordTab.VisualPartLast()).X()) - (((CWordVisualPart&)((CWord&)rWordsRemanining[0]).VisualPartLast()).Width() / 2))); (*ipCurrentTab)++; break; }
		case ALIGN_RIGHT:  { ((CWordVisualPart&)rWordTab.VisualPartLast()).Width(__max(0, l_pCurrentTab->iPosition - (((CWordVisualPart&)rWordTab.VisualPartLast()).X()) -  ((CWordVisualPart&)((CWord&)rWordsRemanining[0]).VisualPartLast()).Width()));      (*ipCurrentTab)++; break; }
		case ALIGN_CHARACTER:	
			{ 
				((CWordVisualPart&)rWordTab.VisualPartLast()).Width(__max(0, l_pCurrentTab->iPosition - (((CWordVisualPart&)rWordTab.VisualPartLast()).X()) - ((CWord&)rWordsRemanining[0]).CharOffset((SCF::TCHAR)l_pCurrentTab->iCharacter)));				

				(*ipCurrentTab)++; 
				break; 
			}
		}
	}

	if (dwLayoutOptions & LO_FIT_HORIZONTALLY)
	{
		if ((((CWordVisualPart&)rWordTab.VisualPartLast()).X() + ((CWordVisualPart&)rWordTab.VisualPartLast()).Width()) > (pTabLayoutRect->iX + pTabLayoutRect->iWidth))
		{
			((CWordVisualPart&)rWordTab.VisualPartLast()).Width(__max(0, (pTabLayoutRect->iX + pTabLayoutRect->iWidth) - ((CWordVisualPart&)rWordTab.VisualPartLast()).X()));

			//Roll back temporary layout if it was done
			if (rWordsRemanining.Size()) { ((CWord&)rWordsRemanining[0]).RollBackLastLayout(); }
			return FALSE;
		}
	}

	//Roll back temporary layout if it was done
	if (rWordsRemanining.Size()) { ((CWord&)rWordsRemanining[0]).RollBackLastLayout(); }
	return TRUE;
}

bool CLine::LayOut(_IN SCFGraphics::Rect4i* pLayoutRect, _IN SCF::DWORD dwLayoutOptions, _IN CVector<CWord>& rWords, _IN SCF::ENUM eAlignType, _IN SCF::ENUM eBaselineType, _IN _OPT int iBaselineOffset, _OUT SCF::DWORD* dwpOutLayoutResults, _OUT SCF::UINT* puiOutWordContinue)
{
	//Check arguments
	if (!pLayoutRect || !dwpOutLayoutResults) { SCFError(ErrorInvalidArgument); return FALSE; }

	if ((dwLayoutOptions & LO_FIT_HORIZONTALLY)	&& (pLayoutRect->iWidth <= 0))  { SCFError(ErrorInvalidArgument); return FALSE; }
	if ((dwLayoutOptions & LO_FIT_VERTICALLY)   && (pLayoutRect->iHeight <= 0)) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Reset measurements
	m_iLeading	= 0;
	m_iAscent	= 0;
	m_iDescent	= 0;

	//Prepare  temporary variables
	SCFGraphics::Rect4i	l_CurLayoutRect = *pLayoutRect;
	int l_iCurrentTab = 0;
	SCF::DWORD l_dwWordLayoutResults = NULL;
	
	//Prepare output layout status variable
	*dwpOutLayoutResults = NULL;

	//Store new values
	m_BoundingBox = *pLayoutRect;

	m_VisualParts.AllDelete();

	CLineVisualPart* l_pCurVisPart;

	//Prepare iterators
	//list <CWord*>::iterator	word_I = start_I;
	SCF::UINT uiWord = 0;

	bool l_bAbortLayout = FALSE;

	//Lay out words, until they no longer fit into the layout/bounding rect specified by the [pLayoutRect] argument
	while (!l_bAbortLayout && (uiWord < rWords.Size()))
	{
		//Lay out a word & obtain the bounding rectangle of the last layout
		if (!rWords[uiWord].LayOut(&l_CurLayoutRect, dwLayoutOptions, pLayoutRect->iWidth / 2, &l_dwWordLayoutResults)) { return FALSE; }
	
		//Create a new visual part
		l_pCurVisPart = new CLineVisualPart(rWords[uiWord], (int)rWords[uiWord].VisualParts().Size() - 1);

		//Check, whether the word is a tab
		if (rWords[uiWord].Type() == WORD_TAB)
		{
			CVectorRange<CWord>* pRange = new CVectorRange<CWord>(rWords, uiWord + 1);

			if (!LayOutTab(pLayoutRect, &l_CurLayoutRect, rWords[uiWord], (CVector<CWord>&)*pRange, dwLayoutOptions, &l_iCurrentTab))
			{
				l_bAbortLayout = TRUE;
			}

			delete pRange;
	
			//Clear word layout results
			l_dwWordLayoutResults = NULL;
		}

		//Prepare layout Rectangle for next word
		l_CurLayoutRect.iX	+= rWords[uiWord].VisualPartLast().Width();
		if (dwLayoutOptions & LO_FIT_HORIZONTALLY) { l_CurLayoutRect.iWidth -= rWords[uiWord].VisualPartLast().Width(); }

		//If the word laid itself only partially into the layout rectangle..
		if (l_dwWordLayoutResults & LR_FIT_FAILED_HORIZONTAL) 
		{
			//If horizontal fitting failed at the first word of the line & nothing fit into the layout rect..
			if ((uiWord == 0) && (l_dwWordLayoutResults & LR_NOTHING_FINISHED)) 
			{
				//Mark results about layout completion & abortion
				*dwpOutLayoutResults |= LR_LAYOUT_ABORTED | LR_NOTHING_FINISHED;

				//Stop line layout
				l_bAbortLayout = TRUE;

				delete l_pCurVisPart;
				break;
			}

			if ((rWords[uiWord].Type() == WORD_SPACE) && (l_dwWordLayoutResults & LR_FULLY_FINISHED))
			{
				*dwpOutLayoutResults |= FitVertically(rWords[uiWord], dwLayoutOptions, eBaselineType, iBaselineOffset, pLayoutRect->iHeight);
				if (*dwpOutLayoutResults & LR_FIT_FAILED_VERTICAL) 
				{
					delete l_pCurVisPart;
					break; 
				}
			
				m_VisualParts.LastAdd(*l_pCurVisPart);
				uiWord++;
				
				break;
			}

			//..if splitting words is NOT allowed 
			if (!(dwLayoutOptions & LO_SPLIT_WORDS) && !rWords[uiWord].CanBeSplit())
			{
				rWords[uiWord].RollBackLastLayout(); 
				delete l_pCurVisPart;
				break; 
			}

			*dwpOutLayoutResults |= FitVertically(rWords[uiWord], dwLayoutOptions, eBaselineType, iBaselineOffset, pLayoutRect->iHeight);
			if (*dwpOutLayoutResults & LR_FIT_FAILED_VERTICAL)
			{ 
				delete l_pCurVisPart; 
				break; 
			}

			m_VisualParts.LastAdd(*l_pCurVisPart);
			break; 
		}

		*dwpOutLayoutResults |= FitVertically(rWords[uiWord], dwLayoutOptions, eBaselineType, iBaselineOffset, pLayoutRect->iHeight);
		if (*dwpOutLayoutResults & LR_FIT_FAILED_VERTICAL) 
		{
			delete l_pCurVisPart;
			break; 
		}

		m_VisualParts.LastAdd(*l_pCurVisPart);

		if (rWords[uiWord].Type() == WORD_LINE_BREAK)
		{
			//If there is just a single line-break word in the line - empty line
			if (uiWord == 0)
			{
				//Expand empty line word width, if we are asked to expand it
				if (dwLayoutOptions & LO_EXPAND_EMPTY_LINES) { rWords[uiWord].VisualPartLast().Width(pLayoutRect->iWidth); }
			}

			*dwpOutLayoutResults |= LR_LINE_BREAK | LR_PARTIALY_FINISHED;

			if (rWords[uiWord].IsExplicit())
			{
				CWord::s_iFormattingCharsLaidOut++;
			}

			l_bAbortLayout = TRUE;
			break;
		}

		//Update variables
		uiWord++;
	}

	//Store total/max line height for this line
	switch (eBaselineType)
	{
	case BL_ASCENT:		{ m_BoundingBox.iHeight = m_iAscent			+ m_iDescent;	break; }
	case BL_LEADING:	{ m_BoundingBox.iHeight = m_iLeading		+ m_iDescent;	break; }
	case BL_FIXED:		{ m_BoundingBox.iHeight = iBaselineOffset	+ m_iDescent;	break; }
	}

	//Mark results based on current state
	if (l_dwWordLayoutResults & LR_PARTIALY_FINISHED)                             { *dwpOutLayoutResults |= LR_PARTIALY_FINISHED; }
	if ((uiWord == rWords.Size()) && (l_dwWordLayoutResults & LR_FULLY_FINISHED)) { *dwpOutLayoutResults |= LR_FULLY_FINISHED; }
	
	//If no words were COMPLETELY laid out..
	if (uiWord == 0)
	{
		//..if the vertical fitting failed (NOT using [l_dwWordLayoutResults], because words don't perform vertical fitting)
		if (*dwpOutLayoutResults & LR_FIT_FAILED_VERTICAL) { *dwpOutLayoutResults |= LR_NOTHING_FINISHED; }

		//..if the horizontal fitting failed on a normal word
		if ((l_dwWordLayoutResults & LR_FIT_FAILED_HORIZONTAL) && (rWords[uiWord].Type() == WORD_NORMAL)) { *dwpOutLayoutResults |= LR_NOTHING_FINISHED; *dwpOutLayoutResults |= LR_LAYOUT_ABORTED; }
	}

	//Don't perform line aligning, if it is empty
	if (!(*dwpOutLayoutResults & LR_FIT_FAILED_VERTICAL))
	{
		if ((uiWord != 0) || (rWords[uiWord].Type() != WORD_LINE_BREAK))
		{
			//Align words to a common baseline
			AlignToBaseLine(pLayoutRect->iY, eBaselineType, m_iLeading, m_iAscent, m_iDescent, iBaselineOffset);

			//Start traversing the word list in the reversed order
			//list <CLineVisualPart*>::reverse_iterator lastPart_I = m_VisualParts.rbegin();

			//Set the rect of the spaces to a zero width rec at the right end of the layout rect
			//while ((lastPart_I != m_VisualParts.rend()) && ((*lastPart_I)->Word()->Type() == WORD_SPACE)) 
			
			for (int i = m_VisualParts.Size() - 1; i >= 0; i--) 
			{
				if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() != WORD_SPACE) { break; }

				if (((CLineVisualPart&)m_VisualParts[i]).Word().TextLength() == 1)
				{
					((CLineVisualPart&)m_VisualParts[i]).VisualPart().Width(0);
				}
				//lastPart_I++; 
			}

			if (!(dwLayoutOptions & LO_SHRINK_HORIZONTALLY))
			{
				AlignHorizontally(pLayoutRect, eAlignType);
			}
		}

		if (m_VisualParts.Size())
		{
			m_BoundingBox.iX     = ((CLineVisualPart&)m_VisualParts[0]).VisualPart().BoundingBox().iX;
			m_BoundingBox.iWidth = ((CLineVisualPart&)m_VisualParts.Last()).VisualPart().BoundingBox().iX + ((CLineVisualPart&)m_VisualParts.Last()).VisualPart().BoundingBox().iWidth - m_BoundingBox.iX;
		}
	}

	if (*dwpOutLayoutResults & LR_FIT_FAILED_VERTICAL)
	{
		if (puiOutWordContinue) { *puiOutWordContinue = 0; }
	}
	else
	{
		if (*dwpOutLayoutResults &  LR_LINE_BREAK)
		{
			if (puiOutWordContinue) { *puiOutWordContinue = uiWord + 1; }
		}
		else
		{
			if (puiOutWordContinue) { *puiOutWordContinue = uiWord; }
		}
	}

	return TRUE;
}

bool CLine::AlignHorizontally(_IN SCFGraphics::Rect4i* pRect, _IN SCF::ENUM eAlignType)
{
	//Check arguments
	if (!pRect) { SCFError(ErrorInvalidArgument); return FALSE; }

	if (!m_VisualParts.Size())		{ return TRUE; }
	if (eAlignType == ALIGN_LEFT)	{ return TRUE; }

	int l_iEndOffset	= 0;

	//Check whether the whole line contains only tabs, quit if true
	{
		bool l_bFound = FALSE;

		//Go through all the visual parts of words that belong to this line
		for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
		{
			if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() != WORD_TAB) { l_bFound = TRUE; break; }
		}

		if (!l_bFound) { return TRUE; }
	}

	//Start traversing the word list in the reversed order
	//list <CLineVisualPart*>::reverse_iterator lastPart_I = m_VisualParts.rbegin();

	//If there is only a single whitespace word in the line
	//if (((*lastPart_I)->Word()->Type() == WORD_SPACE) && (m_VisualParts.Size() == 1)) 
	if ((((CLineVisualPart&)m_VisualParts.Last()).Word().Type() == WORD_SPACE) && (m_VisualParts.Size() == 1)) 
	{
		SCFGraphics::Rect4i l_TempRect = ((CLineVisualPart&)m_VisualParts.Last()).VisualPart().BoundingBox();

		if ((eAlignType == ALIGN_RIGHT) || (eAlignType == (ALIGN_RIGHT | ALIGN_JUSTIFY)))
		{
			l_TempRect.iX += (pRect->iX + pRect->iWidth) - (l_TempRect.iX + l_TempRect.iWidth);
		}

		if ((eAlignType == ALIGN_CENTER) || (eAlignType == (ALIGN_CENTER | ALIGN_JUSTIFY)))
		{
			l_TempRect.iX += ((pRect->iX + pRect->iWidth) - (l_TempRect.iX + l_TempRect.iWidth)) / 2;
		}

		((CLineVisualPart&)m_VisualParts.Last()).VisualPart().BoundingBox(l_TempRect);
		return TRUE; 
	}

/*	//Set the rest of the spaces to a zero width rec at the right end of the layout rect
	while ((lastPart_I != m_VisualParts.rend()) && ((*lastPart_I)->Word()->Type() == WORD_SPACE)) 
	{
		(*lastPart_I)->Word()->Width(lastPart_I->iPartIndex, 0);
		lastPart_I++; 
	}*/

	//If there are no words to align, quit..
	//if (lastPart_I == m_VisualParts.rend()) { return TRUE; }
	if (!m_VisualParts.Size()) { return TRUE; }

	//Compute the distance from the last word to the right edge of the layout rectangle
	l_iEndOffset = (pRect->iX + pRect->iWidth) - (((CLineVisualPart&)m_VisualParts.Last()).VisualPart().X() + ((CLineVisualPart&)m_VisualParts.Last()).VisualPart().Width());

	switch (eAlignType)
	{
	case (ALIGN_JUSTIFY | ALIGN_LEFT):
	case (ALIGN_JUSTIFY | ALIGN_CENTER):
	case (ALIGN_JUSTIFY | ALIGN_RIGHT):
		{
			//Temporary variables				
			int		l_iSpaceCount		= 0;
			int		l_iWordCount		= 0;

			bool l_bCounting			= FALSE;
			int		l_iOffsetStep		= 0;

			//CLineVisualPart* tempEntry;

			for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
			{
				//tempEntry = (*visPart_I);

				if (!l_bCounting && (((CLineVisualPart&)m_VisualParts[i]).Word().Type() == WORD_NORMAL)) { l_bCounting = TRUE; continue; }
			
				if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() == WORD_TAB)		{ break; }
				if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() == WORD_SPACE)	{ l_iSpaceCount++; }
				if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() == WORD_NORMAL)	{ l_iWordCount++; }
			}

			if (!(l_iSpaceCount + l_iWordCount))
			{
				l_iOffsetStep = l_iEndOffset;
			}
			else { l_iOffsetStep = l_iEndOffset / (int)(l_iSpaceCount + l_iWordCount); }

			if (l_iWordCount == 0)
			{
				switch (eAlignType)
				{
				case (ALIGN_JUSTIFY | ALIGN_LEFT):
					{
						return TRUE; 
					}
				case (ALIGN_JUSTIFY | ALIGN_CENTER):
					{
						l_iEndOffset /= 2;
						break;
					}
				case (ALIGN_JUSTIFY | ALIGN_RIGHT):
					{
						break;
					}
				}
			}

			//Move/shift all words to the right
			for (int i = m_VisualParts.Size() - 1; i >= 0; i--) 
			{
				//tempEntry = (*visPart_I);

				if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() == WORD_TAB)
				{
					((CLineVisualPart&)m_VisualParts[i]).VisualPart().Width(l_iEndOffset + ((CLineVisualPart&)m_VisualParts[i]).VisualPart().Width());
					break; 
				}

				((CLineVisualPart&)m_VisualParts[i]).VisualPart().TranslateX(l_iEndOffset);
 
				if (l_iWordCount) { l_iEndOffset -= l_iOffsetStep; }
			}

			return TRUE; 
		}

	case ALIGN_RIGHT: 
		{
			//Move/shift all words to the right
			for (int i = m_VisualParts.Size() - 1; i >= 0; i--) 
			{
				if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() == WORD_TAB)
				{
					((CLineVisualPart&)m_VisualParts[i]).VisualPart().Width(l_iEndOffset + ((CLineVisualPart&)m_VisualParts[i]).VisualPart().Width());
					break; 
				}

				((CLineVisualPart&)m_VisualParts[i]).VisualPart().TranslateX(l_iEndOffset);
			}

			return TRUE; 
		}
	case ALIGN_CENTER: 
		{
			//Move/shift all words to the right
			for (int i = m_VisualParts.Size() - 1; i >= 0; i--) 
			{
				if (((CLineVisualPart&)m_VisualParts[i]).Word().Type() == WORD_TAB) 
				{ 
					((CLineVisualPart&)m_VisualParts[i]).VisualPart().Width((l_iEndOffset / 2) + ((CLineVisualPart&)m_VisualParts[i]).VisualPart().Width());
					break; 
				}

				((CLineVisualPart&)m_VisualParts[i]).VisualPart().TranslateX(l_iEndOffset / 2);
			}

			return TRUE; 
		}
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

bool CLine::AlignToBaseLine(_IN int iY, _IN SCF::ENUM eBaselineType, _IN int fLeading, _IN int fAscent, _IN int fDescent, _IN int iBaselineOffset)
{
	UNREFERENCED_PARAMETER(fDescent);

	//Arrange words at base line
	switch (eBaselineType)
	{
	case BL_LEADING:
		{
			for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
			{
				//Align each word to the common base-line
				((CLineVisualPart&)m_VisualParts[i]).VisualPart().AlignToBaseline(iY + fLeading);
			}
			return TRUE;
		}
	case BL_ASCENT:
		{
			for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
			{
				//Align each word to the common base-line
				((CLineVisualPart&)m_VisualParts[i]).VisualPart().AlignToBaseline(iY + fAscent);
			}
			return TRUE;
		}
	case BL_FIXED:
		{
			for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
			{
				//Align each word to the common base-line
				((CLineVisualPart&)m_VisualParts[i]).VisualPart().AlignToBaseline(iY + iBaselineOffset);
			}
			return TRUE;
		}
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

void CLine::Reposition(_IN int iX, _IN int iY)
{
	//Go trough line words
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		((CLineVisualPart&)m_VisualParts[i]).VisualPart().Translate(iX - m_BoundingBox.iX, iY - m_BoundingBox.iY);
	}

	m_BoundingBox.iX = iX;
	m_BoundingBox.iY = iY;
}

void CLine::Translate(_IN int iX, _IN int iY)
{
	m_BoundingBox.iX += iX;
	m_BoundingBox.iY += iY;

	//Go trough line words
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		((CLineVisualPart&)m_VisualParts[i]).VisualPart().Translate(iX, iY);
	}
}

void CLine::TranslateX(_IN int iX)
{
	m_BoundingBox.iX += iX;

	//Go trough line words
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		((CLineVisualPart&)m_VisualParts[i]).VisualPart().TranslateX(iX);
	}
}

void CLine::TranslateY(_IN int iY)
{
	m_BoundingBox.iY += iY;

	//Go trough line words
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		((CLineVisualPart&)m_VisualParts[i]).VisualPart().TranslateY(iY);
	}
}