#pragma once

#include "Node.h"

namespace SCFXML
{
	class XML_API CXMLDocument;

	//This is a PREFIX tree enumerator, postfix not yet implemented (didn't need it yet)
	class XML_API CXMLEnumerator : public SCFBase::CEnumeratorRaw, public SCFBase::IEnumerator<CXMLNode>
	{
	public:
		CXMLEnumerator(_IN _REF CXMLNode& rNode);
		virtual ~CXMLEnumerator();

	public:
		//Keeps a reference to the node returned by [rDocument.RootElement()]
		CXMLEnumerator(_IN _REF CXMLDocument& rDocument);

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }
		//Returns true if we already queried past the end of the enumeration, that is Next() already returned FALSE 

	public:
		virtual CXMLNode* Current() _GET { return (CXMLNode*)CEnumeratorRaw::ProtectedCurrent(); }


	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { return FALSE; }

	private:
		CStack<CXMLNode> m_Stack;
	};
};