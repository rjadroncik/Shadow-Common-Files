#pragma once

#include "Image.h"

namespace SCFImaging
{
	class PICTURE_API CImageTGA : public CImage
	{
	public:
		ENUM ClassKey() _GET { return ClassImageTGA; }

	public:
		CImageTGA(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions);
		~CImageTGA();
	};
};