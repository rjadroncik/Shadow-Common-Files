#include "Compiler.h"

using namespace SCFCompiler;
using namespace SCFScripting;

CCompiler::CCompiler() : m_Parser(*this)
{
	m_CompilationUnits.AllDelete();
}

CCompiler::~CCompiler()
{
}

bool CCompiler::Compile(_IN CString& rText)
{
	if (!m_Scanner.Scan(rText)) { return FALSE; }

	CCompilationUnit* pCompilationUnit = new CCompilationUnit();

	m_CompilationUnits.LastAdd(*pCompilationUnit);

	if (!m_Parser.Parse(m_Scanner.Tokens(), *pCompilationUnit)) { return FALSE; }

	return TRUE;
}
