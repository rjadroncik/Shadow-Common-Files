#pragma once

#include "../Node.h"

namespace SCFXML { class CXMLPath; };

namespace SCFXMLPrivate
{
	class CXMLPathStep : public SCFBase::CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return (SCF::ENUM)-1; }

	public:
		CXMLPathStep();
		virtual ~CXMLPathStep();

	public:
		inline CXMLPathStep* Next()                        _GET { return (CXMLPathStep*)m_pNext; }
		inline void          Next(_IN CXMLPathStep* pNode) _SET { m_pNext = (CXMLPathStep*)pNode; }

	public:
		virtual void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const = 0;

	private:
		CXMLPathStep* m_pNext;
	};
};
