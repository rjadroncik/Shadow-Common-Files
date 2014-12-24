#include "ParagraphVisualPart.h"
#include "Paragraph.h"

using namespace SCFTextRendering;

CParagraphVisualPart::CParagraphVisualPart()
{
	m_BoundingBox.iX		= 0;
	m_BoundingBox.iY		= 0;
	m_BoundingBox.iWidth	= 0;
	m_BoundingBox.iHeight	= 0;

	m_SpaceAfter = 0;
}

CParagraphVisualPart::~CParagraphVisualPart()
{
}

int CParagraphVisualPart::SubtractLineLastDescent() 
{
	if (LineCount())
	{
		AddToHeight(-LineLast().Descent());

		return LineLast().Descent();
	}

	return 0;
}

bool CParagraphVisualPart::Render(_IN SCFGraphics::Rect4i* pUpdateRect)
{
	m_pParent->Render(pUpdateRect);

	return TRUE;
}

void CParagraphVisualPart::Translate(_IN int iX, _IN int iY)
{
	m_BoundingBox.iX += iX; 
	m_BoundingBox.iY += iY; 

	for (SCF::UINT i = 0; i < m_Lines.Size(); i++)
	{
		((CLine&)m_Lines[i]).Translate(iX, iY);
	}
}
void CParagraphVisualPart::TranslateX(_IN int iX)
{
	m_BoundingBox.iX += iX; 

	for (SCF::UINT i = 0; i < m_Lines.Size(); i++)
	{
		((CLine&)m_Lines[i]).TranslateX(iX);
	}
}

void CParagraphVisualPart::TranslateY(_IN int iY)
{
	m_BoundingBox.iY += iY; 

	for (SCF::UINT i = 0; i < m_Lines.Size(); i++)
	{
		((CLine&)m_Lines[i]).TranslateY(iY);
	}
}
