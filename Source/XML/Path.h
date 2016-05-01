#pragma once

#include "Document.h"
#include "Private/PathStep.h"

namespace SCFXML
{
	//Accepts a simplified XPath expression in the format:
	// {/,//}nodeName{/,//}nodeName

	class XML_API CXMLPath : public SCFBase::CObject
	{
	public:
		CString ToString() _GET { return m_Expression; }

	public:
		CXMLPath();
		CXMLPath(_IN CString& rExpression);

		virtual ~CXMLPath();

	public:
		void          Expression(_IN CString& rExpression) _SET;
		const CString Expression()                         _GET { return m_Expression; }
	
	public:
		//Tries to find the elements(nodes) matching the XMLPath expression
		bool Match(_IN SCFXML::CXMLNode& rRoot, _OUT CList<SCFXML::CXMLNode>& rOutList);

	protected:
		CString m_Expression;

	protected:
		SCFXMLPrivate::CXMLPathStep* m_pPathStepFirst;
	};
};