#pragma once

#include "Image.h"

namespace SCFImaging
{
	class PICTURE_API CImageDDS : public CImage
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassImageDDS; }

	public:
		CImageDDS(_INOUT IStreamRead& rReadStream, _IN SCF::DWORD dwOptions);
		~CImageDDS();

	public:
		void*      MipmapData    (_IN SCF::UINT uiLevel) _GET { return (SCF::BYTE*)m_Data.Value() + MipmapDataOffset(__min(m_uiMipmapCount, uiLevel)); }
		SCF::UINT  MipmapDataSize(_IN SCF::UINT uiLevel) _GET { return MipmapDataOffset(__min(m_uiMipmapCount + 1, uiLevel + 1)) - MipmapDataOffset(__min(m_uiMipmapCount, uiLevel)); }

		SCF::UINT MipmapCount() _GET { return m_uiMipmapCount; }

		SCF::UINT MipmapWidth (_IN SCF::UINT uiLevel) _GET;
		SCF::UINT MipmapHeight(_IN SCF::UINT uiLevel) _GET;

	protected:
		SCF::UINT MipmapDataOffset(_IN SCF::UINT uiLevel) _GET;

	protected:
		SCF::UINT m_uiFirstMipmapDataSize;
	};
};


