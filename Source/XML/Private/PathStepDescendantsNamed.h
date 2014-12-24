#pragma once

#include "PathStepNamed.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepDescendantsNamed : public CXMLPathStepNamed
	{
	public:
		CXMLPathStepDescendantsNamed();
		virtual ~CXMLPathStepDescendantsNamed();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
