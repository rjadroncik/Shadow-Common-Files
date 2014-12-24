#pragma once

#include "PathStepNamed.h"
#include "Classes.h"

namespace SCFXMLPrivate
{
	class CXMLPathPredicate : public SCFBase::CObject
	{
	public:
		CXMLPathPredicate();
		virtual ~CXMLPathPredicate();

	public:
		virtual bool Match(_IN SCFXML::CXMLNode& rNode) const = 0;
	};
};
