#pragma once

#include "Style.h"
#include "Column.h"
#include "FrameStyleStack.h"

using namespace Gdiplus;

extern CString g_NewLine;
extern CString g_NewParagraph;
extern CString g_NewColumn;

extern CString g_Tab;
extern CString g_NBSpace;

extern CString g_Space;

namespace SCFTextRendering
{
	class TEXT_RENDERING_API CFrame : public CObject
	{
		friend bool STDAPICALLTYPE TR_TextRenderShutdown();

	public:
		SCF::ENUM ClassKey() _GET { return SCFTextRendering::ClassFrame; }
		CString   ToString() _GET { return STRING("{Frame}"); }

	public:
		CFrame();
		~CFrame();

	public:
		//Sets, retrieves the text-frame bounding rectangle
		void                 Rect(_IN SCFGraphics::Rect4i& rRect) _SET; //Done
		SCFGraphics::Rect4i& Rect()                               _GET; //Done

		//Sets, retrieves the text-frame bounding rectangle
		void                 Insets(_IN SCFGraphics::Rect4i& rInsets) _SET; //Done
		SCFGraphics::Rect4i& Insets()                                 _GET; //Done

		//Sets, retrieves the text-frame position
		bool Position(_IN int iX, _IN int iY)             { m_Rect.iX = iX; m_Rect.iY = iY; return TRUE; }
		bool Position(_OUT int* fpOutX, _OUT int* fpOutY) { if (fpOutX) { *fpOutX = m_Rect.iX; } if (fpOutY) { *fpOutY = m_Rect.iY; } return TRUE; }

		//Sets, retrieves the text-frame dimensions
		bool Dimensions(_IN int iWidth, _IN int fHeight)             { if ((iWidth <= 0) || (fHeight <= 0)) { SCFError(ErrorInvalidArgument); return FALSE; } m_Rect.iWidth = iWidth; m_Rect.iHeight = fHeight; return TRUE; }
		bool Dimensions(_OUT int* fpOutWidth, _OUT int* fpOutHeight) { if (fpOutWidth) { *fpOutWidth = m_Rect.iWidth; } if (fpOutHeight) { *fpOutHeight = m_Rect.iHeight; } return TRUE; }

	public:
		//Sets, retrieves the text-frame output scale
		bool OutputScale(_IN int fScale) { if (!fScale) { SCFError(ErrorInvalidArgument); return FALSE; } CFrameStyleStack::Top().iOutputScale = fScale; return TRUE; }
		int  OutputScale()               { return CFrameStyleStack::Top().iOutputScale; }

	public:
		//These functions store the style change information - if needed, they find the appropriate paragraph & pass the style entry information to it
		bool ApplyStyle(_IN CStyle& rStyle, _IN int iStartPos, _IN int iLength); //Done
		bool ClearStyle(_IN CStyle& rStyle, _IN int iStartPos, _IN int iLength); 
		bool ClearAllStyles                (_IN int iStartPos, _IN int iLength, _IN SCF::ENUM eType); //Done

	public:
		//Sets default styles for the text-frame
		bool    DefaultStyle     (_IN CStyle& rStyle); //Done
		CStyle* DefaultStyle     (_IN SCF::ENUM eType); //Done
		bool    ClearDefaultStyle(_IN SCF::ENUM eType); //Done

	public:
		//Sets, retrieves the text-frame text
		bool Text(_IN CString& rText); //Done

	public:
		//Measures the minimal needed width & height in order to display all text within the frame, given a desired width ([fDesired..Width] can be specified as -1 to indicate that maximum line width should be used)
		bool EmptyFrameRect           (_IN HDC hDC,                                 _OUT SCFGraphics::Rect4i* pOuRect4i); //Done
		bool RectBasedOnFrameHeight   (_IN HDC hDC, _IN int iDesiredFrameHeight,    _OUT SCFGraphics::Rect4i* pOuRect4i); //Done
		bool RectBasedOnFrameWidth    (_IN HDC hDC, _IN int iDesiredFrameWidth,     _OUT SCFGraphics::Rect4i* pOuRect4i); //Done
		bool RectBasedOnParagraphWidth(_IN HDC hDC, _IN int iDesiredParagraphWidth,	_OUT SCFGraphics::Rect4i* pOuRect4i); //Done
		bool RectBasedOnMaxWordWidth  (_IN HDC hDC,                                 _OUT SCFGraphics::Rect4i* pOuRect4i); //Done

	public:
		//Lays-out the text-frame contents given a device context
		bool LayOut(_IN HDC hDC, _IN UpdateOptions dwUpdateOptions, _IN bool bNewObjects, _IN SCF::DWORD dwLayoutOptions); //Done
	//	bool QueryLayoutWithSameLines(_IN int iColumnHeight, _IN SCF::DWORD dwLayoutOptions);

		//Draw the text-frame contents into the given device context
		bool Render(_IN HDC hDC, _IN SCFGraphics::Rect4i* pUpdateRect); //Done

	public:
		//Returns various statistics (some are base on current layout (i.e.: STATS_LINES))
		int ContentStats(_IN SCF::ENUM eStatsType); //Done

	public:
		bool CursorPosToCharacterPos(_IN int iX, _IN int iY, _OUT int* ipOutCharPos); //Done
		bool CarreRect(_IN int iCharPos, _OUT SCFGraphics::Rect4i& rOutCarerect);

	public:
		bool       RenderOptions(_IN SCF::DWORD dwOptions) { m_dwRenderOptions = dwOptions; return TRUE; } //Done
		SCF::DWORD RenderOptions()                         { return m_dwRenderOptions; } //Done

	public:
		bool AddSelectionRange(_IN int iStartChar, _IN int iLength); //Done
		bool ClearSelection(); //Done

		//This function adds all rectangles fully enclosing the given selection range to the global cache
		bool RealizeSelection(_IN int iStartPos, _IN int iLength); //Done

		//Query cursor information
		bool IsCursorInSelection(_IN int iX, _IN int iY); //Done
		bool IsCursorInText     (_IN int iX, _IN int iY); //Done
		
	private:
		int MaxWordWidth(_IN HDC hDC);

		bool ApproximateWidth(_IN HDC hDC, _INOUT SCFGraphics::Rect4i* pInOuRect4i, _IN int iMinWidth, _IN int iMaxWidth); //Done
		
	private:
		bool PrepareForLayout(_IN HDC hDC, _IN bool bNewObjects = TRUE); //Done
		bool CleanAfterLayout(); //Done

		bool AlignVertically();

	private:
		#pragma warning (suppress:4251)
		CVector<CColumn> m_Columns;
		#pragma warning (suppress:4251)
		CVector<CParagraph> m_Paragraphs;

		//Used for rendering only visible paragraphs
		SCF::UINT m_uiParagraphVisibleCount;

	private:
		//Contains the bounding rectangle
		SCFGraphics::Rect4i	m_Rect;
		bool m_bTextChanged;

	private:
		//Default styles for frame contents
		const CStyle* m_pStyleCharacter;
		const CStyle* m_pStyleParagraph;

		//Frame style
		const CStyle* m_pStyle;

	private:
		SCF::DWORD m_dwRenderOptions;
		int m_iColumnCount;

	private:
		int m_iTextLength;

		//Used for statistics
		int m_iCharactersLaidOut;
		int m_iFormattingCharsLaidOut;
		int m_iWordsLaidOut;
		int m_iParagraphsLaidOut;

	private:
		class CSelection : public CObject
		{
		public:
			SCF::ENUM ClassKey() _GET { return 0xffffffff; }
			CString   ToString() _GET { return STRING("{Selection}"); }
	
		public:
			int	iStartChar;
			int	iLength;
		};

		//This list contains character ranges that should be marked as selected after rendering
		#pragma warning (suppress:4251)
		CVector<CSelection> m_Selections;
	};
};
