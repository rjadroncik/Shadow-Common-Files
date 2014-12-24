#pragma once

#include "BoundValue.h"

namespace SCFXMLPrivate
{
	class CXMLBoundValueAttribute: public CXMLBoundValue
	{
	public:
		CXMLBoundValueAttribute();
		virtual ~CXMLBoundValueAttribute();

	public:
		void Name(_IN CString& rName) _SET { m_Name = rName; }

	public:
		const CValue* Evaluate(_IN SCFXML::CXMLNode& rCurrent) const;

	private:
		CString m_Name;
	};
};
