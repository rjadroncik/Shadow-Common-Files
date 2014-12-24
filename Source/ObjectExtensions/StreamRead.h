#pragma once
#include "Stream.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CObjectSerializable;
	class OBJECT_EXTENSIONS_API IStreamRead : virtual public IStreamBase
	{
	public:
		virtual SCF::UINT64 BytesLeft() = 0;
		virtual SCF::UINT64 BytesRead() = 0;

	public:
		virtual SCF::BYTE  GetByte()  = 0;
		virtual SCF::WORD  GetWord()  = 0;
		virtual int        GetInt()   = 0;
		virtual SCF::INT64 GetInt64() = 0;

		virtual float      GetFloat() = 0;
		virtual bool       GetBool()  = 0;
		virtual SCF::TCHAR GetChar()  = 0;
	
		virtual void GetBytes(_OUT void* vpOutBuffer, _IN SCF::UINT uiCount) = 0;

	public:
		virtual void SkipBytes (_IN SCF::UINT uiCount) = 0;
		virtual void UnGetBytes(_IN SCF::UINT uiCount) = 0;
	};
};