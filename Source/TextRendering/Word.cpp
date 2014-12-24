#include "Word.h"
#include "Constants.h"
#include "Frame.h"
#include "Cache.h"
#include "RunState.h"

using namespace SCFGraphics;
using namespace SCFTextRendering;

//Static class member initialization
int CWord::s_iCharactersLaidOut = 0;
int CWord::s_iFormattingCharsLaidOut = 0;

int CWord::s_iWordsLaidOut = 0;

CWord::CWord(_IN SCF::ENUM eWordType, _IN bool bExplicit)
{
	//Reset object info
	m_eType     = eWordType;
	m_bExplicit = bExplicit;

	m_bLayoutDone = FALSE;
	m_bRenderDone = FALSE;
	m_bMeasurementDone = FALSE;

	m_ipCharWidths = NULL;

	m_uiCharsLaidOut = 0;
	m_uiCurPart = 0;

	m_uiPrevLayoutCharsLaidOut = 0;
	m_uiPrevLayoutCurPart = 0;

	m_bCanRollBack = FALSE;
}

CWord::~CWord()
{
	m_Parts.AllDelete();

	//Free character width array
	if (m_ipCharWidths) { CMemory::Free(m_ipCharWidths); }

	m_VisualParts.AllDelete();
}

bool CWord::IsItemBoundary(CPartInfo& rPartInfo)
{
	for (SCF::UINT i = 0; i < rPartInfo.Entries().Size(); i++)
	{
		if (rPartInfo.Entries()[i].ClassKey() == ClassBoundaryItem) { return TRUE; }
	}

	return FALSE;
}

bool CWord::Text(_IN CString& rText)
{
	m_Text = rText;

	//Realloc character width array
	m_ipCharWidths = (int*)CMemory::Reallocate(m_ipCharWidths, sizeof(int) * m_Text.Length());
	return TRUE;
}

bool CWord::StyleEntriesClear(_IN SCF::UINT uiCharPos, _IN bool bClearPushEntries, _IN bool bClearPopEntries)
{
	//Check arguments
	if (uiCharPos > (int)m_Text.Length()) { SCFError(ErrorInvalidArgument); return FALSE; }

	//If the part-list is empty, quit
	if (!m_Parts.Size()) { return FALSE; }

	//..go through the entire list
	for (SCF::UINT i = 0; i < m_Parts.Size(); i++)
	{
		CPartInfo* pPart = &((CPartInfo&)m_Parts[i]);

		//If a style entry with the given position WAS found...
		if (pPart->CharPos() == uiCharPos)
		{
			//Go through the entire entry list
			for (SCF::UINT j = 0; j < pPart->Entries().Size(); j++)
			{
				//If we found a matching entry..
				if ((pPart->Entries()[j].ClassKey() == ClassBoundaryStyle) &&
					((((CBoundaryStyle&)pPart->Entries()[j]).IsPush() && bClearPushEntries) ||
					(!((CBoundaryStyle&)pPart->Entries()[j]).IsPush() && bClearPopEntries)))
				{
					//..erase it
					pPart->Entries().Delete(j);
					return TRUE;
				}
			}
			return TRUE;
		}
	}

	return FALSE;
}

bool CWord::StyleEntryClear(_IN SCF::UINT uiCharPos, _IN CStyle& rStyle, _IN bool bClearPushEntries, _IN bool bClearPopEntries)
{
	//Check arguments
	if (uiCharPos > (int)m_Text.Length()) { SCFError(ErrorInvalidArgument); return FALSE; }

	//If the part-list is empty, quit
	if (!m_Parts.Size()) { return FALSE; }

	//..go through the entire list
	for (SCF::UINT i = 0; i < m_Parts.Size(); i++)
	{
		CPartInfo* pPart = &((CPartInfo&)m_Parts[i]);

		//If a style entry with the given position WAS found...
		if (pPart->CharPos() == uiCharPos)
		{
			//Go through the entire entry list
			for (SCF::UINT j = 0; j < pPart->Entries().Size(); j++)
			{
				//If we found a matching entry..
				if ((pPart->Entries()[j].ClassKey() == ClassBoundaryStyle) &&
					(&((CBoundaryStyle&)pPart->Entries()[j]).Style() == &rStyle) &&
					((((CBoundaryStyle&)pPart->Entries()[j]).IsPush() && bClearPushEntries) ||
					(!((CBoundaryStyle&)pPart->Entries()[j]).IsPush() && bClearPopEntries)))
				{
					//..erase it
					pPart->Entries().Delete(j);
					return TRUE;
				}
			}
			return TRUE;
		}
	}

	return FALSE;
}

bool CWord::StyleEntry(_IN SCF::UINT uiCharPos, _IN CStyle& rStyle, _IN bool bEntryTypePush)
{
	//Check arguments
	if (uiCharPos > (int)m_Text.Length()) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Create a [CBoundaryStyle] object describing the style entry
	CBoundaryStyle*	pStyleBoundary = new CBoundaryStyle(rStyle, bEntryTypePush);

	//If the part-list is not empty..
	if (m_Parts.Size())
	{
		//..go through the entire list
		for (SCF::UINT i = 0; i < m_Parts.Size(); i++)
		{
			CPartInfo* pPart = &((CPartInfo&)m_Parts[i]);

			//If a style entry with the given position WAS found...
			if (pPart->CharPos() == uiCharPos)
			{
				//Go through the entire entry list
				for (SCF::UINT j = 0; j < pPart->Entries().Size(); j++)
				{
					//If the same entry already exists..
					if ((pPart->Entries()[j].ClassKey() == ClassBoundaryStyle) &&
						(&((CBoundaryStyle&)pPart->Entries()[j]).Style() == &rStyle))
					{
						//..just update entry type & exit
						((CBoundaryStyle&)pPart->Entries()[i]).IsPush(bEntryTypePush);
						return TRUE;
					}
				}

				//An identical entry was not found, so add a new entry into the entry list
				pPart->Entries().LastAdd(*pStyleBoundary);

				return TRUE;
			}

			//If a part boundary with the given position was NOT found...
			if (pPart->CharPos() > uiCharPos)
			{
				//Create new part 
				CPartInfo* pNewPart = new CPartInfo();
				pNewPart->CharPos(uiCharPos);

				//Add style entry into entry-list
				pNewPart->Entries().LastAdd(*pStyleBoundary);

				//Insert a new part into the part list
				m_Parts.Insert(i, *pNewPart);
				return TRUE;
			}
		}
	}

	//Create new part 
	CPartInfo* pNewPart = new CPartInfo();
	pNewPart->CharPos(uiCharPos);

	//Add style entry into entry-list
	pNewPart->Entries().LastAdd(*pStyleBoundary);

	//Insert a new part into the part list
	m_Parts.LastAdd(*pNewPart);
	return TRUE;
}

bool CWord::PushNPopStyles(CVector& rEntries)
{
	//Go through all entries..
	for (SCF::UINT i = 0; i < rEntries.Size(); i++)
	{
		if (rEntries[i].ClassKey() == ClassBoundaryStyle)
		{
			if (((CBoundaryStyle&)rEntries[i]).IsPush())
			{
				//..if we find a PUSH style entry..
				CCharacterStyleStack::PushStyleStack(((CBoundaryStyle&)rEntries[i]).Style());

				//..update style values & ..
				CCharacterStyleStack::ExecuteStyle(((CBoundaryStyle&)rEntries[i]).Style());

			}
			else
			{
				//..if we find a POP style entry..
				CCharacterStyleStack::PopStyleStack(((CBoundaryStyle&)rEntries[i]).Style());
			}
		}
	}

	return TRUE;
}

bool CWord::ItemBoundary(_IN SCF::UINT uiCharPos, _IN SCRIPT_ANALYSIS& scriptAnalysis)
{
	//Check arguments
	if (uiCharPos > (int)m_Text.Length()) { SCFError(ErrorInvalidArgument); return FALSE; }

	//Create a [CBoundaryItem] object describing the item entry
	CBoundaryItem* pItemBoundary = new CBoundaryItem(scriptAnalysis);

	//If the part-list is not empty..
	if (m_Parts.Size())
	{
		//..go through the entire list
		for (SCF::UINT i = 0; i < m_Parts.Size(); i++)
		{
			CPartInfo* pPart = &((CPartInfo&)m_Parts[i]);

			//If a style entry with the given position WAS found...
			if (pPart->CharPos() == uiCharPos)
			{
				//Go through the entire entry list
				for (SCF::UINT j = 0; j < pPart->Entries().Size(); j++)
				{
					//If the same entry already exists..
					if (pPart->Entries()[j].ClassKey() == ClassBoundaryStyle)
					{
						//..delete created [CBoundaryItem] & exit
						delete pItemBoundary;
						return TRUE;
					}
				}

				//An identical entry was not found, so add a new entry into the entry list
				//Make it the first entry in the list !!!
				pPart->Entries().Insert(0, *pItemBoundary);

				return TRUE;
			}

			//If a part boundary with the given position was NOT found...
			if (pPart->CharPos() > uiCharPos)
			{
				//Create new part 
				CPartInfo* pNewPart = new CPartInfo();
				pNewPart->CharPos(uiCharPos);

				//Add style entry into entry-list
				pNewPart->Entries().LastAdd(*pItemBoundary);

				//Insert a new part into the part list
				m_Parts.Insert(i, *pNewPart);

				return TRUE;
			}
		}
	}

	//Create new part 
	CPartInfo* pNewPart = new CPartInfo;
	pNewPart->CharPos(uiCharPos);

	//Add item boundary entry into entry-list
	pNewPart->Entries().LastAdd(*pItemBoundary);

	//If the list is empty, use [LastAdd()] to append the list with the new entry 
	m_Parts.LastAdd(*pNewPart);

	return TRUE;
}

bool CWord::LayOut(_IN SCFGraphics::Rect4i* pRect, _IN SCF::DWORD dwLayoutOptions, _IN _OPT int iMinWidthForSpacesToWrap, _OUT SCF::DWORD* dwpOutLayoutResults)
{
	//Check arguments
	if (!pRect || !dwpOutLayoutResults) 
	{
		_ASSERTE(pRect);
		_ASSERTE(dwpOutLayoutResults);
		SCFError(ErrorInvalidArgument); return FALSE; 
	}

	//Don't do layout twice
	if (!m_bMeasurementDone)
	{
		//Don't do character layout/measurement for non-whitespace(non-breaking) words if not requested
		if ((m_eType != WORD_SPACE) && (dwLayoutOptions & LO_SKIP_CHARACTER_MEASUREMENT))
		{
			//Go through all parts
			for (SCF::UINT i = 0; i < m_Parts.Size(); i++)
			{
				CPartInfo* pPart = &((CPartInfo&)m_Parts[i]);

				//Synchronize style state with the style changes
				PushNPopStyles(pPart->Entries());
				if (!CCharacterStyleStack::UpdateStyleState())
				{
					_ASSERTE(0);
					SCFError(ErrorOutOfMemory); return FALSE; 
				}
			}
		}
		else
		{
			//Go through all parts
			for (SCF::UINT i = 0; i < m_Parts.Size(); i++)
			{
				CPartInfo* pPart = &((CPartInfo&)m_Parts[i]);

				//Synchronize style state with the style changes
				PushNPopStyles(pPart->Entries());
				if (!CCharacterStyleStack::UpdateStyleState())
				{
					_ASSERTE(0);
					SCFError(ErrorOutOfMemory); return FALSE; 
				}

				//Continue the previous item in a new run
				int iLength = 0;


				//Compute length of current run
				if ((i + 1) == m_Parts.Size()) { iLength = m_Text.Length()                        - pPart->CharPos(); }
				else                           { iLength = ((CPartInfo&)m_Parts[i + 1]).CharPos() - pPart->CharPos(); }

				if (IsItemBoundary((CPartInfo&)m_Parts[i])) 
				{
					//Obtain run measurements
					CRunState::MeasureRun(&((CBoundaryItem&)pPart->Entries()[0]).ScriptAnalysis(), &(m_Text.Value()[pPart->CharPos()]), (int)iLength, &CRunState::Run().iWidth,  pPart->PtrLeading(), pPart->PtrDescent(), pPart->PtrAscent(), &m_ipCharWidths[pPart->CharPos()]);
				}
				else
				{
					//Prepare the iterator for the closest PREVIOUS item entry
					SCF::UINT uiPrevious = i - 1;
					while (!IsItemBoundary((CPartInfo&)m_Parts[uiPrevious])) { uiPrevious--; }

					CPartInfo* pPrevious = &((CPartInfo&)m_Parts[uiPrevious]);

					//Obtain the iterator of the entry, which has a [SCRIPT_ANALYSIS] structure
					SCF::UINT uiPreviousEntry = 0;
					for (; uiPreviousEntry < pPrevious->Entries().Size(); uiPreviousEntry++)
					{
						if (pPrevious->Entries()[uiPreviousEntry].ClassKey() == ClassBoundaryItem) { break; }
					}

					CBoundaryItem* pPreviousEntry = &((CBoundaryItem&)pPrevious->Entries()[uiPreviousEntry]);

					//Obtain run measurements - i.e: [SCRIPT_ANALYSIS] structure from the closest previous item entry
					CRunState::MeasureRun(&pPreviousEntry->ScriptAnalysis(), &(m_Text.Value()[pPart->CharPos()]), (int)iLength, &CRunState::Run().iWidth, pPart->PtrLeading(), pPart->PtrDescent(), pPart->PtrAscent(), &m_ipCharWidths[pPart->CharPos()]);
				}
			}
		}

		//Reset visual part list
		m_VisualParts.AllDelete();

		//Reset member variables
		m_uiCharsLaidOut = 0;
		m_uiCurPart      = 0;

		//Reset backup member variables
		m_uiPrevLayoutCharsLaidOut = 0;
		m_uiPrevLayoutCurPart       = 0;

		//Reset status variables
		m_bMeasurementDone = TRUE;
		m_bCanRollBack     = FALSE;
	}

	if (m_bLayoutDone) 
	{
		_ASSERTE(m_VisualParts.Size());
		m_bCanRollBack = FALSE; 
		return TRUE; 
	}

	//Back-up member variables for rollback
	m_uiPrevLayoutCurPart       = m_uiCurPart;
	m_uiPrevLayoutCharsLaidOut = m_uiCharsLaidOut;

	//Reset output results
	*dwpOutLayoutResults = NULL;

	//Prepare a new visual part
	CWordVisualPart* pNewVisPart = new CWordVisualPart();

	pNewVisPart->BoundingBox(*pRect);
	pNewVisPart->StartingCharacter(m_uiCharsLaidOut);

	//Increase laid-out word count, if we are laying-out the first visual part of this word
	if (m_eType == WORD_NORMAL) { if (!m_VisualParts.Size()) { s_iWordsLaidOut++; } }

	//Add new visual part to the list
	m_VisualParts.LastAdd(*pNewVisPart);

	//Reset variables used for measuring
	CRunState::Run().iLeading = 0;
	CRunState::Run().iAscent  = 0;
	CRunState::Run().iDescent = 0;

	CRunState::Run().iWidth = 0;

	int l_iCharsLaidOutThisTime = 0;

	if (m_eType == WORD_LINE_BREAK)
	{
		for (SCF::UINT uiChar = 0; uiChar < m_Text.Length(); uiChar++) { m_ipCharWidths[uiChar] = 0; }
	}

	//While the current char belongs to this rect
	while (!ShouldStopLayingOut(dwLayoutOptions, pRect->iWidth, iMinWidthForSpacesToWrap))
	{
		//Update measurements for this rect
		CRunState::Run().iLeading = __max(CRunState::Run().iLeading, ((CPartInfo&)m_Parts[m_uiCurPart]).Leading());
		CRunState::Run().iAscent  = __max(CRunState::Run().iAscent,  ((CPartInfo&)m_Parts[m_uiCurPart]).Ascent());
		CRunState::Run().iDescent = __max(CRunState::Run().iDescent, ((CPartInfo&)m_Parts[m_uiCurPart]).Descent());

		//Update part width
		CRunState::Run().iWidth += m_ipCharWidths[m_uiCharsLaidOut];

		//Update layout status
		m_uiCharsLaidOut++;
		l_iCharsLaidOutThisTime++;

		if ((m_uiCurPart + 1) < m_Parts.Size())
		{
			//If the next character belongs to the next part, change current part
			if (m_uiCharsLaidOut >= ((CPartInfo&)m_Parts[m_uiCurPart + 1]).CharPos()) { m_uiCurPart++; }
		}
	}

	//If fitting is requested..
	if (dwLayoutOptions & LO_FIT_HORIZONTALLY)
	{
		//..then if either the current visual word part is wider than the layout rect. or we haven't yet laid out all chars, mark failed fitting   
		if ((CRunState::Run().iWidth > pRect->iWidth) || (m_uiCharsLaidOut < m_Text.Length())) { *dwpOutLayoutResults |= LR_FIT_FAILED_HORIZONTAL; }
	}

	//Store width & height measured in the code above
	((CWordVisualPart&)m_VisualParts.Last()).Width (CRunState::Run().iWidth);
	((CWordVisualPart&)m_VisualParts.Last()).Height(CRunState::Run().iLeading + CRunState::Run().iDescent);

	((CWordVisualPart&)m_VisualParts.Last()).Leading(CRunState::Run().iLeading);
	((CWordVisualPart&)m_VisualParts.Last()).Ascent (CRunState::Run().iAscent);
	((CWordVisualPart&)m_VisualParts.Last()).Descent(CRunState::Run().iDescent);

	((CWordVisualPart&)m_VisualParts.Last()).Length(l_iCharsLaidOutThisTime);

	//If the total width of a space is less than the minimum width needed to perform a wrap, update variables, so that they would indicate, that the layout is fully finished  
	if ((m_eType == WORD_SPACE) && (((int)m_Text.Length() * m_ipCharWidths[0]) <= iMinWidthForSpacesToWrap))
	{
		m_uiCharsLaidOut         = m_Text.Length();
		l_iCharsLaidOutThisTime = m_Text.Length();

		m_bLayoutDone = TRUE;
	}

	//Mark layout results
	if (!l_iCharsLaidOutThisTime)                                       { *dwpOutLayoutResults |= LR_NOTHING_FINISHED; }
	if (l_iCharsLaidOutThisTime && (m_uiCharsLaidOut < m_Text.Length())) { *dwpOutLayoutResults |= LR_PARTIALY_FINISHED; }
	if (m_uiCharsLaidOut == m_Text.Length())                             { *dwpOutLayoutResults |= LR_FULLY_FINISHED; m_bLayoutDone = TRUE; }

	//If word splitting is of
	if ((m_eType != WORD_SPACE) && !(dwLayoutOptions & LO_SPLIT_WORDS) && (*dwpOutLayoutResults & LR_FIT_FAILED_HORIZONTAL)) { *dwpOutLayoutResults |= LR_PARTIALY_FINISHED; }

	if ((m_eType != WORD_LINE_BREAK)) { s_iCharactersLaidOut += l_iCharsLaidOutThisTime; }

	_ASSERTE(m_VisualParts.Size());

	m_bCanRollBack = TRUE;
	return TRUE;
}

bool CWord::ShouldStopLayingOut(_IN SCF::DWORD dwLayoutOptions, _IN int iLayoutWidth, _IN int iMinWidthForSpacesToWrap)
{
	UNREFERENCED_PARAMETER(iMinWidthForSpacesToWrap);

	if (dwLayoutOptions & LO_FIT_HORIZONTALLY)
	{
		if ((dwLayoutOptions & LO_SPLIT_WORDS) || (m_eType == WORD_SPACE)) 
		{
			if ((m_uiCharsLaidOut < m_Text.Length()) && ((CRunState::Run().iWidth + m_ipCharWidths[m_uiCharsLaidOut]) <= iLayoutWidth)) { return FALSE; }
			return TRUE;
		}
		else
		{
			if (m_uiCharsLaidOut < m_Text.Length()) { return FALSE; }
			return TRUE;
		}
	}
	else
	{
		if (m_uiCharsLaidOut < m_Text.Length()) { return FALSE; }
		return TRUE;
	}
}

void CWord::RenderWordOutlines()
{
	Pen pen(Color(SCF_ARGB(48, 0, 0, 0)));

	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		CCache::pG->DrawRectangle(&pen, ((CWordVisualPart&)m_VisualParts[i]).BoundingBox().iX * 0.001f, ((CWordVisualPart&)m_VisualParts[i]).BoundingBox().iY * 0.001f, ((CWordVisualPart&)m_VisualParts[i]).BoundingBox().iWidth * 0.001f, ((CWordVisualPart&)m_VisualParts[i]).BoundingBox().iHeight * 0.001f);
	}
}

//Renders the word into the current layout/bounding rectangle
bool CWord::Render(_IN SCFGraphics::Rect4i* pUpdateRect)
{
	//Don't render twice
	if (m_bRenderDone) { return TRUE; }

	if (CCache::dwRenderOptions & RO_DRAW_WORD_OUTLINES) { RenderWordOutlines(); }

	//Current visual part index
	SCF::UINT uiCVP = 0;

	//This variable contains the visual offset from the start of the current visual part
	int	l_iVisPartRenderOffset  = 0;
	int	l_iVisPartRenderedChars = 0;

	//Go through all parts..
	for (SCF::UINT i = 0; i < m_Parts.Size(); i++)
	{
		CPartInfo* pPart = &((CPartInfo&)m_Parts[i]);

		//Synchronize style state with the style changes
		PushNPopStyles(pPart->Entries());
		if (!CCharacterStyleStack::UpdateStyleState()) { SCFError(ErrorOutOfMemory); return FALSE; }

		//Don't bother with the rendering of invisible words
		if (m_eType == WORD_LINE_BREAK)   { continue; }
		if (m_eType == WORD_COLUMN_BREAK) { continue; }
//		if (m_eType == WORD_SPACE)        { continue; }

		//Continue the previous item in a new run
		int l_iLength = 0;

		//Compute length of current run..
		if ((i + 1) == m_Parts.Size()) { l_iLength = m_Text.Length()                        - pPart->CharPos(); }
		else                           { l_iLength = ((CPartInfo&)m_Parts[i + 1]).CharPos() - pPart->CharPos(); }

		if (IsItemBoundary(((CPartInfo&)m_Parts[i]))) 
		{
			//These variables keep track of the smallest visually & logically continuous part of the word
			int	l_iCharsToRender      = 0;
			int	l_iCharsToRenderWidth = 0;

			//Keeps track of the number of characters from the current logical part, that were already rendered
			int	l_iCharsRendered = 0;

			//While there are chars to lay out & we have a current visual part
			while ((l_iCharsRendered < l_iLength) && (uiCVP < m_VisualParts.Size()))
			{
				while ((l_iCharsToRender < (l_iLength - l_iCharsRendered)) && ((l_iCharsRendered + l_iCharsToRender) < (((CWordVisualPart&)m_VisualParts[uiCVP]).StartingCharacter() + ((CWordVisualPart&)m_VisualParts[uiCVP]).Length())))
				{
					l_iCharsToRenderWidth += m_ipCharWidths[pPart->CharPos() + l_iCharsToRender + l_iCharsRendered];
					l_iCharsToRender++;
				}

				//Render current continuous part
				SCFGraphics::Rect4i l_TempRect = ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox();
				if ((m_eType == WORD_NORMAL) && (!pUpdateRect || (RectanglesIntersect(*pUpdateRect, l_TempRect)))) { CRunState::RenderRun(&((CBoundaryItem&)pPart->Entries()[0]).ScriptAnalysis(), &(m_Text.Value()[pPart->CharPos() + l_iCharsRendered]), l_iCharsToRender, ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iX + l_iVisPartRenderOffset, ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iY + ((CWordVisualPart&)m_VisualParts[uiCVP]).Leading(), &m_ipCharWidths[pPart->CharPos() + l_iCharsRendered]); }
				if ((m_eType == WORD_SPACE)  && (!pUpdateRect || (RectanglesIntersect(*pUpdateRect, l_TempRect)))) { CRunState::RenderWhitespaceRun(((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iX + l_iVisPartRenderOffset, ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iY + ((CWordVisualPart&)m_VisualParts[uiCVP]).Leading(), &m_ipCharWidths[pPart->CharPos() + l_iCharsRendered], l_iCharsToRender); }

				//Update visual offset & the number of characters drawn
				l_iVisPartRenderOffset  += l_iCharsToRenderWidth;
				l_iCharsRendered        += l_iCharsToRender;
				l_iVisPartRenderedChars += l_iCharsToRender;

				//If we have reached the end of the current visual part..
				if (l_iVisPartRenderedChars >= ((CWordVisualPart&)m_VisualParts[uiCVP]).Length())
				{
					//..prepare the next visual part
					uiCVP++;
					l_iVisPartRenderOffset  = 0;
					l_iVisPartRenderedChars = 0;
				}

				//Reset variables used for counting & measuring characters
				l_iCharsToRender      = 0;
				l_iCharsToRenderWidth = 0;
			}
		}
		else
		{
			//// the iterator for the closes PREVIOUS item entry
			//list <CPartInfo*>::iterator prevPart_I = part_I;
			//while (!IsItemBoundary((*prevPart_I))) { prevPart_I--; }

			//list <CPartEntry>::iterator prevEntry_I;

			////Obtain the iterator of the entry, which has a [SCRIPT_ANALYSIS] structure
			//for (prevEntry_I = (*prevPart_I)->Entries.begin(); prevEntry_I != (*prevPart_I)->Entries.end(); prevEntry_I++)
			//{
			//	if ((*prevEntry_I).eType == ClassBoundaryItem) { break; }
			//}


			//Prepare the iterator for the closest PREVIOUS item entry
			SCF::UINT uiPrevious = i - 1;
			while (!IsItemBoundary((CPartInfo&)m_Parts[uiPrevious])) { uiPrevious--; }

			CPartInfo* pPrevious = &((CPartInfo&)m_Parts[uiPrevious]);

			//Obtain the iterator of the entry, which has a [SCRIPT_ANALYSIS] structure
			SCF::UINT uiPreviousEntry = 0;
			for (; uiPreviousEntry < pPrevious->Entries().Size(); uiPreviousEntry++)
			{
				if (pPrevious->Entries()[uiPreviousEntry].ClassKey() == ClassBoundaryItem) { break; }
			}

			CBoundaryItem* pPreviousEntry = &((CBoundaryItem&)pPrevious->Entries()[uiPreviousEntry]);

			//These variables keep track of the smallest visually & logically continuous part of the word
			int	l_iCharsToRender      = 0;
			int	l_iCharsToRenderWidth = 0;

			//Keeps track of the number of characters from the current logical part, that were already rendered
			int	l_iCharsRendered = 0;

			//While there are chars to lay out & we have a current visual part
			while ((l_iCharsRendered < l_iLength) && (uiCVP < m_VisualParts.Size()))
			{
				while ((l_iCharsToRender < (l_iLength - l_iCharsRendered)) && ((l_iCharsRendered + l_iCharsToRender) < (((CWordVisualPart&)m_VisualParts[uiCVP]).StartingCharacter() + ((CWordVisualPart&)m_VisualParts[uiCVP]).Length())))
				{
					l_iCharsToRenderWidth += m_ipCharWidths[pPart->CharPos() + l_iCharsToRender + l_iCharsRendered];
					l_iCharsToRender++;
				}

				//Render current continuous part
				SCFGraphics::Rect4i l_TempRect = ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox();
				if ((m_eType == WORD_NORMAL) && (RectanglesIntersect(*pUpdateRect, l_TempRect))) { CRunState::RenderRun(&pPreviousEntry->ScriptAnalysis(), &(m_Text.Value()[pPart->CharPos() + l_iCharsRendered]), l_iCharsToRender, ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iX + l_iVisPartRenderOffset, ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iY + ((CWordVisualPart&)m_VisualParts[uiCVP]).Leading(), &m_ipCharWidths[pPart->CharPos() + l_iCharsRendered]); }
				if ((m_eType == WORD_SPACE)  && (RectanglesIntersect(*pUpdateRect, l_TempRect))) { CRunState::RenderWhitespaceRun(                                                                                                   ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iX + l_iVisPartRenderOffset, ((CWordVisualPart&)m_VisualParts[uiCVP]).BoundingBox().iY + ((CWordVisualPart&)m_VisualParts[uiCVP]).Leading(), &m_ipCharWidths[pPart->CharPos() + l_iCharsRendered], l_iCharsToRender); }

				//Update visual offset & the number of characters drawn
				l_iVisPartRenderOffset  += l_iCharsToRenderWidth;
				l_iCharsRendered        += l_iCharsToRender;
				l_iVisPartRenderedChars += l_iCharsToRender;

				//If we have reached the end of the current visual part..
				if (l_iVisPartRenderedChars >= ((CWordVisualPart&)m_VisualParts[uiCVP]).Length())
				{
					//..prepare the next visual part
					uiCVP++;
					l_iVisPartRenderOffset  = 0;
					l_iVisPartRenderedChars = 0;
				}

				//Reset variables used for counting & measuring characters
				l_iCharsToRender      = 0;
				l_iCharsToRenderWidth = 0;
			}
		}
	}

	//Update this variable to prevent further rendering, unless reset
	m_bRenderDone = TRUE;
	return TRUE;
}

//Delete last visual part & restore member variables prior to its layout, allowing it to be laid-out again at a different position, with different size
bool CWord::RollBackLastLayout()
{
	//Check whether a rollback can be performed
	if (!m_VisualParts.Size())	{ return FALSE; }
	if (!m_bCanRollBack)		{ return FALSE; }

	//Restore member variables
	m_uiCurPart      = m_uiPrevLayoutCurPart;
	m_uiCharsLaidOut = m_uiPrevLayoutCharsLaidOut;

	//Decrease laid-out character count statistic
	if (m_bExplicit && (m_eType != WORD_LINE_BREAK)) { s_iCharactersLaidOut -= ((CWordVisualPart&)m_VisualParts.Last()).Length(); }

	//Erase last visual part
	m_VisualParts.LastDelete();

	//Decrease laid-out word count statistic
	if (m_eType == WORD_NORMAL) { if (!m_VisualParts.Size()) { s_iWordsLaidOut--; } }

	//Update member variables to allow layout & deny further rollback
	m_bCanRollBack	= FALSE;
	m_bLayoutDone	= FALSE;
	return TRUE;
}

void CWord::ResetLayout()
{
	//Reset member variables, so that layout can be restarted
	m_bMeasurementDone	= FALSE; 
	m_bLayoutDone		= FALSE;

	m_VisualParts.AllDelete();
}

bool CWord::CursorPosToCharacterPos(_IN int iX, _IN int iY, _OUT int* ipOutCharPos)
{
	//Check arguments
	if (!ipOutCharPos) { return FALSE; }

	if ((m_eType == WORD_TAB) || (m_eType == WORD_LINE_BREAK))
	{
		if (!m_VisualParts.Size()) { return FALSE; }

		SCFGraphics::Rect4i l_TempRect = ((CWordVisualPart&)m_VisualParts[0]).BoundingBox();

		if (PointInRect(iX, iY, l_TempRect)) { *ipOutCharPos = 0; return TRUE; }
		return FALSE;
	}

	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		SCFGraphics::Rect4i l_TempRect = ((CWordVisualPart&)m_VisualParts[i]).BoundingBox();
		if (PointInRect(iX, iY, l_TempRect))
		{
			int fCurLeft = 0;

			for (int iChar = ((CWordVisualPart&)m_VisualParts[i]).StartingCharacter(); iChar < (((CWordVisualPart&)m_VisualParts[i]).StartingCharacter() + ((CWordVisualPart&)m_VisualParts[i]).Length()); iChar++)
			{
				if ((fCurLeft + m_ipCharWidths[iChar]) > (iX - ((CWordVisualPart&)m_VisualParts[i]).BoundingBox().iX))
				{
					*ipOutCharPos = iChar;
					return TRUE;
				}

				fCurLeft += m_ipCharWidths[iChar];
			}
		}
	}

	return FALSE;
}

bool CWord::RealizeSelectionRange(_IN int iStartChar, _IN int iLength)
{
	//Check arguments
	if (!iLength) { return FALSE; }

	if ((m_eType == WORD_TAB) || (m_eType == WORD_LINE_BREAK))
	{
		if (!m_VisualParts.Size()) { return FALSE; }

		CCache::SelectionRectList.LastAdd(*(new CRectInt(((CWordVisualPart&)m_VisualParts[0]).BoundingBox())));
		return TRUE;
	}

	int l_iCurChar = 0;

	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		if (iStartChar > (l_iCurChar + ((CWordVisualPart&)m_VisualParts[i]).Length())) 
		{ 
			l_iCurChar += ((CWordVisualPart&)m_VisualParts[i]).Length();
			continue; 
		}

		//Compute absolute start & end point
		int iRealizeFrom	= __max(l_iCurChar, iStartChar);
		int iRealizeTo		= __min(l_iCurChar + ((CWordVisualPart&)m_VisualParts[i]).Length(), (int)(iStartChar + iLength));

		//If there is nothing more to clean
		if ((iStartChar + iLength) < l_iCurChar) { return TRUE; }

		SCFGraphics::Rect4i rect = ((CWordVisualPart&)m_VisualParts[i]).BoundingBox();
	
		for (int iChar = 0; iChar < (iRealizeFrom - l_iCurChar); iChar++) 
		{ 
			rect.iX += m_ipCharWidths[iChar]; 
		}

		rect.iWidth = 0;

		for (int iChar = (iRealizeFrom - l_iCurChar); iChar < (iRealizeTo - l_iCurChar); iChar++)
		{
			rect.iWidth += m_ipCharWidths[iChar]; 
		}

		//Make sure that the measured width does not exceed the visual width - for whitespace breaking
		rect.iWidth = __min(((CWordVisualPart&)m_VisualParts[i]).BoundingBox().iWidth, rect.iWidth);

		CCache::SelectionRectList.LastAdd(*(new CRectInt(rect)));
	
		l_iCurChar += ((CWordVisualPart&)m_VisualParts[i]).Length();
	}

	return TRUE;
}

bool CWord::IsCursorInText(_IN int iX, _IN int iY)
{
	//Go trough all paragraphs
	for (SCF::UINT i = 0; i < m_VisualParts.Size(); i++)
	{
		SCFGraphics::Rect4i l_TempRect = ((CWordVisualPart&)m_VisualParts[i]).BoundingBox();

		if (PointInRect(iX, iY, l_TempRect)) { return TRUE; }
	}

	return FALSE;
}

int CWord::CharOffset(_IN SCF::TCHAR cCharacter)
{
	int fOffset = 0;

	for (SCF::UINT uiChar = 0; uiChar < m_Text.Length(); uiChar++)
	{
		if (cCharacter == m_Text[uiChar]) { return fOffset; }
		fOffset += m_ipCharWidths[uiChar];
	}

	return fOffset;
}