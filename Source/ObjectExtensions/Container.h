#pragma once

#include "Object.h"
#include "Enumerator.h"

namespace SCFBase
{
	template <typename TValue>
	class OBJECT_EXTENSIONS_API IContainer
	{
	public:
		virtual UINT Size()    _GET = 0;
		virtual bool IsEmpty() _GET = 0;

	public:
		virtual IEnumerator<TValue>& NewEnumerator() _GET = 0;


	protected:
		virtual ~IContainer() {}
	};
};