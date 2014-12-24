#pragma once
#include "StreamProcessor.h"
#include "StreamRead.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CStreamProcessorRead : public CStreamProcessor 
	{
	public:
		CStreamProcessorRead();
		virtual ~CStreamProcessorRead();

	public:
		virtual void Process(_IN void* vpData, _IN SCF::UINT uiBytes, _OUT IStreamRead& rStream) = 0;
	};
};