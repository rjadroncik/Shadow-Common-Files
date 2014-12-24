#pragma once

#include "Line.h"

namespace SCFTextRendering
{
	class CParagraph;

	class TEXT_RENDERING_API CParagraphVisualPart : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return 0xffffffff; }
		CString   ToString() _GET { return STRING("{ParagraphVisualPart}"); }

	public:
		CParagraphVisualPart();
		~CParagraphVisualPart();

	public:
		inline void Parent(CParagraph* pParagraph) { m_pParent = pParagraph; }

	public:
		inline int  LineCount()                    _GET { return m_Lines.Size(); }
		inline void AddLine(_IN _REF CLine& rLine) _SET { m_Lines.LastAdd(rLine); }

		inline CLine& Line(int iIndex) _GET { return (CLine&)m_Lines[iIndex]; }
		inline CLine& LineLast()       _GET { return (CLine&)m_Lines.Last(); }
		inline CLine& LineFirst()      _GET { return (CLine&)m_Lines[0]; }

	public:
		CVector& Lines() _GET { return (CVector&)m_Lines; }
//		inline std::list <CLine*>::iterator LinesBegin() { return m_Lines.begin(); }
//		inline std::list <CLine*>::iterator LinesEnd()   { return m_Lines.end(); }
		
	public:
		inline void                 BoundingBox(_IN SCFGraphics::Rect4i& rRect) _SET { m_BoundingBox = rRect; }
		inline SCFGraphics::Rect4i& BoundingBox()                               _GET { return (SCFGraphics::Rect4i&)m_BoundingBox; }

		inline void X(_IN int iX) _SET { m_BoundingBox.iX = iX; }
		inline int  X()           _GET { return m_BoundingBox.iX; }

		inline void Y(_IN int iY) _SET { m_BoundingBox.iY = iY; }
		inline int  Y()           _GET { return m_BoundingBox.iY; }

		inline void Width (_IN int iWidth) _SET { m_BoundingBox.iWidth  = iWidth; }
		inline int  Width()                _GET { return m_BoundingBox.iWidth; }

		inline void Height(_IN int iHeight) _SET { m_BoundingBox.iHeight = iHeight; }
		inline int  Height()                _GET { return m_BoundingBox.iHeight; }

	public:
		void Translate (_IN int iX, _IN int iY);
		void TranslateX(_IN int iX);
		void TranslateY(_IN int iY);

		inline void	AddToWidth (_IN int iWidth)  { m_BoundingBox.iWidth	 += iWidth; }
		inline void	AddToHeight(_IN int iHeight) { m_BoundingBox.iHeight += iHeight; }

	public:
		void DeleteLines() { m_Lines.AllDelete(); }

		//Used to adjust the height of the last paragraph in each column - to provide consistency with InDesign
		int SubtractLineLastDescent(); //Done

		bool Render(_IN SCFGraphics::Rect4i* pUpdateRect); //Done

		void SubtractSpaceAfter() { m_BoundingBox.iHeight -= m_SpaceAfter; }
		void AddSpaceAfter(_IN int iSpaceAfter) { m_SpaceAfter += iSpaceAfter; m_BoundingBox.iHeight += iSpaceAfter; }

	private:
		CParagraph* m_pParent;

		int m_SpaceAfter;
		SCFGraphics::Rect4i m_BoundingBox;
		CVector m_Lines;
	};
};