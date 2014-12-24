#pragma once

#include "PathStep.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepAttributesAll : public CXMLPathStep
	{
	public:
		CXMLPathStepAttributesAll();
		virtual ~CXMLPathStepAttributesAll();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
