#pragma once

#include "Image.h"

namespace SCFImaging
{
	class PICTURE_API CImageTGA : public CImage
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassImageTGA; }

	public:
		CImageTGA(_INOUT IStreamRead& rReadStream, _IN SCF::DWORD dwOptions);
		~CImageTGA();
	};
};