#pragma once

#include "PathStep.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepDescendantsAttributesAll : public CXMLPathStep
	{
	public:
		CXMLPathStepDescendantsAttributesAll();
		virtual ~CXMLPathStepDescendantsAttributesAll();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
