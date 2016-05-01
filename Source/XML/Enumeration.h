#pragma once

#include "Errors.h"
#include <SCFObjectExtensions.h>

namespace SCFXML
{
	class XML_API CXMLEnumeration : public SCFBase::CObject
	{
	//////////////////////////// CXMLEnumeration - static part ////////////////////////////////
	public:
		//Registers a class & performs name mappings
		static void Register(_IN CString& rName, _IN CXMLEnumeration& rEnumeration);

		//You can get an enumeration descriptor based on its name
		static CXMLEnumeration* ByName(_IN CString& rName);

		//Never delete the returned object
		static CEnum*   Translate(_IN CString& rEnumeration, _IN CString& rValue);
		static CString* Translate(_IN CString& rEnumeration, _IN ENUM eValue);
	
	//////////////////////////// CXMLEnumeration - definition /////////////////////////////////
	public:
		CString ToString() _GET { return STRING("{XMLEnumeration}"); }

	public:
		CXMLEnumeration();
		virtual ~CXMLEnumeration();

	public:
		void ValueDefine(_IN CString& rName, _IN ENUM eValue) _SET;

	public:
		//Never delete the return value
		CEnum*   ValueOf  (_IN CString& rName) _GET;
		CString* ValueName(_IN ENUM eValue) _GET;

	private:
		#pragma warning(disable:4251)
		CDictionaryString<CEnum> m_Values;
		CDictionaryInt64		 m_Names;
		#pragma warning(default:4251)
	};
};
