#pragma once
#include "StreamProcessorRead.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CStreamProcessorReadASCII : public CStreamProcessorRead
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamProcessorReadASCII; }

	public:
		CStreamProcessorReadASCII();
		virtual ~CStreamProcessorReadASCII();

	public:
		void Process(_IN void* vpData, _IN SCF::UINT uiBytes, _OUT IStreamRead& rStream);
	};
}