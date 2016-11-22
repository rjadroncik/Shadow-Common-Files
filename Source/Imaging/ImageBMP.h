#pragma once

#include "Image.h"

namespace SCFImaging
{
	class IMAGING_API CImageBMP : public CImage
	{
	public:
		CImageBMP(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions);
		virtual ~CImageBMP();
	};
};