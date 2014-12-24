#pragma once
#include "StreamProcessor.h"
#include "StreamWrite.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CStreamProcessorWrite : public CStreamProcessor 
	{
	public:
		CStreamProcessorWrite();
		virtual ~CStreamProcessorWrite();

	public:
		virtual void Process(_IN void* vpData, _IN SCF::UINT uiBytes, _OUT IStreamWrite& rStream) = 0;
	};
};