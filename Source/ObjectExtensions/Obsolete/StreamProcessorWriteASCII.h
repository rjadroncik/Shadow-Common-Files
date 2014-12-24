#pragma once
#include "StreamProcessorWrite.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CStreamProcessorWriteASCII : public CStreamProcessorWrite 
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamProcessorWriteASCII; }

	public:
		CStreamProcessorWriteASCII();
		virtual ~CStreamProcessorWriteASCII();

	public:
		void Process(_IN void* vpData, _IN SCF::UINT uiBytes, _OUT IStreamWrite& rStream);
	};
};