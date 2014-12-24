#include "WordVisualPart.h"

using namespace SCFTextRendering;

CWordVisualPart::CWordVisualPart()
{
	m_BoundingBox.iX		= 0;
	m_BoundingBox.iY		= 0;
	m_BoundingBox.iWidth	= 0;
	m_BoundingBox.iHeight	= 0;

	//Font measurements
	m_iLeading		= 0;
	m_iAscent		= 0;
	m_iDescent		= 0;

	//Character range
	m_iStartChar	= 0;
	m_iLength		= 0;
}

CWordVisualPart::~CWordVisualPart()
{
}