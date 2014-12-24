#pragma once

#include "Paragraph.h"

namespace SCFTextRendering
{
	class TEXT_RENDERING_API CColumnVisualPart : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return 0xffffffff; }
		CString   ToString() _GET { return STRING("{ParagraphVisualPart}"); }

	public:
		CColumnVisualPart(_IN CParagraph& rParagraph, _IN int iVisualPart);
		~CColumnVisualPart();

	public:
		inline CParagraph&           Paragraph()       { return *m_pParagraph; }
		inline CParagraphVisualPart& VisualPart()      { return m_pParagraph->VisualPart(m_iVisualPart); }
		inline int                   VisualPartIndex() { return m_iVisualPart; }

	private:
		CParagraph* m_pParagraph;
		int         m_iVisualPart;
	};
};
