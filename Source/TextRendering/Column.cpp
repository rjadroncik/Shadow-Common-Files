#include "Column.h"

using namespace SCFTextRendering;

CColumn::CColumn()
{
	m_BoundingBox.iX = 0;
	m_BoundingBox.iY = 0;
	m_BoundingBox.iWidth  = 0;
	m_BoundingBox.iHeight = 0;
}

CColumn::~CColumn()
{
	m_VisualParts.AllDelete();
}
