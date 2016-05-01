#pragma once

#include "StreamReadObject.h"
#include "StreamWriteObject.h"

namespace SCFXML
{	
	#define GETVALUE if ((pValue = rReader.GetValue()) != NULL)
	#define PUTVALUE(name, value) rWriter.PutValue(STRING(name), (value))
	#define PUTVALUENEW(name, value, class) rWriter.PutValue(STRING(name), *(new class(value)))

	#define CLASS_XMLSERIALIZABLE_REGISTER(xmlName, class, module) SCFXML::CXMLObjectSerializable::ClassRegister(STRING(xmlName), STRING(#class), sizeof(class), module);

	class XML_API CXMLObjectSerializable : public SCFBase::CObject
	{
	public:
		virtual ~CXMLObjectSerializable();

		virtual CString XmlName() _GET = 0;

	public:
		//Retrieves an unique identifier for the object, defaults to it's memory address
		virtual CString ID() _GET;

	public:
		virtual void XMLSerialize  (_INOUT IXMLStreamWrite& rWriter) const;
		virtual void XMLDeserialize(_INOUT IXMLStreamRead&  rReader);

		virtual void XMLDependentsSerialize  (_INOUT IXMLStreamWriteObject& rWriter) const;
		virtual void XMLDependentsDeserialize(_INOUT IXMLStreamReadObject&  rReader);

	protected:
		CXMLObjectSerializable();

	public:
		//Registering a class requires it's constructor to be exported form a DLL,
		//destructors must always be virtual & implicit linking is presumed
		static bool ClassRegister  (_IN CString& rXmlName, _IN CString& rFullClassName, _IN UINT uiObjectSize, _IN void* hModule);
		static bool ClassUnregister(_IN CString& rXmlName);

	public:
		static bool ClassIsRegistered(_IN CString& rXmlName);

	public:
		//Create a new object based on a registered class key
		static CXMLObjectSerializable* New(_IN CString& rXmlName);
	};
};