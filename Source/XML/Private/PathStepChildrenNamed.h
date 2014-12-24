#pragma once

#include "PathStepNamed.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepChildrenNamed : public CXMLPathStepNamed
	{
	public:
		CXMLPathStepChildrenNamed();
		virtual ~CXMLPathStepChildrenNamed();

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;
	};
};
