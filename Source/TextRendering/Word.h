#pragma once

#include "WordVisualPart.h"
#include "CharacterStyleStack.h"

using namespace Gdiplus;
using namespace SCFBase;

namespace SCFTextRendering
{
	enum WordTypes
	{
	//Standard word
		WORD_NORMAL					= 0x00000001, //Done

	//Standard white-spaces
		WORD_SPACE					= 0x00000010, //Done
		WORD_TAB					= 0x00000040, //Done

	//Special formatting white-spaces
		WORD_LINE_BREAK				= 0x00010000,
	//	WORD_PARAGRAPH_BREAK		= 0x00020000,
		WORD_COLUMN_BREAK			= 0x00040000,
	};

	class CWord : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return SCFTextRendering::ClassWord; }
		CString   ToString() _GET { return STRING("{Word}"); }

	public:
		CWord(_IN SCF::ENUM eWordType, _IN bool bExplicit);
		~CWord();

	public:
		//Functions are used to make sure, that the layout is not done twice
		void ResetLayout(); //Done
		bool LayoutDone() { return m_bLayoutDone; } //Done

		//Functions are used to make sure, that the layout is not done twice
		inline void RenderDone(_IN bool bRenderDone) { m_bRenderDone = bRenderDone; } //Done
		inline bool RenderDone()                     { return m_bRenderDone; } //Done
		
		//Returns the type of the object [WORD_NORMAL, WORD_WHITESPACE, .. ]
		inline SCF::ENUM Type() { return m_eType; } //Done

		//States, whether this word was created to represent a sub-string of the string sent to [TR_SetText(..)]
		inline bool IsExplicit() { return m_bExplicit; } //Done

		//Tells you, whether the word is a whitespace
		inline bool IsWhiteSpace() { if (m_eType == WORD_NORMAL) { return FALSE; } return TRUE; } //Done

		//Tells you, whether the word can be split
		inline bool CanBeSplit() { if ((m_eType == WORD_NORMAL) || (m_eType == WORD_TAB)) { return FALSE; } return TRUE; } //Done

	public:
		//Sets the text, set [iLength] to -1 if [szText] specifies a null terminated string
		bool Text(_IN CString& rText); //Done

		//Retrieves the length of the text contained in this word (in characters/letters, excluding the terminating null character)
		int TextLength() { return m_Text.Length(); } //Done

	public:
		inline CWordVisualPart& VisualPart(int iIndex) _GET { return (CWordVisualPart&)m_VisualParts[iIndex]; }
		inline CWordVisualPart& VisualPartLast()       _GET { return (CWordVisualPart&)m_VisualParts.Last(); }

	public:
		CVector& VisualParts() _GET { return (CVector&)m_VisualParts; }

	public:
		//These functions are called to provide the word with the necessary information about it's partitioning
		bool StyleEntry       (_IN SCF::UINT uiCharPos, _IN CStyle& rStyle, _IN bool bEntryTypePush); //Done
		bool StyleEntryClear  (_IN SCF::UINT uiCharPos, _IN CStyle& rStyle, _IN bool bClearPushEntries, _IN bool bClearPopEntries); //Done
		bool StyleEntriesClear(_IN SCF::UINT uiCharPos,                     _IN bool bClearPushEntries, _IN bool bClearPopEntries); //Done

	public:
		bool ItemBoundary(_IN SCF::UINT uiCharPos, _IN SCRIPT_ANALYSIS& scriptAnalysis); //Done

	public:
		//Lays out the string at the given starting point, computes the bounding rectangle used for further layout adjustments & rendering, this function is dependent on the [ShouldStopLayingOut(..)] function
		bool LayOut(_IN SCFGraphics::Rect4i* pRect, _IN SCF::DWORD dwLayoutOptions, _IN _OPT int iMinWidthForSpacesToWrap, _OUT SCF::DWORD* dwpOutLayoutResults); //Done

	public:
		//Renders the word into the current layout/bounding rectangle
		bool Render(_IN SCFGraphics::Rect4i* pUpdateRect); //Done
		void RenderWordOutlines(); //Done

	public:
		//Undo last visual part layout, so that it can be redone
		bool RollBackLastLayout(); //Done

	public:
		bool CursorPosToCharacterPos(_IN int iX, _IN int iY, _OUT int* ipOutCharPos); //Done

		//This function adds all rectangles fully enclosing the given selection range to the global cache
		bool RealizeSelectionRange(_IN int iStartChar, _IN int iLength); //Done

		//Query cursor information
		bool IsCursorInText(_IN int iX, _IN int iY); //Done

		//Return the visual offset to a given character in the word - return s full width if character is not found
		int	CharOffset(_IN SCF::TCHAR cCharacter); //Done

	public:
		//Used for gathering statistical information
		static int s_iCharactersLaidOut;
		static int s_iFormattingCharsLaidOut;

		static int s_iWordsLaidOut;
		
	protected:
		//This helper function is used exclusively by the [LayOut(..)] & is bound on member variables, if [LayOut(..)] is modified, will probably cause a crash
		bool ShouldStopLayingOut(_IN SCF::DWORD dwLayoutOptions, _IN int iLayoutWidth, _IN int iMinWidthForSpacesToWrap); //Done

	protected:
		//Each [CPartInfo] structure contains a list of [CPartEntry] structures - each describing a particular change (i.e.: style change, beginning of a new uniscribe item, ..)
		class CBoundaryItem : public CObject
		{
		public:
			CBoundaryItem(_IN SCRIPT_ANALYSIS& rScriptAnalysis) { m_ScriptAnalysis = rScriptAnalysis; }
			virtual ~CBoundaryItem() {}

		public:
			SCF::ENUM ClassKey() _GET { return ClassBoundaryItem; }
			CString   ToString() _GET { return STRING("{BoundaryItem}"); }

		public:
			SCRIPT_ANALYSIS& ScriptAnalysis()                                     _GET { return (SCRIPT_ANALYSIS&)m_ScriptAnalysis; }
			void             ScriptAnalysis(_IN SCRIPT_ANALYSIS& rScriptAnalysis) _SET { m_ScriptAnalysis = rScriptAnalysis; }

		protected:
			//A structure describing various aspects of the item - produced by a call to [ScriptItemize(..)] on a whole paragraph
			SCRIPT_ANALYSIS m_ScriptAnalysis;
		};

		class CBoundaryStyle : public CObject
		{
		public:
			CBoundaryStyle(_IN CStyle& rStyle, _IN bool bPush) { m_pStyle = (CStyle*)&rStyle; m_bPush = bPush; }
			virtual ~CBoundaryStyle() {}

		public:
			SCF::ENUM ClassKey() _GET { return ClassBoundaryStyle; }
			CString   ToString() _GET { return STRING("{BoundaryStyle}"); }

		public:
			CStyle& Style()                   _GET { return *(CStyle*)m_pStyle; }
			void    Style(_IN CStyle& rStyle) _SET { m_pStyle = &(CStyle&)rStyle; }
	
		public:
			bool IsPush()               _GET { return m_bPush; }
			void IsPush(_IN bool bPush) _SET { m_bPush = bPush; }

		protected:
			//Defines a style change, a [CPartInfo] may contain multiple items of this type in it's entry list
			CStyle* m_pStyle;
			bool m_bPush;
		};

	protected:
		//Update style state based on style changes
		static bool PushNPopStyles(CVector& rEntries); //Done

	protected:
		//This is used to create a collection of parts, that this word is divided into because of style changes and uniscribe itemization
		class CPartInfo : public CObject
		{
		public:
			CPartInfo() { m_uiCharPos = 0; m_iLeading = 0; m_iDescent = 0; m_iAscent = 0; }
			virtual ~CPartInfo() { m_Entries.AllDelete(); }

		public:
			SCF::ENUM ClassKey() _GET { return 0xffffffff; }
			CString   ToString() _GET { return STRING("{PartInfo}"); }

		public:
			CVector& Entries() _GET { return (CVector&)m_Entries; }

		public:
			SCF::UINT CharPos()                      _GET { return m_uiCharPos; }
			void      CharPos(_IN SCF::UINT uiValue) _SET { m_uiCharPos = uiValue; }

		public:
			int  Leading()               _GET { return m_iLeading; }
			void Leading(_IN int iValue) _SET { m_iLeading = iValue; }

			int  Descent()               _GET { return m_iDescent; }
			void Descent(_IN int iValue) _SET { m_iDescent = iValue; }

			int  Ascent()               _GET { return m_iAscent; }
			void Ascent(_IN int iValue) _SET { m_iAscent = iValue; }

		public:
			int* PtrLeading() _GET { return (int*)&m_iLeading; }
			int* PtrDescent() _GET { return (int*)&m_iDescent; }
			int* PtrAscent()  _GET { return (int*)&m_iAscent; }

		protected:
			//Position of this entry, in characters/letters
			int	m_uiCharPos;

		protected:
			//The total ascent & descent of the run - used for positioning the word on the baseline
			int	m_iLeading; //Leading is adjusted based on leading
			int	m_iDescent;

			//The actual ascent of the run - excluding space for diacritics
			int	m_iAscent;

		protected:
			//A collection of entries, describing the changes, that occur at the character specified by [usCharPos]
			CVector m_Entries;
		};

	protected:
		//This function is used to determine, whether there is a [tItemBoundary] entry in the entry-list of the supplied [CPartInfo] structure
		static bool IsItemBoundary(CPartInfo& rPartInfo);

	protected:
		//Text & it's length, which is used during a call to [CParagraph::ApplyStyle(..)] to quickly determine, to which word a style entry belongs
		CString	m_Text;

		//Stores information about how the word is divided by style changes & item boundaries
		CVector m_Parts;

		//Type of word - whitespace or normal word, initialized by constructor, should not be changed
		SCF::ENUM m_eType;

	protected:
		//STL linked list of all rectangles that fully enclose this word
		CVector m_VisualParts;

		//Array of character widths
		int* m_ipCharWidths;

	protected:
		//Indicates the number of characters/parts already laid-out, & the current part
		SCF::UINT m_uiCharsLaidOut;
		SCF::UINT m_uiCurPart;

		//These two variables provide a backup, so that a rollback can be performed (when a part of the word needs to wrap)
		SCF::UINT m_uiPrevLayoutCharsLaidOut;
		SCF::UINT m_uiPrevLayoutCurPart;

	protected:
		//Indicates whether a rollback is allowed
		bool m_bCanRollBack;

	protected:
		//States, whether this word was created explicitly by the ier, this is used only for "special" words - i.e. new-line words specifyied using the [</newLine>] tag or by specifying the proper ASCII code in the string sent to [TR_SetText(..)]
		bool m_bExplicit;

		//Makes sure, that layout/rendering is not done twice
		bool m_bMeasurementDone;
		bool m_bLayoutDone;
		bool m_bRenderDone;
	};
};
