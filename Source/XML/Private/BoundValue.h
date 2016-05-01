#pragma once

#include "../Node.h"

namespace SCFXMLPrivate
{
	class CXMLBoundValue: public SCFBase::CObject
	{
	public:
		ENUM ClassKey() _GET { return (ENUM)-1; }

	public:
		CXMLBoundValue();
		virtual ~CXMLBoundValue();

	public:
		//Does NOT create a new object on every evaluation -> MUST NOT delete the returned object
		virtual const CValue* Evaluate(_IN SCFXML::CXMLNode& rCurrent) const = 0;
	};
};
