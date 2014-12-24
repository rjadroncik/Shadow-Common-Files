#pragma once
#include "StreamMemory.h"
#include "StreamRead.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamMemoryRead : public CStreamMemory, public virtual IStreamRead 
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamMemoryRead; }
		CString   ToString() _GET { return STRING("{StreamMemoryRead}"); }

	public:
		CStreamMemoryRead(_INOUT CMemoryBlock& rMemoryBlock, _IN SCF::UINT64 ui64Start, _IN SCF::UINT64 ui64Length);
		CStreamMemoryRead(_INOUT CMemoryBlock& rMemoryBlock);
		virtual ~CStreamMemoryRead();

	public:
		virtual void Close() {}
		virtual bool IsOpen() _GET { return TRUE; }

	public:
		//************************* IStreamRead ********************************
		SCF::UINT64 BytesLeft();
		SCF::UINT64 BytesRead();

	public:
		SCF::BYTE  GetByte() ;
		SCF::WORD  GetWord() ;
		int        GetInt()  ;
		SCF::INT64 GetInt64();

		float      GetFloat();
		bool       GetBool() ;
		SCF::TCHAR GetChar() ;

		void GetBytes(_OUT void* vpOutBuffer, _IN SCF::UINT uiCount);
	
	public:
		void SkipBytes (_IN SCF::UINT uiCount);
		void UnGetBytes(_IN SCF::UINT uiCount);
	};
};