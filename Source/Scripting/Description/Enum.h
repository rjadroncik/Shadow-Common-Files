#pragma once

#include "Descriptor.h"

namespace SCFScripting
{
	class SCRIPTING_API CEnum : public CDescriptor
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassEnum; }
		CString   AsString() _GET { return STRING("{Enum}"); }

	public:
		CEnum();
		virtual ~CEnum();

	public:
		void Literal(_IN CString& rName, _IN SCF::ENUM eValue) _SET;

	public:
		//Never delete the return value
		SCFBase::CEnum* LiteralValue(_IN CString& rName)   _GET;
		CString*        LiteralName (_IN SCF::ENUM eValue) _GET;

	private:
		#pragma warning(disable:4251)
		CDictionaryString<SCFBase::CEnum> m_Values;
		CDictionaryInt64  m_Names;
		#pragma warning(default:4251)
	};
};
