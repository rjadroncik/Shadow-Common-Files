#pragma once

#include <SCFObjectExtensions.h>

#include "../Classes.h"

namespace SCFScripting
{
	class SCRIPTING_API CEnumeration : public SCFBase::CDescriptor
	{
	//////////////////////////// CEnumeration - static part ////////////////////////////////
	public:
		//Registers a class & performs name mappings
		static void Register(_IN CString& rName, _IN CEnumeration& rEnumeration);

		//You can get an enumeration descriptor based on its name
		static CEnumeration* ByName(_IN CString& rName);

		//Never delete the returned object
		static CEnum*   Translate(_IN CString& rEnumeration, _IN CString& rValue);
		static CString* Translate(_IN CString& rEnumeration, _IN SCF::ENUM eValue);
	
	//////////////////////////// CEnumeration - definition /////////////////////////////////
	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeration; }
		CString   AsString() _GET { return STRING("{Enumeration}"); }

	public:
		CEnumeration();
		virtual ~CEnumeration();

	public:
		void ValueDefine(_IN CString& rName, _IN SCF::ENUM eValue) _SET;

	public:
		//Never delete the return value
		CEnum*   Value    (_IN CString& rName)   _GET;
		CString* ValueName(_IN SCF::ENUM eValue) _GET;

	private:
		CDictionaryString m_Values;
		CDictionaryInt64  m_Names;
	};
};
