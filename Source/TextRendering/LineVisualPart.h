#pragma once

#include "Word.h"

namespace SCFTextRendering
{
	class TEXT_RENDERING_API CLineVisualPart : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return 0xffffffff; }
		CString   ToString() _GET { return STRING("{LineVisualPart}"); }

	public:
		CLineVisualPart(_IN CWord& rWord, _IN int iVisualPart);
		~CLineVisualPart();

	public:
		inline CWord&           Word()            { return *m_pWord; }
		inline CWordVisualPart& VisualPart()      { return (CWordVisualPart&)m_pWord->VisualParts()[m_iVisualPart]; }
		inline int              VisualPartIndex() { return m_iVisualPart; }

	private:
		CWord* m_pWord;
		int    m_iVisualPart;
	};
};
