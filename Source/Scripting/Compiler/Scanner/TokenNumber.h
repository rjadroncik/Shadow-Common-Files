#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenNumber : public CToken
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTokenNumber; }

	public:
		const CString TypeString() _GET;

	public:
		CTokenNumber(_INOUT _REF CStringRange& rText);
		virtual ~CTokenNumber();

	public:
		CValue& Value() _GET { return *m_pValue; }

	public:
		CInt&   ValueAsInt()   _GET { return   *(CInt*)m_pValue; }
		CFloat& ValueAsFloat() _GET { return *(CFloat*)m_pValue; }

	public:
		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const { CToken::DependentsSerialize(rStream); rStream.Next(*m_pValue); }
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream)       { CToken::DependentsDeserialize(rStream); rStream.Next(); m_pValue = (CValue*)rStream.Current(); }

	protected:
		CTokenNumber() {}

	private:
		CValue* m_pValue;
	};
};
