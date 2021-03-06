#pragma once

#include "NodeType.h"
#include "Errors.h"
#include <SCFObjectExtensions.h>

#define WHITESPACE_CHARS " \t\x0d\x0a"
#define WHITESPACE_CHARS_COUNT 4

namespace SCFXML
{
	class XML_API CXMLStreamRead;

	class XML_API CXMLNode : public SCFBase::CTreeSimpleBase<CString, CXMLNode>
	{
		friend class XML_API CXMLStreamRead;

	public:
		CString ToString() _GET;

	public:
		virtual ~CXMLNode();

	public:
		virtual NodeType Type() _GET = 0;

	public:
		///////////////////////// Basic properties ///////////////////////////

		inline void          QName(_IN CString rQName) _SET { m_QName = rQName; }
		inline const CString QName()                   _GET { return STRINGREF(m_QName); }
 
	public:
		///////////////////////// Qualified names ///////////////////////////

		void Name  (_IN CString& rName)   _SET;
		void Prefix(_IN CString& rPrefix) _SET;

		inline const CString Name()   _GET { return m_uiPrefix ? CStringRange(m_QName, m_uiPrefix + 1) : STRINGREF(m_QName); }
		inline const CString Prefix() _GET { return CStringRange(m_QName, 0, m_uiPrefix); }

	public:
		//Sets the qualified name (together with the namespace prefix part)
		void QName(_IN CString& rPrefix, _IN CString& rName) _SET;
		void QName(_IN CString& rName, _IN UINT uiPrefixLength) _SET;

	public:
		//Access the namespace length control variable for special purposes
		inline void PrefixLength(_IN UINT uiLength) _SET { m_uiPrefix = uiLength; }
		inline UINT PrefixLength()                  _GET { return m_uiPrefix; }	

	public:
		///////////////////////// Children ///////////////////////////

		CXMLNode* ChildNamed      (_IN CString rName) _GET;
		CXMLNode* ChildOrSelfNamed(_IN CString rName) _GET;

	public:
		///////////////////////// Siblings ///////////////////////////

		CXMLNode* NextNamed(_IN CString rName) _GET;

	public:
		///////////////////////// Descendants ///////////////////////////

		CXMLNode* DescendantsNamed      (_IN CString rName) _GET;
		CXMLNode* DescendantsOrSelfNamed(_IN CString rName) _GET;
	
	protected:
		CString m_QName;
		//Defines the length of the namespace prefix part of the qualified name, defaults to 0, meaning there is no prefix
		UINT m_uiPrefix;

	protected:
		CXMLNode();
	};

	template class __declspec(dllexport) SCFBase::CStack<CXMLNode>;
};