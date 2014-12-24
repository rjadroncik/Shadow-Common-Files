#pragma once

#include "ParagraphVisualPart.h"

namespace SCFTextRendering
{
	class TEXT_RENDERING_API CParagraph : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return SCFTextRendering::ClassParagraph; }
		CString   ToString() _GET { return STRING("{Paragraph}"); }

	public:
		CParagraph(_IN bool bExplicit);
		~CParagraph();

	public:
		//Sets the text - invokes word list rebuild, set [iLength] to -1 if [szText] specifies a null terminated string
		bool Text(_IN CString& rText); //Done

	public:
		//Retrieves the total length of the text contained in this paragraph (in characters/letters, excluding the terminating null character)
		inline int TextLength()	{ return m_iTextLength; } //Done

	public:
		//States, whether this paragraph was created implicitly or based on ier input (tag or ASCII code in [TR_SetText(..)])
		inline bool IsExplicit() { return m_bExplicit; } //Done

	public:
		inline int FormattingCharCount() _GET { return m_iFormattingCharCount; } //Done

		inline void FormattingOverhead(_IN int iFormattingOverhead)	_SET { m_iFormattingOverhead = iFormattingOverhead; } //Done
		inline int  FormattingOverhead()                            _GET { return m_iFormattingOverhead; } //Done

	public:
		//Used mostly for statistics
		inline int WordCount() _GET { return m_Words.Size(); } //Done

	public:
		inline CParagraphVisualPart& VisualPart(int iIndex) _GET { return (CParagraphVisualPart&)m_VisualParts[iIndex]; }
		inline CParagraphVisualPart& VisualPartLast()       _GET { return (CParagraphVisualPart&)m_VisualParts.Last(); }

	public:
		CVector& VisualParts() _GET { return (CVector&)m_VisualParts; }

	public:
		int WordCount(SCF::DWORD dwWordTypes); //Done

	public:
		//These functions store the style change information - if a character style is being applied they find the appropriate word & pass the style entry information to it
		bool StyleEntry     (_IN CStyle& rStyle, _IN int iCharPos, _IN bool bEntryTypePush); //Done
		bool StyleEntryClear(_IN CStyle& rStyle, _IN int iCharPos, _IN bool bClearPushEntries, _IN bool bClearPopEntries); //Done
		bool StyleEntriesClear                  (_IN int iCharPos, _IN bool bClearPushEntries, _IN bool bClearPopEntries, _IN SCF::ENUM eType); //Done

		//Called before & after layout is done to apply styles
		bool PushStyle(); //Done
		bool PopStyle(); //Done

	public:
		//Resets the state indicating, whether the words will perform layout & rendering
		bool ResetWordLayout(); //Done
		bool ResetWordRenderStatus(); //Done

		//Deletes all visual parts & lines
		bool ResetLayout(); //Done

		//Lays out the entire paragraph or a remaining portion of it into a given rectangle, returns the actual height of the paragraph
		bool LayOut(_IN SCFGraphics::Rect4i* pRect, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eFirstBaseline, _IN _OPT int iFirstBaselineOffset, _OUT SCF::DWORD* dwpOutLayoutResults, _OUT int* fpOutWidestWordWidth = NULL); //Done

	//	bool QueryLayoutWithSameLines(_IN int iFirstColumnOccupiedHeight, _IN int iColumnHeight, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eFirstBaseline, _IN _OPT int iFirstBaselineOffset, _OUT int* ipOutLinesInLastVisPart, _OUT int* ipOutHeightInLastVisPart, _OUT int* ipOutColumnsUsed);

	public:
		//Renders the whole paragraph based on the current layout created using [LayOutLine()]
		bool Render(_IN SCFGraphics::Rect4i* pUpdateRect); //Done

	public:
		bool CursorPosToCharacterPos(_IN int iX, _IN int iY, _OUT int* ipOutCharPos); //Done

		//This function adds all rectangles fully enclosing the given selection range to the global cache
		bool RealizeSelectionRange(_IN int iStartChar, _IN int iLength); //Done

		//Query cursor information
		bool IsCursorInText(_IN int iX, _IN int iY); //Done

	public:
		//Used for statistics
		static int s_iParagraphsLaidOut;

	public:
		void ResetRenderStatus() { m_bRenderDone = FALSE; }

		void DeleteLastVisualPart() { m_VisualParts.Delete(m_VisualParts.Size() - 1); }

	private:

		//Resizes buffers so that uniscribe cam process a paragraph of a given length (in rare cases - buffers may have to be resized multiple times before the paragraph can be processed)
		static bool PrepareUniscribeBuffers(_IN int iParagraphLength); //Done

		bool ProcessEmptyParagraph(_IN SCFGraphics::Rect4i* pRect, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eFirstBaseline, _IN _OPT int iFirstBaselineOffset, _OUT SCF::DWORD* dwpOutLayoutResults); //Done

		//Structure for keeping uniscribe related state
		struct tUniscribeState
		{
			//ScriptItemize() stores the number of generated items - parts of text which ie only one type of script - in this variable
			int	iGeneratedItems;

			//The length of the script item buffer (pScriptItem) provided to ScriptShape()
			int	iMaxItems;

			//ScriptItemize() writes item information into this buffer
			SCRIPT_ITEM* pScriptItem;

		} static s_Uniscribe;

		static bool s_bClassInitDone;

		//STL linked list of words 
		CVector m_Words;

		//STL linked list of visual parts
		CVector m_VisualParts;

		//This variable is used during a call to [CFrame::ApplyStyle(..)] to quickly determine, to which paragraph a style entry belongs
		int m_iTextLength;

		//States, whether this paragraph was created explicitly by the ier - i.e. using the [</newParagraph>] tag or by specifying the proper ASCII code in the string sent to [TR_SetText(..)]
		bool m_bExplicit;

		//Stores the number of characters used for formatting within this paragraph
		int m_iFormattingCharCount;

		//This variable is set by [LayOut(..)] & used by [Render(..)] to render only visible words
		//std::list <CWord*>::iterator	m_LayoutEnd_I;
		int m_iWordsVisible;

		//Variables describing style entries
		const CStyle* m_pPushStyle;
		const CStyle* m_pPopStyle;
		
		//Stores the number of characters used for specifying this paragraph - that is the length of the string that a [<newParagraph/>] tag is translated - can vary
		int m_iFormattingOverhead;

		bool m_bRenderDone;
	};
};
