#pragma once

#include "../Node.h"
#include "BoundValue.h"

namespace SCFXMLPrivate
{
	class CXMLBoundValueString: public CXMLBoundValue
	{
	public:
		CXMLBoundValueString();
		virtual ~CXMLBoundValueString();

	public:
		void Value(_IN CString& rValue) _SET;

	public:
		const CValue* Evaluate(_IN SCFXML::CXMLNode& rCurrent) const;

	private:
		const CValue* m_pValue;
	};
};
