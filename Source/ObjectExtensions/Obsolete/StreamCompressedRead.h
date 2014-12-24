#pragma once
#include "StreamMemory.h"
#include "StreamRead.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CStreamCompressedRead : public CStream, public virtual IStreamRead 
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamCompressedRead; }

	public:
		CStreamCompressedRead(_IN IStreamRead& rReadStream);
		virtual ~CStreamCompressedRead();

	public:
		virtual void Close() {}
		virtual bool IsOpen() _GET { return TRUE; }

	public:
		//************************* IStreamRead ********************************
		SCF::UINT64 BytesLeft();
		SCF::UINT64 BytesRead();

		SCF::BYTE  GetByte() ;
		SCF::WORD  GetWord() ;
		int   GetInt()  ;
		SCF::INT64 GetInt64();

		float GetFloat();
		bool  GetBool() ;
		SCF::TCHAR GetChar() ;

		void GetBytes(_OUT void* vpOutBuffer, _IN SCF::UINT uiCount);

		void SkipBytes (_IN SCF::UINT uiCount);
		void UnGetBytes(_IN SCF::UINT uiCount);
	};
};