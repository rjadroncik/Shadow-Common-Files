#pragma once
#include "StreamString.h"
#include "StreamRead.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamStringRead : public CStreamString, public virtual IStreamRead 
	{
	public:
		CString ToString() _GET { return STRING("{StreamStringRead}"); }

	public:
		CStreamStringRead(_INOUT CString& rString, _IN UINT64 ui64Start, _IN UINT64 ui64Length);
		CStreamStringRead(_INOUT CString& rString);
		virtual ~CStreamStringRead();

	public:
		virtual void Close() {}
		virtual bool IsOpen() _GET { return TRUE; }

	public:
		//************************* IStreamRead ********************************
		UINT64 BytesLeft();
		UINT64 BytesRead();

	public:
		BYTE  GetByte() ;
		WORD  GetWord() ;
		int        GetInt()  ;
		INT64 GetInt64();

		float      GetFloat();
		bool       GetBool() ;
		TCHAR GetChar() ;

		void GetBytes(_OUT void* vpOutBuffer, _IN UINT uiCount);
	
	public:
		void SkipBytes (_IN UINT uiCount);
		void UnGetBytes(_IN UINT uiCount);
	};
};