#pragma once

#include <SCFObjectExtensions.h>
#include <SCFScripting.h>

using namespace SCFBase;

#define ClassClassB 0x0FFFFFFE

class CClassA;

enum ClassBEnumeration
{
	EnumValue01,
	EnumValue02,
	EnumValue03,
	EnumValue04,
};

class CClassB : public CObject
{
public:
	ENUM ClassKey() _GET { return ClassClassB; }
	CString   ToString() _GET { return STRING("ClassB"); }

public:
	CClassB();
	virtual ~CClassB();

public:
	void Dispose();

public:
//	void     TestStringSet(_IN CObject& rValue) _SET;
//	CObject& TestString()                       _GET;

protected:
	CString   m_TestString;
	int       m_TestInt;
	ENUM m_TestEnum;

	CClassA* m_pTestClassA;
	CClassA* m_pTestClassA2;
};