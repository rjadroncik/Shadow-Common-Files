#pragma once

#include "Image.h"

namespace SCFImaging
{
	class PICTURE_API CImageDDS : public CImage
	{
	public:
		ENUM ClassKey() _GET { return ClassImageDDS; }

	public:
		CImageDDS(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions);
		~CImageDDS();

	public:
		void*      MipmapData    (_IN UINT uiLevel) _GET { return (BYTE*)m_Data.Value() + MipmapDataOffset(__min(m_uiMipmapCount, uiLevel)); }
		UINT  MipmapDataSize(_IN UINT uiLevel) _GET { return MipmapDataOffset(__min(m_uiMipmapCount + 1, uiLevel + 1)) - MipmapDataOffset(__min(m_uiMipmapCount, uiLevel)); }

		UINT MipmapCount() _GET { return m_uiMipmapCount; }

		UINT MipmapWidth (_IN UINT uiLevel) _GET;
		UINT MipmapHeight(_IN UINT uiLevel) _GET;

	protected:
		UINT MipmapDataOffset(_IN UINT uiLevel) _GET;

	protected:
		UINT m_uiFirstMipmapDataSize;
	};
};


