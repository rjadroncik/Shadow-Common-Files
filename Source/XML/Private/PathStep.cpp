#include "PathStep.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStep::CXMLPathStep()
{
	m_pNext = nullptr;
}

CXMLPathStep::~CXMLPathStep()
{
	if (m_pNext) { delete m_pNext; }
}
