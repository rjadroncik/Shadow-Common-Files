#include "Declaration.h"

using namespace SCFXML;

CXMLDeclaration::CXMLDeclaration()
{
	m_Version = STRING("1.0");
	m_Encoding = STRING("utf-8");
	m_Standalone = STRING("yes");
}

CXMLDeclaration::~CXMLDeclaration()
{

}
