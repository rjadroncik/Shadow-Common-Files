#pragma once
#include "ScriptableObject.h"
#include "ScriptableMethod.h"
#include "ScriptableProperty.h"

#include "DictionaryInt64.h"
#include "DictionaryString.h"

namespace SCFObjectExtensions
{
	typedef CScriptableObject* (__cdecl *ACCESSIBLE_OBJECT_CONSTRUCTOR) (_IN CVector& rArguments);

	class OBJECT_EXTENSIONS_API CScriptableClass : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassScriptableClass; }

	public:
		CScriptableClass(_IN SCF::ENUM eClassKey);
		virtual ~CScriptableClass();

	public:
		//These return dictionaries of all supported/exposed properties & methods.
		//This should be implemented to properly handle inherited properties & methods.
		//Return values MUST NOT be deleted!
		const CDictionaryString& Methods()    _GET { return m_Methods; }
		const CDictionaryString& Properties() _GET { return m_Properties; }

	public:
		void AddMethod  (_IN CString& rName, _IN CScriptableMethod&   rMethod);
		void AddProperty(_IN CString& rName, _IN CScriptableProperty& rProperty);

	public:
		inline SCF::ENUM ClassClassKey() { return m_eClassKey; }

	public:
		void ConstructorAdd(_IN CScriptableMethod& rConstructor, _IN ACCESSIBLE_OBJECT_CONSTRUCTOR constructor) _SET;
		SCF::UINT ConstructorCount() _GET { return m_uiConstructorCount; }

		const CScriptableMethod* Constructor(_IN SCF::UINT uiIndex) _GET;

		CScriptableObject* Construct(_IN SCF::UINT uiIndex, _IN CVector& rArguments);

	public:
		CString AsString() _GET { return STRING("{ScriptableClass}"); }

	private:
		SCF::ENUM m_eClassKey;

	private:
		//Dictionaries containing all the properties & methods in the accessible object
		CDictionaryString m_Methods;
		CDictionaryString m_Properties;

	private:
		struct SConstructor
		{
			const CScriptableMethod*      pDescriptor;
			ACCESSIBLE_OBJECT_CONSTRUCTOR Function;

		}* m_pConstructors; 

		SCF::UINT m_uiConstructorCount;
	};
};
