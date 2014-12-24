#pragma once

#include "ObjectSerializable.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumerator;

	class OBJECT_EXTENSIONS_API CContainer : public CObjectSerializable
	{
	public:
		virtual CEnumerator* EnumeratorNew() _GET = 0;

	public:
		virtual SCF::UINT Size()    _GET = 0;
		virtual bool      IsEmpty() _GET = 0;
	};
};