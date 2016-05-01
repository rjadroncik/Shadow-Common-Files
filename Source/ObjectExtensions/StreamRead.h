#pragma once
#include "Stream.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CObject;
	class OBJECT_EXTENSIONS_API IStreamRead : virtual public IStreamBase
	{
	public:
		virtual UINT64 BytesLeft() = 0;
		virtual UINT64 BytesRead() = 0;

	public:
		virtual BYTE  GetByte()  = 0;
		virtual WORD  GetWord()  = 0;
		virtual int        GetInt()   = 0;
		virtual INT64 GetInt64() = 0;

		virtual float      GetFloat() = 0;
		virtual bool       GetBool()  = 0;
		virtual TCHAR GetChar()  = 0;
	
		virtual void GetBytes(_OUT void* vpOutBuffer, _IN UINT uiCount) = 0;

	public:
		virtual void SkipBytes (_IN UINT uiCount) = 0;
		virtual void UnGetBytes(_IN UINT uiCount) = 0;
	};
};