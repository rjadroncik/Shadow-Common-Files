#pragma once

#include <SCFDebugging.h>

using namespace SCFDebugging;
using namespace SCFBase;

class CTestValues : public CTestCase
{
public:
	CTestValues(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestValues();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CChar*        m_pChar;
	CInt*         m_pInt;
	CInt64*       m_pInt64;
	CBool*        m_pBool;
	CFloat*       m_pFloat;
	CFloat2*      m_pFloat2;
	CFloat3*      m_pFloat3;
	CFloat4*      m_pFloat4;
	CString*      m_pString;
	CStringRange* m_pStringRange;
	CEnum*        m_pEnum;
	CPointer*     m_pPointer;
	CDateTime*    m_pDateTime;

	CValue*    m_pValue01;
	CValue*    m_pValue02;
	CValue*    m_pValue03;
};
