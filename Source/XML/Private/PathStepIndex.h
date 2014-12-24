#pragma once

#include "PathStepNamed.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepIndex : public CXMLPathStep
	{
	public:
		CXMLPathStepIndex();
		virtual ~CXMLPathStepIndex();

	public:
		inline void      Index(_IN int iIndex) _SET { m_iIndex = iIndex; }
		inline const int Index()               _GET { return m_iIndex; }

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;

	private:
		int m_iIndex;
	};
};
