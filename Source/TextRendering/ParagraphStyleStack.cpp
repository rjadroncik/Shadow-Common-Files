#include "ParagraphStyleStack.h"

using namespace SCFTextRendering;

CParagraphStyleStack::CStyleState CParagraphStyleStack::s_Style;
CStack CParagraphStyleStack::s_Styles;
CStack CParagraphStyleStack::s_PopQueue;

void CParagraphStyleStack::Initialize()
{
	ResetStyleState();
}

//Manipulates style stack - [PushStyle(..)] backs-up current style state on the stack & leaves it untouched for further ie
bool CParagraphStyleStack::PushStyleStack(_IN CStyle& rStyle)
{
	s_Styles.Push(*(new CStyleState(s_Style)));
	s_Style.pStyle = &rStyle;

	return TRUE;
}

void CParagraphStyleStack::FlushStyleStack()
{
	while (!s_Styles.IsEmpty()) { s_Styles.PopAndDelete(); }
}

bool CParagraphStyleStack::PopStyleStack(_IN CStyle& rStyle)
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
void CParagraphStyleStack::ResetStyleState()
{
	//Empty style stack to flush open/unsettled styles - that is styles that have an infinite length (specified as -1), these styles have only a PUSH entry & no matched POP entry, this prevents the stack to infinitely increase its size 
	while (!s_Styles.IsEmpty())
	{
		//Pop stack
		s_Styles.PopAndDelete();
	}

	//paragraph style - global, but private state
	s_Style.eAlignHorizontal = ALIGN_LEFT;

	s_Style.Indent.iLineFirst = 0;

	s_Style.Indent.iLeft  = 0;
	s_Style.Indent.iRight = 0;

	s_Style.Indent.iBefore = 0;
	s_Style.Indent.iAfter  = 0;

	s_Style.BackgroundColor = SCF_RGBA(255, 255, 255, 0);	

	s_Style.Tabs.AllDelete();
}

//This function filters the style properties it recognizes & updates state accordingly
bool CParagraphStyleStack::ExecuteStyle(_IN CStyle& rStyle)
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
		case PP_FIRSTLINE_INDENT:	{ s_Style.Indent.iLineFirst = ((CInt&)pProperty->Value()).Value(); break; }
		case PP_INDENT_LEFT:		{ s_Style.Indent.iLeft      = ((CInt&)pProperty->Value()).Value(); break; }
		case PP_INDENT_RIGHT:		{ s_Style.Indent.iRight		= ((CInt&)pProperty->Value()).Value(); break; }
		//case PP_BACKGROUND_COLOR:	{ s_Style.BackgroundColor	= ((CInt&)pProperty->Value()).Value(); break; }
		case PP_SPACE_BEFORE:		{ s_Style.Indent.iBefore	= ((CInt&)pProperty->Value()).Value(); break; }
		case PP_SPACE_AFTER:		{ s_Style.Indent.iAfter		= ((CInt&)pProperty->Value()).Value(); break; }

		case PP_ALIGN_HORIZONTAL:	{ s_Style.eAlignHorizontal = ((CEnum&)pProperty->Value()).Value(); break; }

/*			case PP_TAB_LIST:
			{
				int iDataSize = 0;

				if (SUCCEEDED(pProperty->Complex()->DataSize(&iDataSize)))
				{
					//Calculate tab count, quit if null
					int iTabCount = iDataSize / sizeof(CTabEntry);
					if (!iTabCount) { break; }

					//Retrieve data
					CTabEntry* pTab = (CTabEntry*)CMemory::Allocate(iDataSize);
					pProperty->Complex()->Data(pTab, iDataSize);

					//Sort & store all entries
					CTabEntry*	pCurTab = pTab;
					bool 	bTabAdded = FALSE;

					for (int iIndex = 0; iIndex < iTabCount; iIndex++)
					{
						//Try finding the appropriate location for the new entry
						for (list <CTabEntry>::iterator tab_iterator = s_Style.Tabs.begin(); tab_iterator !=  s_Style.Tabs.end(); tab_iterator++)
						{
							//If an entry with the given position exists, update it
							if (((CTabEntry)(*tab_iterator)).iPosition == pCurTab->iPosition) { (CTabEntry)(*tab_iterator) = *pCurTab; bTabAdded = TRUE; break; }

							//If we found an entry that should lay beyond the new entry, insert the new one before it
							if (((CTabEntry)(*tab_iterator)).iPosition > pCurTab->iPosition) { s_Style.Tabs.insert(tab_iterator, *pCurTab); bTabAdded = TRUE; break; }
						}

						if (!bTabAdded)
						{
							//If we got here, the new entry belongs at the end of the list
							s_Style.Tabs.push_back(*pCurTab);
						}

						//Select next tab entry
						pCurTab++;
						bTabAdded = FALSE;
					}

					CMemory::Free(pTab);
				}

				break;
			}*/
		}
	}

	return TRUE;
}

CTabEntry* CParagraphStyleStack::QueryTabEntry(_IN int iTabIndex)
{
	//Check arguments
	if (iTabIndex >= (int)s_Style.Tabs.Size()) { SCFError(ErrorAccessDenied); return FALSE; }

	//Move iterator to desired location
	//list <CTabEntry>::iterator	tab_I = s_Style.Tabs.begin();
	//for (int iIndex = 0; iIndex < iTabIndex; tab_I++, iIndex++) {}

	////Copy structure
	//*pOutTabEntry = *tab_I;

	return &(CTabEntry&)s_Style.Tabs[iTabIndex];
}

bool CParagraphStyleStack::QueryIntegerValue(_IN SCF::ENUM eProperty, _OUT int* fpOutInteger)
{
	//Check arguments
	if (!fpOutInteger) { SCFError(ErrorInvalidArgument); return FALSE; }

	switch (eProperty)
	{
		//	case PP_INDENT_LEFT:				{ *fpOutInteger = s_Style.Indent.iLeft;			return TRUE; }
		//	case PP_INDENT_RIGHT:				{ *fpOutInteger = s_Style.Indent.iRight;			return TRUE; }
	case PP_FIRSTLINE_INDENT:			{ *fpOutInteger = s_Style.Indent.iLineFirst;		return TRUE; }
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

