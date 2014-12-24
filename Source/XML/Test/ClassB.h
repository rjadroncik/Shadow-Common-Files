#pragma once

#include <SCFObjectExtensions.h>
#include <SCFXML.h>

using namespace SCFBase;
using namespace SCFXML;

#define ClassClassB 0x0FFFFFFE

class CClassA;

enum ClassBEnumeration
{
	EnumValue01,
	EnumValue02,
	EnumValue03,
	EnumValue04,
};

class __declspec(dllexport) CClassB : public CXMLObjectSerializable
{
public:
	SCF::ENUM ClassKey() _GET { return ClassClassB; }

public:
	CClassB();
	virtual ~CClassB();

public:
	virtual void XMLSerialize  (_INOUT IXMLStreamWrite& rWriter) const;
	virtual void XMLDeserialize(_INOUT IXMLStreamRead&  rReader);

	virtual void XMLDependentsSerialize  (_INOUT IXMLStreamWriteObject& rWriter) const;
	virtual void XMLDependentsDeserialize(_INOUT IXMLStreamReadObject&  rReader);

public:
	CString ToString() _GET { return STRING("{ClassB}"); }

public:
	void Dispose();

public:
	inline const CString TestString()                         _GET { return STRINGREF(m_TestString); }
	inline void          TestString(_IN CString& rTestString) _SET { m_TestString = rTestString; }

protected:
	CString   m_TestString;
	int       m_TestInt;
	SCF::ENUM m_TestEnum;

	CClassA* m_pTestClassA;
	CClassA* m_pTestClassA2;
};