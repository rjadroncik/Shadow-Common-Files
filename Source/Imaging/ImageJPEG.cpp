#include "ImageJPEG.h"

#include <SCFWinAPI.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <string.h>

using namespace SCFBase;
using namespace SCF;

namespace SCFImagingPrivate
{
	struct SHuffmanTable
	{
		BYTE bits[16];
		BYTE hval[256];
		BYTE size[256];
		WORD code[256];
	};

	struct SDecodeData 
	{
		SHuffmanTable TableAC[4];
		SHuffmanTable TableDC[4];
		int           TableQuant[4][64];

		struct SComponentInfo
		{
			UINT uiID;

			//Sampling factors
			int iSamplingH;
			int iSamplingV; 

			//Table selectors
			UINT uiTableAC;
			UINT uiTableDC;
			UINT uiTableQuant;

		} Component[3];

		struct SScanInfo
		{
			//Progressive jpeg spectral selection
			int ss, se; 
			//Progressive jpeg successive approx
			int ah, al; 

		} Scan;

		UINT uiRestartInterval;
		int iaDC[3];

		//Data used to synchronize bit reading
		UINT uiCurBit;
		BYTE ucCurByte;
	};
};

using namespace SCFImagingPrivate;
using namespace SCFImaging;

BYTE CImageJPEG::ReadByte(_INOUT IStreamRead& rStream, _INOUT SDecodeData& rData)
{
    rData.ucCurByte = rStream.GetByte();
    rData.uiCurBit = 0;
 
	return rData.ucCurByte;
}

WORD CImageJPEG::ReadWord(_INOUT IStreamRead& rStream, _INOUT SDecodeData& rData)
{
    WORD i = rStream.GetWord();
	rData.uiCurBit = 0;

    return ((i << 8) & 0xFF00) + ((i >> 8) & 0x00FF);
}

BYTE CImageJPEG::ReadBit(_INOUT IStreamRead& rStream, _INOUT SDecodeData& rData)
{
    if (rData.uiCurBit == 0) 
	{
        ReadByte(rStream, rData);

        if (rData.ucCurByte == 0xFF) 
		{
			while(rData.ucCurByte == 0xFF) { ReadByte(rStream, rData); }

            if (rData.ucCurByte >= 0xD0 && rData.ucCurByte <= 0xD7)
			{
                memset(rData.iaDC, 0, sizeof(int) * 3);
			}

			if (rData.ucCurByte == 0) { rData.ucCurByte = 0xFF; }
			else                      { ReadByte(rStream, rData); }
        }
    }

    BYTE i = (BYTE)(rData.ucCurByte >> (7 - rData.uiCurBit++)) & 0x01;

	if (rData.uiCurBit == 8) { rData.uiCurBit = 0; }
    
	return i;
}

UINT CImageJPEG::ReadBits(_INOUT IStreamRead& rStream, _INOUT SDecodeData& rData, _IN UINT uiCount) 
{
    register UINT j = 0;

	for (UINT i = 0; i < uiCount; i++) 
	{
        j <<= 1;
        j |= ReadBit(rStream, rData);
    }

    return j;
}

int CImageJPEG::BitToInt(WORD bit, UINT i)
{
	if ((i & (1 << (bit - 1))) > 0) { return i; }

    return -(int)(i ^ ((1 << bit) - 1));
}

WORD CImageJPEG::HuffmanCode(_INOUT IStreamRead& rStream, _INOUT SDecodeData& rData, _IN SHuffmanTable& rTable)
{
	register WORD code = 0;

    for (BYTE size = 1; size < 17; size++) 
	{
        code <<= 1;
        code |= ReadBit(rStream, rData);

        for (BYTE i = 0; rTable.size[i] <= size; i++)
		{
			if (rTable.code[i] == code) { return rTable.hval[i]; }
        }
    }

    return code;
}

void CImageJPEG::InverseDCT(float *data)
{
    float t0,t1,t2,t3,t4,t5,t6,t7;
    float t10,t11,t12,t13;

    float z5,z10,z11,z12,z13;
    float *dataptr;
 
	dataptr = data;
    for (int i = 0; i < 8; i++) 
	{
        t0 = dataptr[8 * 0];
        t1 = dataptr[8 * 2];
        t2 = dataptr[8 * 4];
        t3 = dataptr[8 * 6];

        t10 = t0 + t2;
        t11 = t0 - t2;
        t13 = t1 + t3;
        t12 = - t13 + (t1 - t3) * 1.414213562f;

        t0 = t10 + t13;
        t3 = t10 - t13;
        t1 = t11 + t12;
        t2 = t11 - t12;

        t4 = dataptr[8 * 1];
        t5 = dataptr[8 * 3];
        t6 = dataptr[8 * 5];
        t7 = dataptr[8 * 7];

        z13 = t6 + t5;
        z10 = t6 - t5;
        z11 = t4 + t7;
        z12 = t4 - t7;

        t7  =  z11 + z13;
        t11 = (z11 - z13) * 1.414213562f;
        z5  = (z10 + z12) * 1.847759065f;
        t10 = - z5 + z12  * 1.082392200f;
        t12 =   z5 - z10  * 2.613125930f;
        t6  =  t12 - t7;
        t5  =  t11 - t6;
        t4  =  t10 + t5;

        dataptr[8 * 0] = t0 + t7;
        dataptr[8 * 7] = t0 - t7;
        dataptr[8 * 1] = t1 + t6;
        dataptr[8 * 6] = t1 - t6;
        dataptr[8 * 2] = t2 + t5;
        dataptr[8 * 5] = t2 - t5;
        dataptr[8 * 4] = t3 + t4;
        dataptr[8 * 3] = t3 - t4;
  
		dataptr++;
    }

    dataptr = data;
    for (int i = 0; i < 8; i++)
	{
        t10 = dataptr[0] + dataptr[4];
        t11 = dataptr[0] - dataptr[4];
        t13 = dataptr[2] + dataptr[6];
        t12 = - t13 + (dataptr[2] - dataptr[6]) * 1.414213562f;

        t0  = t10 + t13;
        t3  = t10 - t13;
        t1  = t11 + t12;
        t2  = t11 - t12;

        z13 = dataptr[5] + dataptr[3];
        z10 = dataptr[5] - dataptr[3];
        z11 = dataptr[1] + dataptr[7];
        z12 = dataptr[1] - dataptr[7];

        t7  =  z11 + z13;
        t11 = (z11 - z13) * 1.414213562f;
        z5  = (z10 + z12) * 1.847759065f;
        t10 = - z5 + z12  * 1.082392200f;
        t12 =   z5 - z10  * 2.613125930f;
        t6  =  t12 - t7;
        t5  =  t11 - t6;
        t4  =  t10 + t5;

        dataptr[0] = t0 + t7;
        dataptr[7] = t0 - t7;
        dataptr[1] = t1 + t6;
        dataptr[6] = t1 - t6;
        dataptr[2] = t2 + t5;
        dataptr[5] = t2 - t5;
        dataptr[4] = t3 + t4;
        dataptr[3] = t3 - t4;

        dataptr += 8;
    }
}

int CImageJPEG::ReadMarkers(_INOUT IStreamRead& rStream, _INOUT SDecodeData& rData)
{    
	for (;;) 
	{
		if (ReadByte(rStream, rData) != 0xFF) { return 0; }

        BYTE ucMarker = ReadByte(rStream, rData);
        if (ucMarker != 0xD8) 
		{
            UINT uiLength = ReadWord(rStream, rData) - 2;

            switch (ucMarker) 
			{
			case 0xC0: //Baseline
				{
					m_uiBits     = ReadByte(rStream, rData);
					m_uiHeight   = ReadWord(rStream, rData);
					m_uiWidth    = ReadWord(rStream, rData);
					m_uiChannels = ReadByte(rStream, rData);

					m_uiBits *= m_uiChannels;

					if ((uiLength - 6) != (m_uiChannels * 3)) { return 0;  }

					for (UINT i = 0; i < m_uiChannels; i++) 
					{
						rData.Component[i].uiID = ReadByte(rStream, rData);
						
						BYTE ucSamling = ReadByte(rStream, rData);
						
						rData.Component[i].iSamplingH = (ucSamling >> 4) & 0x0F;
						rData.Component[i].iSamplingV =  ucSamling       & 0x0F;
						rData.Component[i].uiTableQuant = ReadByte(rStream, rData);
					}
					break;
				}
            case 0xC1: //Extended sequential, Huffman
            case 0xC2: //Progressive, Huffman            
            case 0xC3: //Lossless, Huffman
            case 0xC5: //Differential sequential, Huffman
            case 0xC6: //Differential progressive, Huffman
            case 0xC7: //Differential lossless, Huffman
            case 0xC8: //Reserved for JPEG extensions
            case 0xC9: //Extended sequential, arithmetic
            case 0xCA: //Progressive, arithmetic
            case 0xCB: //Lossless, arithmetic
            case 0xCD: //Differential sequential, arithmetic
            case 0xCE: //Differential progressive, arithmetic
            case 0xCF: //Differential lossless, arithmetic
				{
					return 0;
				}
            case 0xC4: //Huffman table
				{
					while (uiLength > 0)
					{
						SHuffmanTable *pTable;
						BYTE ucTable = ReadByte(rStream, rData);

						if (ucTable & 0x10) { pTable = &rData.TableAC[ucTable & 0x0F]; }
						else                { pTable = &rData.TableDC[ucTable & 0x0F]; }

						UINT uiBits = 0;

						for (int i = 0; i < 16; i++)
						{
							pTable->bits[i] = ReadByte(rStream, rData);
							uiBits += pTable->bits[i];
						}
						uiLength -= 17;

						for (UINT i = 0; i < uiBits; i++) { pTable->hval[i] = ReadByte(rStream, rData); }
						uiLength -= uiBits;

						WORD usCode = 0;

						for (BYTE i = 0, k = 0; i < 16; i++) 
						{
							for (BYTE j = 0; j < pTable->bits[i]; j++, k++)
							{
								pTable->size[k] = i + 1;
								pTable->code[k] = usCode++;
							}
							usCode <<= 1;
						}
					}
					break;
				}
            case 0xDB: //Quantization table
				{
					while(uiLength > 0) 
					{
						BYTE ucTable     = ReadByte(rStream, rData);
						BYTE ucTableHigh = (ucTable >> 4) & 0x0F;

						for (int i = 0; i < 64; i++)
						{
							if (ucTableHigh) { rData.TableQuant[ucTable][i] = ReadWord(rStream, rData); }
							else             { rData.TableQuant[ucTable][i] = ReadByte(rStream, rData); }
						}

						uiLength -= 65;
						if (ucTableHigh) { uiLength -= 64; }
					}
					break;
				}
            case 0xD9: //End of image (EOI)
				{
					return 0;
				}
            case 0xDA: //Start of Scan (SOS)
				{
					BYTE ucNumOfTables = ReadByte(rStream, rData);

					for (BYTE i = 0; i < ucNumOfTables; i++) 
					{
						BYTE ucID       = ReadByte(rStream, rData);
						BYTE ucSelector = ReadByte(rStream, rData);

						for (BYTE l = 0; l < m_uiChannels; l++)
						{
							if (rData.Component[l].uiID == ucID) 
							{
								rData.Component[l].uiTableDC = (ucSelector >> 4) & 0x0F;
								rData.Component[l].uiTableAC =  ucSelector       & 0x0F;
							}
						}
					}

					rData.Scan.ss = ReadByte(rStream, rData);
					rData.Scan.se = ReadByte(rStream, rData);

					BYTE ucApprox = ReadByte(rStream, rData);

					rData.Scan.ah = (ucApprox >> 4) & 0x0F;
					rData.Scan.al =  ucApprox       & 0x0F;
					return 1;
				}
            case 0xDD: //Restart interval
				{
					rData.uiRestartInterval = ReadWord(rStream, rData);
                    break;
				}
            default:
				{
					rStream.SkipBytes(uiLength);
					break;
				}
            }
		}
    }
}

void CImageJPEG::Decompress(_INOUT IStreamRead& rStream, _INOUT SDecodeData& rData)
{    
	BYTE* bpData = (BYTE*)m_Data.Value();

	UINT uiaScaleH[3];
	UINT uiaScaleV[3];

    static float faVector[64];
    static float faDCT[64];

    static const int iaZigZag[64] = 
	{
         0, 1, 5, 6,14,15,27,28,
         2, 4, 7,13,16,26,29,42,
         3, 8,12,17,25,30,41,43,
         9,11,18,24,31,40,44,53,
        10,19,23,32,39,45,52,54,
        20,22,33,38,46,51,55,60,
        21,34,37,47,50,56,59,61,
        35,36,48,49,57,58,62,63 
	};

	//1.0, k = 0; cos(k * M_PI / 16) * sqrt(2), k = 1...7
    static const float faAANScale[8] = 
	{  
        1.0f, 1.387039845f, 1.306562965f, 1.175875602f,
        1.0f, 0.785694958f, 0.541196100f, 0.275899379f 
	};

    uiaScaleH[0] = 1;
    uiaScaleV[0] = 1;

    if (m_uiChannels == 3) 
	{
        uiaScaleH[1] = rData.Component[0].iSamplingH / rData.Component[1].iSamplingH;
        uiaScaleV[1] = rData.Component[0].iSamplingV / rData.Component[1].iSamplingV;
        uiaScaleH[2] = rData.Component[0].iSamplingH / rData.Component[2].iSamplingH;
        uiaScaleV[2] = rData.Component[0].iSamplingV / rData.Component[2].iSamplingV;
    }

    memset(rData.iaDC, 0, sizeof(int) * 3);

    for (UINT Y = 0; Y < m_uiHeight; Y += rData.Component[0].iSamplingV << 3) 
	{
		if (rData.uiRestartInterval > 0) { rData.uiCurBit = 0; }

        for (UINT X = 0; X < m_uiWidth; X += rData.Component[0].iSamplingH << 3) 
		{
            for (UINT plane = 0; plane < m_uiChannels; plane++)
			{
                for (int V = 0; V < rData.Component[plane].iSamplingV; V++)
				{
                    for (int H = 0; H < rData.Component[plane].iSamplingH; H++) 
					{
                        WORD usCode = 0x0F & HuffmanCode(rStream, rData, rData.TableDC[rData.Component[plane].uiTableDC]);

                        faVector[0] = (float)(rData.iaDC[plane] + BitToInt(usCode, ReadBits(rStream, rData, usCode)));
                        rData.iaDC[plane] = (int)faVector[0];
                       
                        for (UINT i = 1; i < 64; ) 
						{
							usCode = HuffmanCode(rStream, rData, rData.TableAC[rData.Component[plane].uiTableAC]);
							
							if (usCode == 0) { while (i < 64) { faVector[i++] = 0; } }
                            else 
							{
                                UINT uiTmp = i + ((usCode >> 4) & 0x0F);

								while (i < uiTmp) { faVector[i++] = 0; }

                                usCode &= 0x0F;
                                faVector[i++] = (float)BitToInt(usCode, ReadBits(rStream, rData, usCode));
                            }
                        }

                        for (UINT y = 0, i = 0; y < 8; y++)
						{
                            for (UINT x = 0; x < 8; x++, i++) 
							{
                                int iIndex = iaZigZag[i];
                                faDCT[i] = faVector[iIndex] * rData.TableQuant[rData.Component[plane].uiTableQuant][iIndex] * faAANScale[x] * faAANScale[y];
                            }
						}

                        InverseDCT(faDCT);

                        for (UINT y = 0; y < 8; y++)
						{
                            for (UINT x = 0; x < 8; x++)
							{
                                int iColor = ((int)faDCT[(y << 3) + x] >> 3) + 128;

								if (iColor < 0) { iColor = 0; }
								else            { if (iColor > 255) { iColor = 255; } }

                                if (uiaScaleH[plane] == 1 && uiaScaleV[plane] == 1) 
								{
                                    UINT i = X + x + (H << 3);
                                    UINT j = Y + y + (V << 3);

									if ((i < m_uiWidth) && (j < m_uiHeight))
									{
                                        bpData[((j * m_uiWidth + i) * m_uiChannels) + plane] = (BYTE)iColor;
									}
                                }
                                else 
								{	for (UINT l = 0; l < uiaScaleV[plane]; l++)
									{
										for (UINT k = 0; k < uiaScaleH[plane]; k++) 
										{
											UINT i = X + (x + (H << 3)) * uiaScaleH[plane] + k;
											UINT j = Y + (y + (V << 3)) * uiaScaleV[plane] + l;

											if ((i < m_uiWidth) && (j < m_uiHeight))
											{
												bpData[((j * m_uiWidth + i) * m_uiChannels) + plane] = (BYTE)iColor;
											}
										}
									}
								}
                            }
						}
                    }
				}
			}
        }
    }
}

void CImageJPEG::YCbCrToRGB()  
{ 
	BYTE* bpData = (BYTE*)m_Data.Value();

	for (UINT i = 0; i < (m_uiWidth * m_uiHeight * m_uiChannels); i += m_uiChannels) 
	{
        int Y  = bpData[i];
        int Cb = bpData[i + 1] - 128;
        int Cr = bpData[i + 2] - 128;

        int R = (int)(Y                 + 1.40200f * Cr);
        int G = (int)(Y - 0.34414f * Cb - 0.71414f * Cr);
        int B = (int)(Y + 1.77200f * Cb);

		if (R < 0) { R = 0;	}
		else       { if (R > 255) { R = 255; } }

        if (G < 0) { G = 0;	}
		else       { if (G > 255) { G = 255; } }

        if (B < 0) { B = 0;	}
		else       { if (B > 255) { B = 255; } }

        bpData[i]     = (BYTE)R;
        bpData[i + 1] = (BYTE)G;
        bpData[i + 2] = (BYTE)B;
		if (m_uiChannels > 3) { bpData[i + 3] = 255; }
    }
}

void CImageJPEG::GrayToRGB() 
{  
	BYTE* bpData = (BYTE*)m_Data.Value();

	for (UINT i = 0; i < (m_uiWidth * m_uiHeight * m_uiChannels); i += m_uiChannels) 
	{
        bpData[i + 1] = bpData[i];
        bpData[i + 2] = bpData[i];
		if (m_uiChannels > 3) { bpData[i + 3] = 255; }
    }
}

CImageJPEG::CImageJPEG(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions)
{
	SCF_UNREFERENCED_PARAMETER(dwOptions);

	SDecodeData DecodingData = { 0 };
	if (!this->ReadMarkers(rReadStream, DecodingData)) { return; }  

	m_Data.Size(m_uiWidth * m_uiHeight * m_uiChannels);
	this->Decompress(rReadStream, DecodingData);

	if (m_uiChannels == 1 ) { m_eGLFormat = GL_ALPHA; this->GrayToRGB(); }
	if (m_uiChannels == 3 ) { m_eGLFormat = GL_RGB;   this->YCbCrToRGB(); }
	if (m_uiChannels == 4 ) { m_eGLFormat = GL_RGBA; }

	m_eCompression = CT_JPEG;
}

CImageJPEG::~CImageJPEG() 
{
}