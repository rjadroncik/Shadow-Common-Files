#include "LineVisualPart.h"

using namespace SCFTextRendering;

CLineVisualPart::CLineVisualPart(_IN CWord& rWord, _IN int iVisualPart)
{
	m_pWord = (CWord*)&rWord;
	m_iVisualPart = iVisualPart;
}

CLineVisualPart::~CLineVisualPart()
{
}
