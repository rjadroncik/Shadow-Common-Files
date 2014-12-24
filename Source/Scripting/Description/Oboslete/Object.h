#pragma once

#include "Vector.h"

namespace SCFObjectExtensions
{	
	//This interface defines an access point to object, which can be used to dynamically
	//retrieve & set properties & to execute methods
	class OBJECT_EXTENSIONS_API IScriptableObject
	{
	public:
		//Return values CANNOT be deleted if [CProperty::IsReference()] returns true
		virtual void     PropertySet(_IN SCF::ENUM eName, _IN CObject& rValue) _SET = 0;
		virtual CObject* PropertyGet(_IN SCF::ENUM eName)                      _GET = 0;

	public:
		//Return values HAVE TO be deleted!
		virtual CObject* MethodExecute(_IN SCF::ENUM eName)                          = 0;
		virtual CObject* MethodExecute(_IN SCF::ENUM eName, _IN CObject& rArgument)  = 0;
		virtual CObject* MethodExecute(_IN SCF::ENUM eName, _IN CVector& rArguments) = 0;
	};

	class OBJECT_EXTENSIONS_API CScriptableObject : public CObject, public virtual IScriptableObject
	{
	public:
		virtual ~CScriptableObject();

	public:
		//Return values CANNOT be deleted if [CProperty::IsReference()] returns true
		void     PropertySet(_IN SCF::ENUM eName, _IN CObject& rValue) _SET;
		CObject* PropertyGet(_IN SCF::ENUM eName)                      _GET;

	public:
		//Return values HAVE TO be deleted!
		CObject* MethodExecute(_IN SCF::ENUM eName)                         ;
		CObject* MethodExecute(_IN SCF::ENUM eName, _IN CObject& rArgument) ;
		CObject* MethodExecute(_IN SCF::ENUM eName, _IN CVector& rArguments);

	protected:
		CScriptableObject();
	};
};