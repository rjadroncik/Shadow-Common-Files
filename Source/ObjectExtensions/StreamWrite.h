#pragma once
#include "Stream.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CObjectSerializable;
	class OBJECT_EXTENSIONS_API IStreamWrite : virtual public IStreamBase
	{
	public:
		virtual void PutByte (_IN SCF::BYTE  ucValue)  = 0;
		virtual void PutWord (_IN SCF::WORD  usValue)  = 0;
		virtual void PutInt  (_IN int        iValue)   = 0;
		virtual void PutInt64(_IN SCF::INT64 i64Value) = 0;

		virtual void PutFloat(_IN float      fValue)   = 0;
		virtual void PutBool (_IN bool       bValue)   = 0;
		virtual void PutChar (_IN SCF::TCHAR cValue)   = 0;

		virtual void PutBytes(_IN void* vpBytes, _IN SCF::UINT uiCount) = 0;

	public:
		virtual SCF::UINT64 BytesWritten() _GET = 0;
		virtual bool        Appending()    _GET = 0;
	};
};