#pragma once

#include "Class.h"
#include "Enumeration.h"

namespace SCFScripting
{
	class SCRIPTING_API CRegistry
	{
	public:
		//Registers a class & performs name mappings
		static void EnumerationRegister(_IN CString& rName, _IN CEnumeration& rEnumeration);
	
		//Never delete the returned object
		static CEnum*   EnumerationTranslate(_IN CString& rEnumeration, _IN CString& rValue);
		static CString* EnumerationTranslate(_IN CString& rEnumeration, _IN SCF::ENUM eValue);

		static CEnumeration* Enumeration(_IN CString& rName);

	public:
		//Registers a class & performs name mappings
		static void ClassRegister(_IN CString& rName, _IN SCF::ENUM eClassKey, _IN CClass& rClass);

		//You can get a class descriptor based on a name or class key
		static CClass* Class(_IN CString& rName);
		static CClass* Class(_IN SCF::ENUM eClassKey);

	public:
		//These methods query class, method & property IDs
		static SCF::UINT IDOfClass   (_IN CString& rName);
		static SCF::UINT IDOfMethod  (_IN CString& rName);
		static SCF::UINT IDOfProperty(_IN CString& rName);

		//These methods query class, method & property names
		static CString NameOfClass   (_IN SCF::UINT uiID);
		static CString NameOfMethod  (_IN SCF::UINT uiID);
		static CString NameOfProperty(_IN SCF::UINT uiID);

	protected:
		//These methods map class, method & property names for later queries
		static void MapClass   (_IN CString& rName, _IN SCF::ENUM eClassKey);
		static void MapMethod  (_IN CString& rName);
		static void MapProperty(_IN CString& rName);

	protected:
		static CDictionaryString s_Enumerations;

		//Dictionary of all classes
		static CDictionaryInt64 s_Classes;

		//Mappings between the string names & integer IDs
		static CDictionaryString s_IDsClasses;
		static CDictionaryString s_IDsMethods;
		static CDictionaryString s_IDsProperties;

		static CDictionaryInt64 s_NamesClasses;
		static CDictionaryInt64 s_NamesMethods;
		static CDictionaryInt64 s_NamesProperties;

		static SCF::UINT s_uiNextIDMethod;
		static SCF::UINT s_uiNextIDProperty;

	private:
		CRegistry();
		~CRegistry();
	};
};
