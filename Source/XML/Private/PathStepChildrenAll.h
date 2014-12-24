#pragma once

#include "PathStep.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepChildrenAll : public CXMLPathStep
	{
	public:
		CXMLPathStepChildrenAll();
		virtual ~CXMLPathStepChildrenAll();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
