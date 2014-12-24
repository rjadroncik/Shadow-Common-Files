#pragma once

#include "Headers.h"

namespace SCFTextRendering
{
	//A visual park keeps track of all the measurements needed to properly dot the layout & aligning
	class TEXT_RENDERING_API CWordVisualPart : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return 0xffffffff; }
		CString   ToString() _GET { return STRING("{WordVisualPart}"); }

	public:
		CWordVisualPart(); 
		~CWordVisualPart();

	public:
		inline void                 BoundingBox(_IN SCFGraphics::Rect4i& rRect) _SET { m_BoundingBox = rRect; }
		inline SCFGraphics::Rect4i&	BoundingBox()                               _GET { return (SCFGraphics::Rect4i&)m_BoundingBox; }

		inline void X(_IN int iX) _SET { m_BoundingBox.iX = iX; }
		inline int  X()           _GET { return m_BoundingBox.iX; }

		inline void Y(_IN int iY) _SET { m_BoundingBox.iY = iY; }
		inline int  Y()           _GET { return m_BoundingBox.iY; }

		inline void Width (_IN int iWidth) _SET { m_BoundingBox.iWidth = iWidth; }
		inline int  Width()                _GET { return m_BoundingBox.iWidth; }

		inline void Height(_IN int iHeight) _SET { m_BoundingBox.iHeight = iHeight; }
		inline int  Height()                _GET { return m_BoundingBox.iHeight; }

	public:
		inline void	Translate (_IN int iX, _IN int iY) { m_BoundingBox.iX += iX; m_BoundingBox.iY += iY; }
		inline void	TranslateX(_IN int iX)             { m_BoundingBox.iX += iX; }
		inline void	TranslateY(_IN int iY)             { m_BoundingBox.iY += iY; }

	public:
		inline void Leading(_IN int iLeading) _SET { m_iLeading	= iLeading; }
		inline int  Leading()                 _GET { return m_iLeading; }
		
		inline void Ascent(_IN int iAscent) _SET { m_iAscent = iAscent; }
		inline int  Ascent()                _GET { return m_iAscent; }
	
		inline void Descent(_IN int iDescent) _SET { m_iDescent	= iDescent; }
		inline int  Descent()                 _GET { return m_iDescent; }

		inline void StartingCharacter(_IN int iIndex) _SET { m_iStartChar = iIndex; }
		inline int  StartingCharacter()               _GET { return m_iStartChar; }

		inline void Length(_IN int iLength) _SET { m_iLength = iLength; }
		inline int  Length()                _GET { return m_iLength; }
		
	public:
		inline void AlignToBaseline(_IN int iY) { Y(iY - Leading()); }

	private:
		//Bounding rect
		SCFGraphics::Rect4i	m_BoundingBox;

		//Font measurements
		int m_iLeading;
		int m_iAscent;
		int m_iDescent;

		//Character range
		int m_iStartChar;
		int m_iLength;
	};
};
