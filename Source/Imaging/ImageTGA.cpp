#include "ImageTGA.h"

#include <SCFWinAPI.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

using namespace SCFImaging;
using namespace SCFBase;

CImageTGA::CImageTGA(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions)
{
	SCF_UNREFERENCED_PARAMETER(dwOptions);

	BYTE ucDataOffset;
	//Read the length from the header to pixel data
	rReadStream.GetBytes(&ucDataOffset, 1);
	rReadStream.SkipBytes(1); 

	BYTE ucTmp;
	//Read in the image type (TGA_RGB, TGA_A, TGA_RLE, ..)
	rReadStream.GetBytes(&ucTmp, 1);

	switch (ucTmp)
	{
	case  2: { m_eCompression = CT_NONE; break; }
	case  3: { m_eCompression = CT_NONE; break; }
	case 10: { m_eCompression = CT_RLE;  break; }
	default: { return; }
	}

	//Skip past general information
	rReadStream.SkipBytes(9); 

	//Read width, height and bits per pixel
	rReadStream.GetBytes((BYTE*)&m_uiWidth,  2);
	rReadStream.GetBytes((BYTE*)&m_uiHeight, 2);
	rReadStream.GetBytes((BYTE*)&m_uiBits,   1);

	//Find pixel data
	rReadStream.SkipBytes(ucDataOffset + 1); 

	//Check for RLE compression
	if (m_eCompression != CT_RLE)
	{
		//Check if the image is 24/32-bit
		if ((m_uiBits == 24) || (m_uiBits == 32))
		{
			m_uiChannels = m_uiBits / 8;

			if (m_uiChannels == 3	) { m_eGLFormat = GL_BGR; }
			if (m_uiChannels == 4	) { m_eGLFormat = GL_BGRA; }

			m_Data.Size(this->Stride() * m_uiHeight);

			//Read all the pixel data
			rReadStream.GetBytes(m_Data.Value(), this->Stride() * m_uiHeight);
		}
		else { return; }
	}
	//Else, it must be Run-Length Encoded (RLE)
	else
	{
		//aaaaabbcccccccc >> a5b2c8

		//Create some variables to hold the ucRLEID, current colors read, channels, & stride.
		BYTE ucRLEID      = 0;
		UINT uiColorsRead = 0;

		m_uiChannels = m_uiBits / 8;

		if (m_uiChannels == 1 ) { m_eGLFormat = GL_ALPHA; }
		if (m_uiChannels == 3 ) { m_eGLFormat = GL_BGR; }
		if (m_uiChannels == 4 ) { m_eGLFormat = GL_BGRA; }

		//Next we want to allocate the memory for the Pixel and create an array,
		//depending on the channel count, to read in for each pixel.
		m_Data.Size(this->Stride() * m_uiHeight);

		BYTE  baColor[4];
		UINT  uiPixelsRead = 0;					

		//Load the Pixel Data
		while (uiPixelsRead < m_uiWidth * m_uiHeight)
		{
			//Read the Current Color Count + 1
			rReadStream.GetBytes(&ucRLEID, 1);

			//Check if we don't have an encoded string of colors
			if (ucRLEID < 128)
			{
				//Increase the count by 1
				ucRLEID++;

				//Go through and read all the unique colors found
				while (ucRLEID)
				{
					//Read in the current color
					rReadStream.GetBytes(baColor, m_uiChannels);

					//Store the current pixel in our image array
					((BYTE*)m_Data.Value())[uiColorsRead + 0] = baColor[0];
					((BYTE*)m_Data.Value())[uiColorsRead + 1] = baColor[1];
					((BYTE*)m_Data.Value())[uiColorsRead + 2] = baColor[2];

					//If we have a 4 channel 32-bit image, assign one more for the alpha
					if (m_uiBits == 32) { ((BYTE*)m_Data.Value())[uiColorsRead + 3] = baColor[3]; }

					//Increase the current Pixel read, decrease the amount
					//of Pixel left, and increase the starting index for the next pixel.
					uiPixelsRead++;
					ucRLEID--;
					uiColorsRead += m_uiChannels;
				}
			}
			//Else, let's read in a string of the same character
			else
			{
				//Minus the 128 uiID + 1 (127) to get the color count that needs to be read
				ucRLEID -= 127;

				//Read in the current color, which is the same for a while
				rReadStream.GetBytes(baColor, m_uiChannels);

				// Go and read as many Pixel as are the same
				while (ucRLEID)
				{
					//Assign the current pixel to the current index in our pixel array
					((BYTE*)m_Data.Value())[uiColorsRead + 0] = baColor[0];
					((BYTE*)m_Data.Value())[uiColorsRead + 1] = baColor[1];
					((BYTE*)m_Data.Value())[uiColorsRead + 2] = baColor[2];

					//If we have a 4 channel 32-bit image, assign one more for the alpha
					if (m_uiBits == 32) { ((BYTE*)m_Data.Value())[uiColorsRead + 3] = baColor[3]; }

					//Increase the current Pixel read, decrease the amount
					//of Pixel left, and increase the starting index for the next pixel.
					uiPixelsRead++;
					ucRLEID--;
					uiColorsRead += m_uiChannels;
				}
			}
		}
	}
}

CImageTGA::~CImageTGA()
{
}