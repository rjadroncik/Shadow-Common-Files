#include "ColumnVisualPart.h"

using namespace SCFTextRendering;

CColumnVisualPart::CColumnVisualPart(_IN CParagraph& rParagraph, _IN int iVisualPart)
{
	m_pParagraph = (CParagraph*)&rParagraph;
	m_iVisualPart = iVisualPart;
}

CColumnVisualPart::~CColumnVisualPart()
{

}
