#include "Paragraph.h"
#include "Constants.h"
#include "Frame.h"
#include "Cache.h"
#include "ParagraphStyleStack.h"

using namespace SCFBase;
using namespace SCFTextRendering;

CParagraph::tUniscribeState	CParagraph::s_Uniscribe;

//Current style & style stack

bool CParagraph::s_bClassInitDone     = FALSE;
int  CParagraph::s_iParagraphsLaidOut = 0;

CParagraph::CParagraph(_IN bool bExplicit)
{
	if (!s_bClassInitDone)
	{
		s_bClassInitDone = TRUE;

		//Uniscribe global, but private state
		s_Uniscribe.iMaxItems		= 0;
		s_Uniscribe.iGeneratedItems	= 0;

		s_Uniscribe.pScriptItem = NULL;

		//Uhmmmm.. reset style state.. :)
		CParagraphStyleStack::ResetStyleState();
	}

	m_iTextLength = 0;
	m_bExplicit = bExplicit;

	m_pPushStyle = NULL;
	m_pPopStyle  = NULL;

	//m_LayoutEnd_I = m_Words.begin();
	m_iWordsVisible = 0;

	m_iFormattingOverhead	= 0;
	m_iFormattingCharCount	= 0;

	m_bRenderDone = FALSE;
}

CParagraph::~CParagraph()
{
	//Go trough all visual parts
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{ 
		//Clear line list
		((CParagraphVisualPart&)m_VisualParts[i]).DeleteLines();
	}
	m_VisualParts.AllDelete();

	//Clear word list
	m_Words.AllDelete();
}

bool CParagraph::Text(_IN CString& rText)
{
	m_iTextLength = rText.Length();

	//Go trough all visual parts
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{ 
		//Clear line list
		((CParagraphVisualPart&)m_VisualParts[i]).DeleteLines();
	}
	m_VisualParts.AllDelete();
	
	//Clear word list
	m_Words.AllDelete();

	m_iWordsVisible = 0;
	m_iFormattingCharCount = 0;

	//Do no try to parse text & create words, if the paragraph is empty
	if (!m_iTextLength) 
	{
		//Implicitly create a word, representing an empty line
		CWord* pNewWord = new CWord(WORD_LINE_BREAK, FALSE);
		pNewWord->Text(g_NewParagraph);

		//Store the new text-word
		m_Words.LastAdd(*pNewWord);

		//Store itemization info
		SCRIPT_ANALYSIS a;
		SecureZeroMemory(&a, sizeof(a));
		((CWord&)m_Words.Last()).ItemBoundary(0, a);

		return TRUE; 
	}

	//Variables used to divide text into words
	CWord* pNewWord = NULL;

	int	iStart = 0; 
	int	iEnd = 0;

//	SCF::LPTSTR	szpWhiteSpace[] = { g_szSpace, g_szTab, g_szNewLine };

	CVector WhiteSpace;
	WhiteSpace.LastAdd(g_Space);
	WhiteSpace.LastAdd(g_Tab);
	WhiteSpace.LastAdd(g_NewLine);

	//Cycle, which divides the text into text-words & white-space words
	do
	{
		CStringRange  StringRange(rText, iStart, m_iTextLength - iStart);
		CStringSearch StringSearch(StringRange);

		CInt CharFound(-1);

		//Search for the sub-strings that specify new paragraphs
		int iIndex = StringSearch.FindStrings(WhiteSpace, &CharFound);
		//bool hSearcbool = FindSubStrings((SCF::LPTSTR)&sText[iStart], m_iTextLength - iStart, szpWhiteSpace, 3, &iCharFound);

		//Compute new ending character position for the new paragraph
		if (iIndex == -1) { iEnd = m_iTextLength; }
		else              { iEnd = iStart + iIndex; }

		//If the word has a size of 0 & the first character is a white-space
		if (!(iEnd - iStart) && (iIndex != -1))
		{
			//Found a new-line char/string 
			if (CharFound == 2)
			{
				iEnd += g_NewLine.Length();

				//Explicitly create a new line word
				pNewWord = new CWord(WORD_LINE_BREAK, TRUE);
				pNewWord->Text(CStringRange(rText, iStart, iEnd - iStart));

				//Store the new tab word
				m_Words.LastAdd(*pNewWord);

				m_iFormattingCharCount += g_NewLine.Length();
			}

			//Found a tab char 
			if (CharFound == 1)
			{
				iEnd += g_Tab.Length();

				pNewWord = new CWord(WORD_TAB, TRUE);
				pNewWord->Text(CStringRange(rText, iStart, iEnd - iStart));

				//Store the new tab word
				m_Words.LastAdd(*pNewWord);
			}

			//Found a space char 
			if (CharFound == 0)
			{
				iEnd += g_Space.Length();

				//CStringRange  StringRange2(rText, iEnd, m_iTextLength - iEnd);
				//CStringSearch StringSearch2(StringRange2);

				//While the next space is located right after the current
				//while ((iIndex = StringSearch.FindString(g_Space)) == 0)
				//while ((hSearcbool = FindSubStrings((SCF::LPTSTR)&sText[iEnd], m_iTextLength - iEnd, &g_szSpace, 1, NULL)) == 0)

				while (rText[iEnd] == g_Space[0])
				{
					//..move end to add it into current word 
					iEnd += g_Space.Length();
				}
		
				//Create a new white-space word (starting character position is set in the previous cycle, or before the cycle starts) 
				pNewWord = new CWord(WORD_SPACE, TRUE);
				pNewWord->Text(CStringRange(rText, iStart, iEnd - iStart));

				//Store the new white-space word
				m_Words.LastAdd(*pNewWord);
			}

			//Compute starting character position for the next word & continue with next word
			iStart = iEnd;
			continue;
		}
	
		//Create a new text-word (starting character position is set in the previous cycle, or before the cycle starts) 
		pNewWord = new CWord(WORD_NORMAL, TRUE);
		pNewWord->Text(CStringRange(rText, iStart, iEnd - iStart));

		//Store the new text-word
		m_Words.LastAdd(*pNewWord);

		//Compute starting character position for the next word
		iStart = iEnd;
	}
	while (iStart < m_iTextLength);

	if ((m_Words.Size() == 1) && (((CWord&)m_Words[0]).Type() == WORD_LINE_BREAK))
	{
		SCF::TCHAR sText[1] = { 10 };

		//Implicitly create a new line word
		pNewWord = new CWord(WORD_LINE_BREAK, FALSE);
		pNewWord->Text(sText);

		//Store the new line-break word
		m_Words.LastAdd(*pNewWord);
	}

	//!!!!!! UNISCRIBE ITEMIZATION !!!!!!
	PrepareUniscribeBuffers(m_iTextLength);

	//Decompose the paragraph into separate runs
	HRESULT hResult = ScriptItemize(rText.Value(), m_iTextLength, s_Uniscribe.iMaxItems, NULL, NULL, s_Uniscribe.pScriptItem, &s_Uniscribe.iGeneratedItems);

	//Compute initial new enlarged paragraph size
	int	TempNewTextLength = (int)(m_iTextLength * 1.5);

	//If buffers are too small, keep enlarging them, until [ScriptItemize(..)] no longer returns [E_OUTOFMEMORY]
	while (hResult == E_OUTOFMEMORY)
	{
		//Resize buffers & try itemization
		PrepareUniscribeBuffers(TempNewTextLength);
		hResult = ScriptItemize(rText.Value(), m_iTextLength, s_Uniscribe.iMaxItems, NULL, NULL, s_Uniscribe.pScriptItem, &s_Uniscribe.iGeneratedItems);

		//Compute new enlarged paragraph size for next cycle
		TempNewTextLength = (int)(TempNewTextLength * 1.5);
	}

	//Check if the selected font supports all characters - this is not yet necessary
//	HRESULT hResult = ScriptCMap(hDC, &pScriptCache, pSubString, iTextLength, NULL, ipGlyphIndex);

	int l_iCurChar = 0;
	int iCurItem = 0;

	//Store itemization results in words
	for (SCF::UINT i = 0; i < m_Words.Size(); i++)
	{
		//Add a item boundary entry a the beginning of each word
		((CWord&)m_Words[i]).ItemBoundary(0, s_Uniscribe.pScriptItem[iCurItem].a);

		//Look for all items, that belong into the current word
		while ((s_Uniscribe.pScriptItem[iCurItem].iCharPos > l_iCurChar) && (s_Uniscribe.pScriptItem[iCurItem].iCharPos < (l_iCurChar + (int)((CWord&)m_Words[i]).TextLength())))
		{
			//Store itemization info, increase item index
			((CWord&)m_Words[i]).ItemBoundary((int)(s_Uniscribe.pScriptItem[iCurItem].iCharPos - l_iCurChar), s_Uniscribe.pScriptItem[iCurItem].a);

			iCurItem++;
		}

		//Move character position to the next word
		l_iCurChar += ((CWord&)m_Words[i]).TextLength();
	}
	return TRUE;
}

bool CParagraph::PrepareUniscribeBuffers(_IN int iParagraphLength)
{
	//Enlarge run dependent buffers if necessary
	if (iParagraphLength > (int)s_Uniscribe.iMaxItems)
	{
		//Compute new item count
		s_Uniscribe.iMaxItems = __max(2, iParagraphLength);

		//Reallocate item buffer
		s_Uniscribe.pScriptItem = (SCRIPT_ITEM*)CMemory::Reallocate(s_Uniscribe.pScriptItem, sizeof(SCRIPT_ITEM) * s_Uniscribe.iMaxItems);
	}
	return TRUE;
}

bool CParagraph::PushStyle()
{
	if (m_pPushStyle)
	{
		//Push stack & execute paragraph style
		CParagraphStyleStack::PushStyleStack(*m_pPushStyle);
		CParagraphStyleStack::ExecuteStyle  (*m_pPushStyle);
		return TRUE;
	}
	return S_FALSE;
}

bool CParagraph::PopStyle()
{
	if (m_pPopStyle)
	{ 
		//Pop style stacks
		CParagraphStyleStack::PopStyleStack(*m_pPopStyle); 
		return TRUE;
	}
	return S_FALSE;
}

bool CParagraph::ResetLayout()
{
	//Go trough all visual parts
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{ 
		//Clear line list
		((CParagraphVisualPart&)m_VisualParts[i]).DeleteLines();
	}
	m_VisualParts.AllDelete();

	//Go trough all words
	for (SCF::UINT i = 0; i < m_Words.Size(); i++) 
	{
		((CWord&)m_Words[i]).ResetLayout(); 
	}
	return TRUE;
}

bool CParagraph::ResetWordLayout()
{
	//Reset layout status
	for (SCF::UINT i = 0; i < m_Words.Size(); i++)
	{
		((CWord&)m_Words[i]).ResetLayout();
	}

	return TRUE;
}

bool CParagraph::ResetWordRenderStatus()
{
	//Reset layout status
	for (SCF::UINT i = 0; i < m_Words.Size(); i++)
	{
		((CWord&)m_Words[i]).RenderDone(FALSE);
	}

	return TRUE;
}

bool CParagraph::ProcessEmptyParagraph(_IN SCFGraphics::Rect4i* pRect, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eFirstBaseline, _IN _OPT int iFirstBaselineOffset, _OUT SCF::DWORD* dwpOutLayoutResults)
{
	UNREFERENCED_PARAMETER(iFirstBaselineOffset);
	UNREFERENCED_PARAMETER(eFirstBaseline);
	UNREFERENCED_PARAMETER(dwLayoutOptions);

	//Check arguments
	if (!pRect)               { SCFError(ErrorInvalidArgument); return FALSE; }
	if (!dwpOutLayoutResults) { SCFError(ErrorInvalidArgument); return FALSE; }

	//..apply styles & reset layout status on all words
	PushStyle();
	ResetWordLayout();

	//Go trough all visual parts
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{ 
		//Clear line list
		((CParagraphVisualPart&)m_VisualParts[i]).DeleteLines();
	}
	m_VisualParts.AllDelete();

	CParagraphVisualPart* l_NewVisPart = new CParagraphVisualPart();

	//Apply paragraph styles [PP_INDENT_LEFT, PP_INDENT_RIGHT, PP_SPACE_BEFORE]
	l_NewVisPart->X(pRect->iX + CParagraphStyleStack::Top().Indent.iLeft);
	l_NewVisPart->Y(pRect->iY + CParagraphStyleStack::Top().Indent.iBefore);
	l_NewVisPart->Width(pRect->iWidth - (CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight));
	l_NewVisPart->Height(CParagraphStyleStack::Top().Indent.iBefore + CParagraphStyleStack::Top().Indent.iAfter);

	//Initialize required member variables
	m_iWordsVisible = 0;

	m_VisualParts.LastAdd(*l_NewVisPart);

	*dwpOutLayoutResults = LR_FULLY_FINISHED;

	PopStyle();
	return TRUE;
}

//Used during measurement of multi-column frames
//This function assumes that layout has been done without vertical fitting
/*bool CParagraph::QueryLayoutWithSameLines(_IN int iFirstColumnOccupiedHeight, _IN int iColumnHeight, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eFirstBaseline, _IN _OPT int iFirstBaselineOffset, _OUT int* ipOutLinesInLastVisPart, _OUT int* ipOutHeightInLastVisPart, _OUT int* ipOutColumnsUsed)
{
	*ipOutLinesInLastVisPart	= 0;
	*ipOutHeightInLastVisPart	= 0;
	*ipOutColumnsUsed			= 0;

	int l_iHeightleft			= iColumnHeight;
	int	l_iOccupiedHeight		= iFirstColumnOccupiedHeight;

	list <CLine*>::iterator curLine_I = m_VisualParts.back()->LinesBegin();

	for (; curLine_I != m_VisualParts.back()->LinesEnd(); curLine_I++)
	{
		if ((l_iHeightleft - l_iOccupiedHeight) > (*curLine_I)->Leading())
		{
			l_iHeightleft				-= (*curLine_I)->Leading();

			*ipOutHeightInLastVisPart	+= (*curLine_I)->Leading();
			*ipOutLinesInLastVisPart++;
		}
		else
		{
			l_iOccupiedHeight			= 0;
			l_iHeightleft				= iColumnHeight;

			*ipOutHeightInLastVisPart	= 0;
			*ipOutLinesInLastVisPart	= 0;

			*ipOutColumnsUsed++;
		}
	}

	return TRUE;
}*/

bool CParagraph::LayOut(_IN SCFGraphics::Rect4i* pRect, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eFirstBaseline, _IN _OPT int iFirstBaselineOffset, _OUT SCF::DWORD* dwpOutLayoutResults, _OUT int* fpOutWidestWordWidth)
{
	//Check arguments
	if (!pRect || !dwpOutLayoutResults) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Handle empty paragraphs
	if (!m_Words.Size()) { return FALSE; }

	//If this is an empty paragraph, invoke empty paragraph handler
	//if (m_Words.front()->Type() == WORD_PARAGRAPH_BREAK) { return ProcessEmptyParagraph(pRect, dwLayoutOptions, eFirstBaseline, iFirstBaselineOffset, dwpOutLayoutResults); }

	//Temporary variables
	int l_iCurHeight = 0;

	bool l_bStop = FALSE;
	bool l_bLineFirst = FALSE; 
	bool l_bFirstBaseLine = (dwLayoutOptions & LO_FIRST_BASELINE) ? TRUE : FALSE; 

	SCFGraphics::Rect4i	l_CurLineRect;
	CLine* l_pCurLine = NULL;
	SCF::DWORD l_dwLineLayoutResults = NULL;

	bool bLayoutResult = FALSE;

	//Prepare output layout status variable
	*dwpOutLayoutResults = NULL;

	//Prepare a new visual part
	CParagraphVisualPart* l_NewVisPart = new CParagraphVisualPart();
	l_NewVisPart->BoundingBox(*pRect);
	l_NewVisPart->Parent(this);

	//Prepare word iterator
//	list <CWord*>::iterator	word_I = m_LayoutEnd_I;
	SCF::UINT uiWord = m_iWordsVisible;

	//If we are starting over..
	if (!(dwLayoutOptions & LO_CONTINUE)) 
	{
		l_bLineFirst = TRUE; 

		//..reset the word iterator
		uiWord = 0;

		//Apply styles & reset layout status on all words
		PushStyle();
		ResetWordLayout();

		//If we don't want to 'visualy' continue the paragraph..
		if (!(dwLayoutOptions & LO_AS_CONTINUED) && !(dwLayoutOptions & LO_IGNORE_SPACE_BEFORE)) 
		{
			//..apply paragraph style [PP_SPACE_BEFORE]
			l_NewVisPart->TranslateY(CParagraphStyleStack::Top().Indent.iBefore); 
			l_NewVisPart->AddToHeight(-CParagraphStyleStack::Top().Indent.iBefore);
		}

		for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
		{ 
			//Clear line list
			((CParagraphVisualPart&)m_VisualParts[i]).DeleteLines();
		}
		m_VisualParts.AllDelete();

		//..reset layout end iterator
		m_iWordsVisible = 0;
	}
	else
	{
		bool l_PreviousVisPartsEmpty = TRUE;

		//Go trough all visual parts..
		for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
		{
			//Check, whether this part is empty
			if (((CParagraphVisualPart&)m_VisualParts[i]).LineCount() > 0) { l_PreviousVisPartsEmpty = FALSE; }
		}

		//Apply styles specific to the first line, if all previous visual parts were empty
		if (l_PreviousVisPartsEmpty)
		{
			l_bLineFirst = TRUE;
		}
	}

	//Look for previously laid out lines
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++) 
	{
		//If there already is a first line, DON'T apply [PP_FIRSTLINE_INDENT]
		if (((CParagraphVisualPart&)m_VisualParts[i]).LineCount()) { l_bLineFirst = FALSE; }
	}

	//Apply paragraph styles [PP_INDENT_LEFT, PP_INDENT_RIGHT]
	l_NewVisPart->TranslateX( CParagraphStyleStack::Top().Indent.iLeft);
	l_NewVisPart->AddToWidth(-(CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight));

	SCFGraphics::Rect4i l_LineLayoutRect = l_NewVisPart->BoundingBox();

	while (!l_bStop && (uiWord < m_Words.Size()))
	{
		//Prepare a new line
		l_pCurLine = new CLine();

		if (l_bLineFirst || l_bFirstBaseLine) 
		{
			//Temporary variables
			SCF::DWORD l_dwLayoutOptions = NULL;
			SCF::ENUM  l_eFirstBaseline  = BL_LEADING;

			//Set temporary variables based on the needed output/action
			if (l_bLineFirst)     { l_dwLayoutOptions = LO_FIRSTLINE; }
			if (l_bFirstBaseLine) { l_eFirstBaseline = eFirstBaseline; }

			if (l_bLineFirst && !(dwLayoutOptions & LO_AS_CONTINUED))
			{
				//Apply paragraph style [PP_FIRSTLINE_INDENT]
				l_LineLayoutRect.iX     += CParagraphStyleStack::Top().Indent.iLineFirst;
				l_LineLayoutRect.iWidth -= CParagraphStyleStack::Top().Indent.iLineFirst;
			}

			bLayoutResult = l_pCurLine->LayOut(&l_LineLayoutRect, dwLayoutOptions, CVectorRange(m_Words, uiWord), CParagraphStyleStack::Top().eAlignHorizontal, l_eFirstBaseline, iFirstBaselineOffset, &l_dwLineLayoutResults, &uiWord);

			if (l_bLineFirst && !(dwLayoutOptions & LO_AS_CONTINUED))
			{
				//Reset horizontal measurements of the line layout rectangle
				l_LineLayoutRect.iX     = l_NewVisPart->X();
				l_LineLayoutRect.iWidth = l_NewVisPart->Width();
			}

			l_bFirstBaseLine = FALSE;
			l_bLineFirst     = FALSE;
		}
		else { bLayoutResult = l_pCurLine->LayOut(&l_LineLayoutRect, dwLayoutOptions, CVectorRange(m_Words, uiWord), CParagraphStyleStack::Top().eAlignHorizontal, BL_LEADING, iFirstBaselineOffset, &l_dwLineLayoutResults, &uiWord); }

		//Stop if failed to properly process the line, or nothing inside the line could be laid-out
		if (!bLayoutResult || (l_dwLineLayoutResults & LR_NOTHING_FINISHED)) 
		{
			delete l_pCurLine; break; 
		}

		//Obtain line measurements
		l_pCurLine->Rect(&l_CurLineRect);

		//Quit laying out lines, if they no longer fit into the layout/bounding rectangle & mark result
		if ((dwLayoutOptions & LO_FIT_VERTICALLY) && (l_dwLineLayoutResults & LR_FIT_FAILED_VERTICAL))
		{
			*dwpOutLayoutResults |= LR_FIT_FAILED_VERTICAL; 
			
			delete l_pCurLine; break; 
		}

		//If fitting is disabled, width is summed up during line layout
		if (!(dwLayoutOptions & LO_FIT_HORIZONTALLY)) 
		{
			//If we are laying out the first line, measure it WITH the first-line indent
			if (!l_NewVisPart->LineCount()) { l_NewVisPart->Width(__max(l_NewVisPart->Width(), l_CurLineRect.iWidth + (CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight) + CParagraphStyleStack::Top().Indent.iLineFirst)); }
			else                            { l_NewVisPart->Width(__max(l_NewVisPart->Width(), l_CurLineRect.iWidth + (CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight))); }
		}

		//Update variables
		l_iCurHeight += l_CurLineRect.iHeight;

		//Prepare bounding/layout rectangle for next line
		l_LineLayoutRect.iY      += l_CurLineRect.iHeight;
		l_LineLayoutRect.iHeight -= l_CurLineRect.iHeight;

		//Add the line into the list
		l_NewVisPart->AddLine(*l_pCurLine);

		if (l_dwLineLayoutResults & LR_LAYOUT_ABORTED) { l_bStop = TRUE; }
		if (l_dwLineLayoutResults & LR_FULLY_FINISHED) { l_bStop = TRUE; }
	}

	//Store iterator used for rendering
	m_iWordsVisible = uiWord;

	//Store dimensions
	l_NewVisPart->Y(pRect->iY);
	l_NewVisPart->X(pRect->iX);
	l_NewVisPart->Height(l_iCurHeight);

	//If horizontal fitting was requested, set fixed paragraph width
	if (dwLayoutOptions & LO_FIT_HORIZONTALLY) { l_NewVisPart->Width(pRect->iWidth); }

	//If we are starting over..
	if (!(dwLayoutOptions & LO_CONTINUE)) 
	{
		//If we don't want to 'visualy' continue the paragraph..
//		if (!(dwLayoutOptions & LO_AS_CONTINUED)) 
		if (!(dwLayoutOptions & LO_AS_CONTINUED) && !(dwLayoutOptions & LO_IGNORE_SPACE_BEFORE)) 
		{
			//..apply paragraph style [PP_SPACE_BEFORE]
			l_NewVisPart->AddToHeight(CParagraphStyleStack::Top().Indent.iBefore);
		}
	}

	//Mark the overall results of the layout process, if the layout is finished, reset iterators & apply style [PP_SPACE_AFTER] 
	if (uiWord == m_Words.Size())													
	{
		if (fpOutWidestWordWidth)
		{
			*fpOutWidestWordWidth = 0;

			//Reset layout status
			for (SCF::UINT i = 0; i < m_Words.Size(); i++)
			{
				//Measure width only for non-whitespace words
				if (((CWord&)m_Words[i]).IsWhiteSpace()) { continue; }

				//Add the first-line indent to the width of the first word for proper results
				if (uiWord == 0) { *fpOutWidestWordWidth = __max(*fpOutWidestWordWidth, ((CWord&)m_Words[i]).VisualPartLast().BoundingBox().iWidth + CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight + CParagraphStyleStack::Top().Indent.iLineFirst); }
				else            { *fpOutWidestWordWidth = __max(*fpOutWidestWordWidth, ((CWord&)m_Words[i]).VisualPartLast().BoundingBox().iWidth + CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight); }
			}
		}

		//Mark layout result
		*dwpOutLayoutResults |= LR_FULLY_FINISHED; 
	
		//Apply style [PP_SPACE_AFTER]
		if (!(dwLayoutOptions & LO_IGNORE_SPACE_AFTER))
		{
			l_NewVisPart->AddSpaceAfter(CParagraphStyleStack::Top().Indent.iAfter); 
		}
	
		//We are done with the layout, so pop paragraph style 
		PopStyle(); 
	}
		
	if (uiWord == 0)                               { *dwpOutLayoutResults |= LR_NOTHING_FINISHED; }
	if ((uiWord != 0) && (uiWord < m_Words.Size())) { *dwpOutLayoutResults |= LR_PARTIALY_FINISHED; }

	//Apply style [PP_SPACE_AFTER] & pass the layout abort flag up, to the caller
	if (l_dwLineLayoutResults & LR_LAYOUT_ABORTED)						
	{
		*dwpOutLayoutResults |= LR_LAYOUT_ABORTED;

		//Apply style [PP_SPACE_AFTER]
		if (!(dwLayoutOptions & LO_IGNORE_SPACE_AFTER))
		{
			l_NewVisPart->AddSpaceAfter(CParagraphStyleStack::Top().Indent.iAfter); 
		}
	}

	//Increase laid-out paragraph count
	if (!m_VisualParts.Size()) { s_iParagraphsLaidOut++; }

	if (dwLayoutOptions & LO_SHRINK_HORIZONTALLY)
	{
		// Store the actual maximum width instead of the desired width
		l_NewVisPart->Width(0);

		//Go trough all lines
		//for (list <CLine*>::iterator line_I = l_NewVisPart->LinesBegin(); line_I != l_NewVisPart->LinesEnd(); line_I++)
		for (SCF::UINT i = 0; i < l_NewVisPart->Lines().Size(); i++)
		{
			CLine* pLine = &(CLine&)l_NewVisPart->Lines()[i];

			// line layout rectangle
			pLine->Rect(&l_CurLineRect);

			//If we are measuring the first line in the paragraph..
			if (!(dwLayoutOptions & LO_CONTINUE) && !(dwLayoutOptions & LO_AS_CONTINUED) && (i == 0)) 
			{
				//..apply the [PP_FIRSTLINE_INDENT] style property & update maximum width
				l_NewVisPart->Width(__max(l_NewVisPart->Width(), l_CurLineRect.iWidth + CParagraphStyleStack::Top().Indent.iLineFirst));
			}
			else
			{
				//..otherwise just update maximum width
				l_NewVisPart->Width(__max(l_NewVisPart->Width(), l_CurLineRect.iWidth));
			}
		}

		//Apply the [PP_INDENT_LEFT] & [PP_INDENT_RIGHT] paragraph style properties
		l_NewVisPart->AddToWidth(CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight);
	}

	//Store the new visual part in the list
	m_VisualParts.LastAdd(*l_NewVisPart);

	return TRUE;
}

bool CParagraph::Render(_IN SCFGraphics::Rect4i* pUpdateRect)
{
	if (m_bRenderDone) { return TRUE; }

	m_bRenderDone = TRUE;

	if (m_VisualParts.IsEmpty()) { return FALSE; }

	//Prepare style state
	if (m_pPushStyle)
	{
		//Execute paragraph style
		CParagraphStyleStack::PushStyleStack(*m_pPushStyle);
		CParagraphStyleStack::ExecuteStyle  (*m_pPushStyle);
	}

	ResetWordRenderStatus();

	if (CCache::dwRenderOptions & RO_DRAW_PARAGRAPH_OUTLINES)
	{
		Pen pen(Color(SCF_ARGB(255, 196, 196, 196)));
		pen.SetDashStyle(DashStyleDot);

		//Go trough all visual parts
		for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
		{
			CCache::pG->DrawRectangle(&pen, ((CParagraphVisualPart&)m_VisualParts[i]).X() * 0.001f, ((CParagraphVisualPart&)m_VisualParts[i]).Y() * 0.001f, ((CParagraphVisualPart&)m_VisualParts[i]).Width() * 0.001f, ((CParagraphVisualPart&)m_VisualParts[i]).Height() * 0.001f); 
		}

		//If there is just one rect, apply all styles at once
	/*	if ((m_VisualParts.Size() == 1) && (m_LayoutEnd_I == m_Words.end()))
		{
			CCache::pG->DrawRectangle(&pen, (m_VisualParts.back()->X() + CParagraphStyleStack::Top().Indent.iLeft) * 0.001f, (m_VisualParts.back()->Y() + CParagraphStyleStack::Top().Indent.iBefore) * 0.001f, (m_VisualParts.back()->Width() - (CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight)) * 0.001f, (m_VisualParts.back()->Height() - (CParagraphStyleStack::Top().Indent.iBefore + CParagraphStyleStack::Top().Indent.iAfter)) * 0.001f); 
		}
		else
		{
			//Go trough all visual parts
			for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
			{
				//Apply the [PP_SPACE_BEFORE] style to the first rectangle
				if (visPart_I == m_VisualParts.begin()) 
				{ 
					CCache::pG->DrawRectangle(&pen, (((CParagraphVisualPart&)m_VisualParts[i]).X() + CParagraphStyleStack::Top().Indent.iLeft) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Y() + CParagraphStyleStack::Top().Indent.iBefore) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Width() - (CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight)) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Height() - CParagraphStyleStack::Top().Indent.iBefore) * 0.001f); 
				}
				else
				{
					vector <CParagraphVisualPart*>::iterator tmp_I = visPart_I; tmp_I++;

					//Apply the [PP_SPACE_AFTER] style to the last rectangle, but only if all words have been laid-out
					if (tmp_I == m_VisualParts.end() && (m_LayoutEnd_I == m_Words.end())) 
					{ 
						CCache::pG->DrawRectangle(&pen, (((CParagraphVisualPart&)m_VisualParts[i]).X() + CParagraphStyleStack::Top().Indent.iLeft) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Y()) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Width() - (CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight)) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Height() - CParagraphStyleStack::Top().Indent.iAfter) * 0.001f); 
					}
					else
					{
						//Apply only [PP_INDENT_LEFT, PP_INDENT_RIGHT], as in the other cases
						CCache::pG->DrawRectangle(&pen, (((CParagraphVisualPart&)m_VisualParts[i]).X() + CParagraphStyleStack::Top().Indent.iLeft) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Y()) * 0.001f, (((CParagraphVisualPart&)m_VisualParts[i]).Width() - (CParagraphStyleStack::Top().Indent.iLeft + CParagraphStyleStack::Top().Indent.iRight)) * 0.001f, ((CParagraphVisualPart&)m_VisualParts[i]).Height() * 0.001f);
					}
				}
			}
		}*/
	}

	//Go trough all visual parts
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		//Go trough all lines
		//for (list <CLine*>::iterator line_I = ((CParagraphVisualPart&)m_VisualParts[i]).LinesBegin(); line_I != ((CParagraphVisualPart&)m_VisualParts[i]).LinesEnd(); line_I++)
		for (SCF::UINT j = 0; j < ((CParagraphVisualPart&)m_VisualParts[i]).Lines().Size(); j++)
		{
			//Render line background
			((CParagraphVisualPart&)m_VisualParts[i]).Line(j).RenderBackground(CParagraphStyleStack::Top().BackgroundColor);
		}
	}

	//Go trough all visual parts
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		//Go trough all lines
		//for (list <CLine*>::iterator line_I = ((CParagraphVisualPart&)m_VisualParts[i]).LinesBegin(); line_I != ((CParagraphVisualPart&)m_VisualParts[i]).LinesEnd(); line_I++)
		for (SCF::UINT j = 0; j < ((CParagraphVisualPart&)m_VisualParts[i]).Lines().Size(); j++)
		{
			//Render line
			if (!((CParagraphVisualPart&)m_VisualParts[i]).Line(j).Render(pUpdateRect)) { return FALSE; }
		}
	}

	//Pop style stacks, if needed
	if (m_pPopStyle) 
	{
		CParagraphStyleStack::PopStyleStack(*m_pPopStyle); 
	}

	return TRUE;
}

bool CParagraph::StyleEntriesClear(_IN int iCharPos, _IN bool bClearPushEntries, _IN bool bClearPopEntries, _IN SCF::ENUM eType)
{
	switch (eType)
	{
	case STYLE_CHARACTER:
		{
			int l_iCurChar = 0;

			//Go trough all words
			//for (list <CWord*>::iterator listIterator = m_Words.begin(); listIterator != m_Words.end(); listIterator++)
			for (SCF::UINT i = 0; i < m_Words.Size(); i++)
			{
				//Find the word, that should contain the style entries
				if ((iCharPos >= l_iCurChar) && (iCharPos <= (l_iCurChar + ((CWord&)m_Words[i]).TextLength())))
				{
					//Clear all style entries
					((CWord&)m_Words[i]).StyleEntriesClear((int)(iCharPos - l_iCurChar), bClearPushEntries, bClearPopEntries);
					return TRUE;
				}

				l_iCurChar += ((CWord&)m_Words[i]).TextLength();
			}
			SCFError(ErrorUnexpectedCodeExecution); return FALSE;
		}

	case STYLE_PARAGRAPH:
		{
			//Reset style variables
			if (bClearPushEntries)	{ m_pPushStyle = NULL; }
			if (bClearPopEntries)	{ m_pPopStyle  = NULL; }

			return TRUE;
		}
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

bool CParagraph::StyleEntryClear(_IN CStyle& rStyle, _IN int iCharPos, _IN bool bClearPushEntries, _IN bool bClearPopEntries)
{
	switch (rStyle.Type())
	{
	case STYLE_CHARACTER:
		{
			int l_iCurChar = 0;

			//Go trough all words
			//for (list <CWord*>::iterator listIterator = m_Words.begin(); listIterator != m_Words.end(); listIterator++)
			for (SCF::UINT i = 0; i < m_Words.Size(); i++)
			{
				//Find the word, that should receive the style entry
				if ((iCharPos >= l_iCurChar) && (iCharPos <= (l_iCurChar + ((CWord&)m_Words[i]).TextLength())))
				{
					//Set a style entry
					((CWord&)m_Words[i]).StyleEntryClear((int)(iCharPos - l_iCurChar), rStyle, bClearPushEntries, bClearPopEntries);
					return TRUE;
				}

				l_iCurChar += ((CWord&)m_Words[i]).TextLength();
			}
			SCFError(ErrorUnexpectedCodeExecution); return FALSE;
		}

	case STYLE_PARAGRAPH:
		{
			//Reset style variables
			if (bClearPushEntries)	{ m_pPushStyle = NULL; }
			if (bClearPopEntries)	{ m_pPopStyle  = NULL; }

			return TRUE;
		}
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

bool CParagraph::StyleEntry(_IN CStyle& rStyle, _IN int iCharPos, _IN bool bEntryTypePush)
{
	switch (rStyle.Type())
	{
	case STYLE_CHARACTER:
		{
			int l_iCurChar = 0;

			//Go trough all words
			//for (list <CWord*>::iterator listIterator = m_Words.begin(); listIterator != m_Words.end(); listIterator++)
			for (SCF::UINT i = 0; i < m_Words.Size(); i++)
			{
				//Find the word, that should receive the style entry
				if ((iCharPos >= l_iCurChar) && (iCharPos <= (l_iCurChar + ((CWord&)m_Words[i]).TextLength())))
				{
					//Set a style entry
					((CWord&)m_Words[i]).StyleEntry((int)(iCharPos - l_iCurChar), rStyle, bEntryTypePush);
					return TRUE;
				}

				l_iCurChar += ((CWord&)m_Words[i]).TextLength();
			}
			SCFError(ErrorUnexpectedCodeExecution); return FALSE;
		}

	case STYLE_PARAGRAPH:
		{
			//Set style variables
			if (bEntryTypePush)	{ m_pPushStyle = &rStyle; }
			else                { m_pPopStyle  = &rStyle; }

			return TRUE;
		}
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

/*bool CParagraph::LineCount(_IN int iVisPart)
{
	//Look for bad arguments
	if (iVisPart < -1)							{ SCFError(ErrorInvalidArgument); return FALSE; }
	if (iVisPart >= (int)m_VisualParts.Size())	{ SCFError(ErrorInvalidArgument); return FALSE; }

	if (m_VisualParts.empty())					{ return FALSE; }

	if (iVisPart == -1)
	{
		return (bool)m_VisualParts.back().LineList.Size();
	}

	//Obtain the iterator of the requested rect
	vector <CParagraphVisualPart*>::iterator visPart_I = m_VisualParts.begin();
	for (int iRect = 0; iRect < iVisPart; iRect++, visPart_I++) {}

	return (bool)((CParagraphVisualPart&)m_VisualParts[i]).LineCount();
}*/

int CParagraph::WordCount(SCF::DWORD dwWordTypes)
{
	int iCount = 0;

	for (SCF::UINT i = 0; i < m_Words.Size(); i++)
	{
		if (dwWordTypes & ((CWord&)m_Words[i]).Type())
		{
			iCount++;
		}
	}

	return iCount;
}

bool CParagraph::CursorPosToCharacterPos(_IN int iX, _IN int iY, _OUT int* ipOutCharPos)
{
	SCFGraphics::Rect4i l_TempRect;

	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		l_TempRect = ((CParagraphVisualPart&)m_VisualParts[i]).BoundingBox();

		_PENDING;
		//if (PointInRect(iX, iY, l_TempRect))
		{
			int l_iCurChar = 0;

			for (SCF::UINT i = 0; i < m_Words.Size(); i++)
			{
				if (((CWord&)m_Words[i]).CursorPosToCharacterPos(iX, iY, ipOutCharPos))
				{
					*ipOutCharPos += l_iCurChar;
					return TRUE;
				}

				l_iCurChar += ((CWord&)m_Words[i]).TextLength();
			}
		}
	}

	return FALSE;
}

//This function adds all rectangles fully enclosing the given selection range to the global cache
bool CParagraph::RealizeSelectionRange(_IN int iStartChar, _IN int iLength)
{
	//Check arguments
	if (!iLength) { return FALSE; }

	int l_iCurChar = 0;

	for (SCF::UINT i = 0; i < m_Words.Size(); i++)
	{
		if (iStartChar > (l_iCurChar + ((CWord&)m_Words[i]).TextLength())) 
		{
			l_iCurChar += ((CWord&)m_Words[i]).TextLength();
			continue; 
		}

		//Compute absolute start & end point
		int iRealizeFrom = __max(l_iCurChar, iStartChar);
		int iRealizeTo   = __min(l_iCurChar + ((CWord&)m_Words[i]).TextLength(), iStartChar + iLength);

		//If there is nothing more to clean
//		if (iRealizeFrom >= iRealizeTo) { return TRUE; }
		if ((iStartChar + iLength) < l_iCurChar) { return TRUE; }

		//Create rectangle(s) for the given character range
		((CWord&)m_Words[i]).RealizeSelectionRange(iRealizeFrom - l_iCurChar, iRealizeTo - iRealizeFrom);
	
		l_iCurChar += ((CWord&)m_Words[i]).TextLength();
	}

	return TRUE;
}

bool CParagraph::IsCursorInText(_IN int iX, _IN int iY)
{
	//Go trough all words
	for (SCF::UINT i = 0; i < m_Words.Size(); i++)
	{
		if (((CWord&)m_Words[i]).IsCursorInText(iX, iY)) { return TRUE; }
	}

	return FALSE;
}