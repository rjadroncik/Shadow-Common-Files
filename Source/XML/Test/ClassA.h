#pragma once

#include <SCFObjectExtensions.h>
#include <SCFXML.h>

using namespace SCFBase;
using namespace SCFXML;

#define ClassClassA 0x0FFFFFFF

#include "ClassB.h"

class __declspec(dllexport) CClassA : public CXMLObjectSerializable
{
public:
	SCF::ENUM ClassKey() _GET { return ClassClassA; }

public:
	CClassA(_IN int iValue);
	virtual ~CClassA();

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
	CString m_TestString;
	int     m_TestInt;

	CClassA* m_pTestClassA;
	CClassB* m_pTestClassBSub;

private:
	CClassA() {}
};

