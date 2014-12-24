#pragma once

#include "Node.h"

namespace SCFXML
{
	class XML_API CXMLDocument;

	//This is a PREFIX tree enumerator, postfix not yet implemented (didn't need it yet)
	class XML_API CXMLEnumerator : public SCFBase::CEnumerator
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLEnumerator; }

	public:
		CXMLEnumerator(_IN _REF CXMLNode& rNode);
		virtual ~CXMLEnumerator();

	public:
		//Keeps a reference to the node returned by [rDocument.RootElement()]
		CXMLEnumerator(_IN _REF CXMLDocument& rDocument);

	public:
		inline CXMLNode* Current() _GET { return (CXMLNode*)m_pCurrent; }

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { return FALSE; }

	private:
		CStack m_Stack;
	};
};