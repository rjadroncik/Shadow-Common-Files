#pragma once

#include "LineVisualPart.h"

namespace SCFTextRendering
{
	enum LayoutOptions
	{
		//Set, to indicate requested fitting
		LO_FIT_VERTICALLY				= 0x00000001,
		LO_FIT_HORIZONTALLY				= 0x00000002,
		LO_FIT_USING_DESCENT			= 0x00000004,

		//When this is set the paragraph width is shrank to the smallest possible size, this is used with [LO_FIT_HORIZONTALLY]
		LO_SHRINK_HORIZONTALLY			= 0x00010000,

		//Set, to indicate that layout is continuing & variables SHOULD NOT be reset
		LO_CONTINUE						= 0x00000010,

		//Set, to indicate that layout is to be done as if the paragraph was being continued, but variables SHOULD be reset
		LO_AS_CONTINUED					= 0x00000020,

		//Set, when laying out the first line of a paragraph, to avoid tab shifting
		LO_FIRSTLINE					= 0x00000200,

		//Set, to notify paragraph to apply the [FP_FIRST_BASELINE] frame property
		LO_FIRST_BASELINE				= 0x00000400,

		LO_IGNORE_SPACE_BEFORE			= 0x01000000,
		LO_IGNORE_SPACE_AFTER			= 0x02000000,

		//Set to allow a word to be split into parts
		LO_SPLIT_WORDS					= 0x00001000,

		LO_EXPAND_EMPTY_LINES			= 0x00100000,

		LO_SKIP_CHARACTER_MEASUREMENT	= 0x10000000,
	};

	enum LayoutResults
	{
		//States the status of the layout process
		LR_NOTHING_FINISHED			= 0x00000001,
		LR_PARTIALY_FINISHED		= 0x00000002,
		LR_FULLY_FINISHED			= 0x00000004,

		//Set, when a word wider than the supplied layout rectangle was found, during a line layout
		LR_LAYOUT_ABORTED			= 0x00000100,

		//Indicates, what type of fitting was unsuccessful
		LR_FIT_FAILED_VERTICAL		= 0x00001000,
		LR_FIT_FAILED_HORIZONTAL	= 0x00002000,

		//Set when special formatting words are found during layout
		LR_LINE_BREAK				= 0x00010000,
		LR_COLUMN_BREAK				= 0x00020000,
	};

	class TEXT_RENDERING_API CLine : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return SCFTextRendering::ClassLine; }
		CString   ToString() _GET { return STRING("{Line}"); }

	public:
		CLine();
		~CLine();

	public:
		inline CLineVisualPart& VisualPart(int iIndex) _GET { return (CLineVisualPart&)m_VisualParts[iIndex]; }
		inline CLineVisualPart& VisualPartLast()       _GET { return (CLineVisualPart&)m_VisualParts.Last(); }

	public:
		CVector& VisualParts() _GET { return (CVector&)m_VisualParts; }

	public:
		bool Rect(_OUT SCFGraphics::Rect4i* pOuRect4i);

		inline bool BBoxPosition(_IN int iX, _IN int iY) { m_BoundingBox.iX = iX, m_BoundingBox.iY = iY; return TRUE; }

	public:
		//The [pOutContinue_I] is filled with the iterator of the word, that should be passed as the [start_I] parameter of this function for the next line
		bool LayOut(_IN SCFGraphics::Rect4i* pLayoutRect, _IN SCF::DWORD dwLayoutOptions, _IN CVector& rWords, _IN SCF::ENUM eAlignType, _IN SCF::ENUM eBaselineType, _IN _OPT int iBaselineOffset, _OUT SCF::DWORD* dwpOutLayoutResults, _OUT SCF::UINT* puiOutWordContinue);

		bool Render(_IN SCFGraphics::Rect4i* pUpdateRect);
		bool RenderBackground(_IN SCFGraphics::ColorRGBA backgroundColor);

	public:
		void Reposition(_IN int iX, _IN int iY);	
		void Translate (_IN int iX, _IN int iY);
		void TranslateX(_IN int iX);
		void TranslateY(_IN int iY);

	public:
		inline int Leading() { return m_iLeading; }
		inline int Ascent()  { return m_iAscent;  }
		inline int Descent() { return m_iDescent; }

	//	inline void SubtractDescent() { m_BoundingBox.iHeight -= m_iDescent; }

	private:
		bool AlignToBaseLine(_IN int iY, _IN SCF::ENUM eBaselineType, _IN int fLeading, _IN int fAscent, _IN int fDescent, _IN int iBaselineOffset);
		bool AlignHorizontally(_IN SCFGraphics::Rect4i* pRect, _IN SCF::ENUM eAlignType);

		DWORD FitVertically(_INOUT CWord& rWord, _IN SCF::DWORD dwLayoutOptions, _IN SCF::ENUM eBaselineType, _IN int iBaselineOffset, _IN int iHeight);
		
		//The vector contains the remaining words after the tab
		bool LayOutTab(_IN SCFGraphics::Rect4i* pLineLayoutRect, _IN SCFGraphics::Rect4i* pTabLayoutRect, _IN CWord& rWordTab, _IN CVector& rWordsRemanining, _IN SCF::DWORD dwLayoutOptions, _INOUT int* ipCurrentTab);

	private:
		CVector m_VisualParts;
		SCFGraphics::Rect4i m_BoundingBox;

		int	m_iLeading;
		int	m_iAscent;
		int	m_iDescent;
	};
};
