#include "FrameStyleStack.h"

using namespace SCFTextRendering;

CFrameStyleStack::CStyleState CFrameStyleStack::s_Style;
CStack CFrameStyleStack::s_Styles;

void CFrameStyleStack::Initialize()
{
	ResetStyleState();
}

//Manipulates style stack - [PushStyle(..)] backs-up current style state on the stack & leaves it untouched for further ie
bool CFrameStyleStack::PushStyleStack()
{
	s_Styles.Push(*(new CStyleState(s_Style)));

	return TRUE;
}

bool CFrameStyleStack::PopStyleStack()
{
	if (!s_Styles.Size()) { return FALSE; }

	s_Style = *(CStyleState*)s_Styles.Top();
	s_Styles.PopAndDelete();

	return TRUE;
}

void CFrameStyleStack::FlushStyleStack()
{
	while (!s_Styles.IsEmpty()) { PopStyleStack(); }
}

bool CFrameStyleStack::QueryIntegerValue(_IN SCF::ENUM eProperty, _OUT int* fpOutInteger)
{
	//Check arguments
	if (!fpOutInteger) { SCFError(ErrorInvalidArgument); return FALSE; }

	switch (eProperty)
	{
	case FP_TAB_WIDTH:					{ *fpOutInteger = s_Style.iTabWidth;			return TRUE; }
	case FP_TEXT_SUPERSCRIPT_SIZE:		{ *fpOutInteger = s_Style.iSuperScriptSize;		return TRUE; }
	case FP_TEXT_SUPERSCRIPT_POSITION:	{ *fpOutInteger = s_Style.iSuperScriptPosition;	return TRUE; }
	case FP_TEXT_SUBSCRIPT_SIZE:		{ *fpOutInteger = s_Style.iSubScriptSize;		return TRUE; }
	case FP_TEXT_SUBSCRIPT_POSITION:	{ *fpOutInteger = s_Style.iSubScriptPosition;	return TRUE; }
	case FP_TEXT_SMALLCAPS_SIZE:		{ *fpOutInteger = s_Style.iSmallCapsSize;		return TRUE; }
	}

	SCFError(ErrorInvalidArgument); return FALSE;
}

//This function filters the style properties it recognizes & updates state accordingly
bool CFrameStyleStack::ExecuteStyle(_IN CStyle& rStyle)
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
		case FP_INDENT_LEFT:				{ s_Style.Inset.iX               = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_INDENT_RIGHT:				{ s_Style.Inset.iWidth           = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_INDENT_TOP:					{ s_Style.Inset.iY               = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_INDENT_BOTTOM:				{ s_Style.Inset.iHeight          = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_FIRST_BASELINE:				{ s_Style.eFirstBaseline         = ((CEnum&)pProperty->Value()).Value(); break; }
		case FP_FIRST_BASELINE_OFFSET:		{ s_Style.iFirstBaselineOffset   = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_TAB_WIDTH:					{ s_Style.iTabWidth              = ((CInt&)pProperty->Value()).Value(); if (s_Style.iTabWidth <= 0) { s_Style.iTabWidth = 1;} break; }
		case FP_TEXT_SUPERSCRIPT_SIZE:		{ s_Style.iSuperScriptSize       = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_TEXT_SUPERSCRIPT_POSITION:	{ s_Style.iSuperScriptPosition   = ((CInt&)pProperty->Value()).Value();	break; }
		case FP_TEXT_SUBSCRIPT_SIZE:		{ s_Style.iSubScriptSize         = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_TEXT_SUBSCRIPT_POSITION:	{ s_Style.iSubScriptPosition     = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_TEXT_SMALLCAPS_SIZE:		{ s_Style.iSmallCapsSize         = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_COLUMN_COUNT:				{ s_Style.Column.iCount          = ((CInt&)pProperty->Value()).Value(); if (s_Style.Column.iCount <= 0)	{ s_Style.Column.iCount = 1;} break; }
		case FP_COLUMN_GUTTER:				{ s_Style.Column.iGutter         = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_COLUMN_WIDTH:				{ s_Style.Column.iWidth          = ((CInt&)pProperty->Value()).Value(); break; }
		case FP_COLUMN_WIDTH_FIXED:			{ s_Style.Column.bFixedWidth     = ((CBool&)pProperty->Value()).Value(); break; }
		case FP_ALIGN_VERTICAL:				{ s_Style.eAlignVertical         = ((CEnum&)pProperty->Value()).Value(); break; }
		case FP_CONTINUE_PARAGRAPH:			{ s_Style.bContinueParagraph     = ((CBool&)pProperty->Value()).Value(); break; }
		case FP_IGNORE_LAST_LINE_DESCENT:	{ s_Style.bIgnoreLineLastDescent = ((CBool&)pProperty->Value()).Value(); break; }
		}
	}

	return TRUE;
}

void CFrameStyleStack::ResetStyleState()
{
	//Reset text-frame insets
	s_Style.Inset.iX      = 0;
	s_Style.Inset.iY      = 0;
	s_Style.Inset.iWidth  = 0;
	s_Style.Inset.iHeight = 0;

	s_Style.eFirstBaseline       = BL_ASCENT;
	s_Style.iFirstBaselineOffset = 0;

	s_Style.iTabWidth = 36000;

	//Text position state
	s_Style.iSuperScriptSize     = 583;
	s_Style.iSuperScriptPosition = 333;

	s_Style.iSubScriptSize     = 583;
	s_Style.iSubScriptPosition = 333;

	s_Style.iSmallCapsSize = 700;

	//Column state 
	s_Style.Column.bFixedWidth = FALSE;
	s_Style.Column.iGutter     = 12 * PT_2_INTERNAL; //4.233mm
	s_Style.Column.iWidth      = (int)(354.331 * PT_2_INTERNAL); //125mm
	s_Style.Column.iCount      = 1;

	s_Style.eAlignVertical = ALIGN_TOP;

	s_Style.iOutputScale           = 1000; 
	s_Style.bContinueParagraph     = FALSE;
	s_Style.bIgnoreLineLastDescent = TRUE;
}

