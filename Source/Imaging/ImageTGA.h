#pragma once

#include "Image.h"

namespace SCFImaging
{
	class IMAGING_API CImageTGA : public CImage
	{
	public:
		CImageTGA(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions);
		~CImageTGA();
	};
};