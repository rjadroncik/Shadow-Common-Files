#include "CompilationUnit.h"

using namespace SCFCompiler;

CCompilationUnit::CCompilationUnit()
{
}

CCompilationUnit::~CCompilationUnit()
{
	m_Enums.AllDelete();
	m_Interfaces.AllDelete();
	m_Classes.AllDelete();
}