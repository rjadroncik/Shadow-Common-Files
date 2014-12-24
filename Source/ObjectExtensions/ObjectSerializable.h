#pragma once 
#include "Object.h"

namespace SCFBase
{
	#define CLASS_SERIALIZABLE_REGISTER(classKey, class, module) SCFBase::CObjectSerializable::ClassRegister(classKey, STRING(#class), sizeof(class), module);

	class OBJECT_EXTENSIONS_API IStreamWrite;
	class OBJECT_EXTENSIONS_API IStreamWriteObject;
	class OBJECT_EXTENSIONS_API IStreamRead;
	class OBJECT_EXTENSIONS_API IStreamReadObject;

	class OBJECT_EXTENSIONS_API CObjectSerializable : public CObject
	{
		friend class OBJECT_EXTENSIONS_API IStreamReadObject;
		friend class OBJECT_EXTENSIONS_API IStreamWriteObject;
		friend class OBJECT_EXTENSIONS_API CStreamReadObject;
		friend class OBJECT_EXTENSIONS_API CStreamWriteObject;

	public:
		virtual ~CObjectSerializable();

	public:
		virtual bool IsSerializable() _GET { return TRUE; }
	
	public:
		virtual void Serialize  (_INOUT IStreamWrite& rStream) const;
		virtual void Deserialize(_INOUT IStreamRead&  rStream);
		
		virtual void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		virtual void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);

	protected:
		CObjectSerializable();

	public:
		//Registering a class requires it's constructor to be exported form a DLL,
		//destructors must always be virtual & implicit linking is presumed
		static bool ClassRegister  (_IN SCF::ENUM eClassKey, _IN CString& rQualifiedName, _IN SCF::UINT uiObjectSize, _IN void* hModule);
		static bool ClassUnregister(_IN SCF::ENUM eClassKey);

	public:
		static bool ClassIsRegistered(_IN SCF::ENUM eClassKey);

	public:
		//Create a new object based on a registered class key
		static CObjectSerializable* New(_IN SCF::ENUM eClassKey);
	};
};