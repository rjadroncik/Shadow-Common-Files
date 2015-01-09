#pragma once

#include <SCFObjectExtensions.h>
#include <SCFScripting.h>

using namespace SCFBase;

#define ClassClassA 0x0FFFFFFF

#include "ClassB.h"

class CClassA : public CObject
{
public:
	SCF::ENUM ClassKey() _GET { return ClassClassA; }
	CString   ToString() _GET { return STRING("ClassA"); }

public:
	CClassA(_IN int iValue);
	virtual ~CClassA();

public:
	void Dispose();

public:
	void     TestString(_IN CString* pValue) _SET { m_TestString = *pValue; }
	CString* TestString()                    _GET { return (CString*)&m_TestString; }

protected:
	CString m_TestString;
	int     m_TestInt;

	CClassA* m_pTestClassA;
	CClassB* m_pTestClassBSub;

private:
	CClassA() {}
};

