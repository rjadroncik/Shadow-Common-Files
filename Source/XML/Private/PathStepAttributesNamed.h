#pragma once

#include "PathStepNamed.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepAttributesNamed : public CXMLPathStepNamed
	{
	public:
		CXMLPathStepAttributesNamed();
		virtual ~CXMLPathStepAttributesNamed();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
