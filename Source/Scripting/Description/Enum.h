#pragma once

#include "Descriptor.h"

namespace SCFScripting
{
	template class __declspec(dllexport) SCFBase::CDictionaryString<SCFBase::CEnum>;
	template class __declspec(dllexport) SCFBase::CDictionaryInt64<SCFBase::CString>;

	class SCRIPTING_API CEnum : public CDescriptor
	{
	public:
		CString ToString() _GET { return STRING("{Enum}"); }

	public:
		CEnum();
		virtual ~CEnum();

	public:
		void Literal(_IN CString& rName, _IN ENUM eValue) _SET;

	public:
		//Never delete the return value
		SCFBase::CEnum* LiteralValue(_IN CString& rName) _GET;
		CString*        LiteralName (_IN ENUM eValue)    _GET;

	private:
		CDictionaryString<SCFBase::CEnum> m_Values;
		CDictionaryInt64<CString> m_Names;
	};
};
