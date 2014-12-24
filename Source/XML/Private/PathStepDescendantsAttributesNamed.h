#pragma once

#include "PathStepNamed.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepDescendantsAttributesNamed : public CXMLPathStepNamed
	{
	public:
		CXMLPathStepDescendantsAttributesNamed();
		virtual ~CXMLPathStepDescendantsAttributesNamed();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
