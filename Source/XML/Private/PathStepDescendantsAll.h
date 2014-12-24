#pragma once

#include "PathStep.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepDescendantsAll : public CXMLPathStep
	{
	public:
		CXMLPathStepDescendantsAll();
		virtual ~CXMLPathStepDescendantsAll();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
