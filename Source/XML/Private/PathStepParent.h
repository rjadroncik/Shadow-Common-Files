#pragma once

#include "PathStep.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepParent : public CXMLPathStep
	{
	public:
		CXMLPathStepParent();
		virtual ~CXMLPathStepParent();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
