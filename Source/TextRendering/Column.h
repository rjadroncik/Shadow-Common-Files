#pragma once

#include "ColumnVisualPart.h"

namespace SCFTextRendering
{
	class TEXT_RENDERING_API CColumn : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return SCFTextRendering::ClassColumn; }
		CString   ToString() _GET { return STRING("{Column}"); }

	public:
		CColumn();
		~CColumn();

	public:
		inline CColumnVisualPart& VisualPart(int iIndex) _GET { return m_VisualParts[iIndex]; }
		inline CColumnVisualPart& VisualPartLast()       _GET { return m_VisualParts.Last(); }

	public:
		CVector<CColumnVisualPart>& VisualParts() _GET { return (CVector<CColumnVisualPart>&)m_VisualParts; }

	public:
		inline void                 BoundingBox(_IN SCFGraphics::Rect4i& rRect) _SET { m_BoundingBox = rRect; }
		inline SCFGraphics::Rect4i& BoundingBox()                               _GET { return (SCFGraphics::Rect4i&)m_BoundingBox; }

		inline void X(_IN int iX) _SET { m_BoundingBox.iX = iX; }
		inline int  X()           _GET { return m_BoundingBox.iX; }

		inline void Y(_IN int iY) _SET { m_BoundingBox.iY = iY; }
		inline int  Y()           _GET { return m_BoundingBox.iY; }

		inline void Width (_IN int iWidth) _SET { m_BoundingBox.iWidth = iWidth; }
		inline int  Width()                _GET { return m_BoundingBox.iWidth; }

		inline void Height(_IN int iHeight) _SET { m_BoundingBox.iHeight = iHeight; }
		inline int  Height()                _GET { return m_BoundingBox.iHeight; }

	public:
		inline void	TranslateX(_IN int iX) { m_BoundingBox.iX += iX; }
		inline void	TranslateY(_IN int iY) { m_BoundingBox.iY += iY; }

		inline void	AddToWidth (_IN int iWidth)  { m_BoundingBox.iWidth  += iWidth; }
		inline void	AddToHeight(_IN int iHeight) { m_BoundingBox.iHeight += iHeight; }

	private:
		SCFGraphics::Rect4i m_BoundingBox;
		#pragma warning (suppress:4251)
		CVector<CColumnVisualPart> m_VisualParts;
	};
};
