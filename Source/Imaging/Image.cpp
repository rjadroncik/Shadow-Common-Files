#include "Image.h"
#include "ImageJPEG.h"
#include "ImageBMP.h"
#include "ImageTGA.h"
#include "ImageDDS.h"

#include <SCFWinAPI.h>

using namespace SCFImaging;
using namespace SCFBase;

CImage::CImage() : m_Data(0)
{
    m_uiWidth  = 0;				
    m_uiHeight = 0;				

    m_eCompression = CT_NONE;

    m_uiChannels    = 0;	
    m_uiBits        = 0;	
    m_uiMipmapCount = 0;
}

CImage::~CImage() 
{
}

CImage* CImage::Compose(_IN CString& rFileRGBPath, _IN DWORD dwRGBOptions, _IN CString& rFileAlphaPath, _IN DWORD dwAlphaOptions)
{
    //Temporary images
    CImage* pImageRGB   = CImage::Load(rFileRGBPath,   dwRGBOptions);
    CImage* pImageAlpha = CImage::Load(rFileAlphaPath, dwAlphaOptions);
                                                                                                                             
    //Load temporary images
    if (!pImageRGB->m_Data.Value())   { delete pImageRGB; delete pImageAlpha; return NULL; }
    if (!pImageAlpha->m_Data.Value()) { delete pImageRGB; delete pImageAlpha; return NULL; }

    if ((pImageRGB->m_uiWidth != pImageAlpha->m_uiWidth) || (pImageRGB->m_uiHeight != pImageAlpha->m_uiHeight)) { delete pImageRGB; delete pImageAlpha; return NULL; }

    CImage* pResult = new CImage();

    pResult->m_uiBits     = 32;
    pResult->m_uiChannels = 4;
 
    pResult->m_uiWidth  = pImageRGB->m_uiWidth;
    pResult->m_uiHeight = pImageRGB->m_uiHeight;

    pResult->m_Data.Size(pResult->m_uiWidth * pResult->m_uiHeight * pResult->m_uiChannels);

    BYTE* bpDataResult = (BYTE*)pResult->m_Data.Value();
    BYTE* bpDataRGB    = (BYTE*)pImageRGB->m_Data.Value();
    BYTE* bpDataAlpha  = (BYTE*)pImageAlpha->m_Data.Value();

    for (UINT i = 0; i < pResult->m_uiHeight; i++)
    {
        for (UINT j = 0; j < pResult->m_uiWidth; j++)
        {
            bpDataResult[(i * pResult->m_uiWidth + j) * pResult->m_uiChannels]     = bpDataRGB[(i * pImageRGB->m_uiWidth + j) * pImageRGB->m_uiChannels];
            bpDataResult[(i * pResult->m_uiWidth + j) * pResult->m_uiChannels + 1] = bpDataRGB[(i * pImageRGB->m_uiWidth + j) * pImageRGB->m_uiChannels + 1];
            bpDataResult[(i * pResult->m_uiWidth + j) * pResult->m_uiChannels + 2] = bpDataRGB[(i * pImageRGB->m_uiWidth + j) * pImageRGB->m_uiChannels + 2];

            bpDataResult[(i * pResult->m_uiWidth + j) * pResult->m_uiChannels + 3] = bpDataAlpha[(i * pImageAlpha->m_uiWidth + j) * pImageAlpha->m_uiChannels];
        }
    }

    return pResult;
}

CImage* CImage::Load(_IN CString& rFilePath, _IN DWORD dwOptions)
{
    CFile File(rFilePath);
    CStreamFileRead ReadStream(File);
    
    if (ReadStream.IsOpen()) 
    { 
        return Load(File.Extension(), dwOptions, ReadStream);
    }
    else { return NULL; }
}

CImage* CImage::Load(_IN CString& rFileExtension, _IN DWORD dwOptions, _INOUT IStreamRead& rStream)
{
    if (rFileExtension.IsEqualCI(STRING("bmp")))  { return new CImageBMP (rStream, dwOptions); }
    if (rFileExtension.IsEqualCI(STRING("dds")))  { return new CImageDDS (rStream, dwOptions); }
    if (rFileExtension.IsEqualCI(STRING("tga")))  { return new CImageTGA (rStream, dwOptions); }
    if (rFileExtension.IsEqualCI(STRING("jpg")))  { return new CImageJPEG(rStream, dwOptions); }
    if (rFileExtension.IsEqualCI(STRING("jpeg"))) { return new CImageJPEG(rStream, dwOptions); }

    return NULL;
}
