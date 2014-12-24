#include <SCFStandard.h>

#include <SCFWinAPI.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#include <ddraw.h>

#include "ImageDDS.h"

namespace SCFImagingPrivate
{
	//This is a x64 compatible version of the structure,
	//Microsoft should learn NOT to store pointers in files :D
	struct DDSURFACEDESC2_X64
	{
		DWORD               dwSize;                 // size of the DDSURFACEDESC structure
		DWORD               dwFlags;                // determines what fields are valid
		DWORD               dwHeight;               // height of surface to be created
		DWORD               dwWidth;                // width of input surface
		union
		{
			LONG            lPitch;                 // distance to start of next line (return value only)
			DWORD           dwLinearSize;           // Formless late-allocated optimized surface size
		} DUMMYUNIONNAMEN(1);
		union
		{
			DWORD           dwBackBufferCount;      // number of back buffers requested
			DWORD           dwDepth;                // the depth if this is a volume texture 
		} DUMMYUNIONNAMEN(5);
		union
		{
			DWORD           dwMipMapCount;          // number of mip-map levels requested
			// dwZBufferBitDepth removed, use ddpfPixelFormat one instead
			DWORD           dwRefreshRate;          // refresh rate (used when display mode is described)
			DWORD           dwSrcVBHandle;          // The source used in VB::Optimize
		} DUMMYUNIONNAMEN(2);
		DWORD               dwAlphaBitDepth;        // depth of alpha buffer requested
		DWORD               dwReserved;             // reserved
		DWORD               lpSurface;              // pointer to the associated surface memory
		union
		{
			DDCOLORKEY      ddckCKDestOverlay;      // color key for destination overlay use
			DWORD           dwEmptyFaceColor;       // Physical color for empty cube-map faces
		} DUMMYUNIONNAMEN(3);
		DDCOLORKEY          ddckCKDestBlt;          // color key for destination blt use
		DDCOLORKEY          ddckCKSrcOverlay;       // color key for source overlay use
		DDCOLORKEY          ddckCKSrcBlt;           // color key for source blt use
		union
		{
			DDPIXELFORMAT   ddpfPixelFormat;        // pixel format description of the surface
			DWORD           dwFVF;                  // vertex format description of vertex buffers
		} DUMMYUNIONNAMEN(4);
		DDSCAPS2            ddsCaps;                // direct draw surface capabilities
		DWORD               dwTextureStage;         // stage in multi-texture cascade
	};
};

using namespace SCFImaging;
using namespace SCFImagingPrivate;

CImageDDS::CImageDDS(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions)
{
	UNREFERENCED_PARAMETER(dwOptions);

	DDSURFACEDESC2_X64 SurfaceDescriptorX64;
	char			   caFourCC[4];

	//Verify the file is a true DDS file
	rReadStream.GetBytes(caFourCC, 4);

	if (memcmp(caFourCC, "DDS ", 4) != 0) { return; }

	//Read the surface descriptor
	rReadStream.GetBytes(&SurfaceDescriptorX64, sizeof(SurfaceDescriptorX64));

	if (SurfaceDescriptorX64.dwSize == sizeof(DDSURFACEDESC2_X64)) 
	{
		switch (SurfaceDescriptorX64.ddpfPixelFormat.dwFourCC)
		{
		case FOURCC_DXT1:
			{
				m_eGLFormat    = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				m_eCompression = CT_DXT1;
				break;
			}
		case FOURCC_DXT3:
			{
				m_eGLFormat    = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				m_eCompression = CT_DXT3;
				break;
			}
		case FOURCC_DXT5:
			{
				m_eGLFormat    = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				m_eCompression = CT_DXT5;
				break;
			}
		default: { return; }
		}

		//How big will the buffer need to be to load all of the pixel data including mip-maps?
		if (SurfaceDescriptorX64.dwLinearSize == 0) { return; }

		if (SurfaceDescriptorX64.ddpfPixelFormat.dwFourCC == FOURCC_DXT1) { m_uiChannels = 3; }
		else                                                              { m_uiChannels = 4; }

		m_uiWidth               = SurfaceDescriptorX64.dwWidth;
		m_uiHeight              = SurfaceDescriptorX64.dwHeight;
		m_uiMipmapCount         = SurfaceDescriptorX64.dwMipMapCount;
		m_uiBits                = m_uiChannels * 8;
		m_uiFirstMipmapDataSize = SurfaceDescriptorX64.dwLinearSize;

		//Allocate memory for data
		m_Data.Size(MipmapDataOffset(m_uiMipmapCount));

		//Read compressed data
		rReadStream.GetBytes(m_Data.Value(), m_Data.Size());
	}
	else if (SurfaceDescriptorX64.dwSize == sizeof(DDSURFACEDESC2)) 
	{
		DDSURFACEDESC2* pSurfaceDescriptor = (DDSURFACEDESC2*)&SurfaceDescriptorX64;

		rReadStream.UnGetBytes(sizeof(DDSURFACEDESC2_X64) - sizeof(DDSURFACEDESC2));

		switch (pSurfaceDescriptor->ddpfPixelFormat.dwFourCC)
		{
		case FOURCC_DXT1:
			{
				m_eGLFormat    = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				m_eCompression = CT_DXT1;
				break;
			}
		case FOURCC_DXT3:
			{
				m_eGLFormat    = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				m_eCompression = CT_DXT3;
				break;
			}
		case FOURCC_DXT5:
			{
				m_eGLFormat    = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				m_eCompression = CT_DXT5;
				break;
			}
		default: { return; }
		}

		//How big will the buffer need to be to load all of the pixel data including mip-maps?
		if (pSurfaceDescriptor->dwLinearSize == 0) { return; }

		if (pSurfaceDescriptor->ddpfPixelFormat.dwFourCC == FOURCC_DXT1) { m_uiChannels = 3; }
		else                                                             { m_uiChannels = 4; }

		m_uiWidth               = pSurfaceDescriptor->dwWidth;
		m_uiHeight              = pSurfaceDescriptor->dwHeight;
		m_uiMipmapCount         = pSurfaceDescriptor->dwMipMapCount;
		m_uiBits                = m_uiChannels * 8;
		m_uiFirstMipmapDataSize = pSurfaceDescriptor->dwLinearSize;

		//Allocate memory for data
		m_Data.Size(MipmapDataOffset(m_uiMipmapCount));

		//Read compressed data
		rReadStream.GetBytes(m_Data.Value(), m_Data.Size());
	}
}

UINT CImageDDS::MipmapDataOffset(_IN UINT uiLevel) _GET
{
	UINT uiTotalOffset = 0;
	UINT uiSize        = m_uiFirstMipmapDataSize;

	for (UINT i = 0; i < uiLevel; i++)
	{
		uiTotalOffset += uiSize;

		//Compute the size of next mip-map level, 
		switch (m_eCompression)
		{
		case CT_NONE: { uiSize = uiSize / 4;            break; }
		case CT_DXT1: { uiSize = __max(8, uiSize / 4);  break; }
		case CT_DXT3:
		case CT_DXT5: { uiSize = __max(16, uiSize / 4); break; }
		}
	}

	return uiTotalOffset;
}

UINT CImageDDS::MipmapWidth(_IN UINT uiLevel) _GET
{
	UINT uiWidth = m_uiWidth;
	for (UINT i = 0; i < uiLevel; i++) { uiWidth /= 2; }
	return uiWidth;
}

UINT CImageDDS::MipmapHeight(_IN UINT uiLevel) _GET
{
	UINT uiHeight = m_uiHeight;
	for (UINT i = 0; i < uiLevel; i++) { uiHeight /= 2; }
	return uiHeight;
}

CImageDDS::~CImageDDS()
{
}