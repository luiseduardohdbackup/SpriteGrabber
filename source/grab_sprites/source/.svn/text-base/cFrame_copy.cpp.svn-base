#include <string.h>

#include "cBitmap.h"
#include "cFrame.h"


//---------------------------------------------------------
//	Copy
//---------------------------------------------------------

void
cFrame::Copy()
{
	cBitmap*	pBitmap;
	FIBITMAP*	pFIB;

	long		SourceWidth;
	long		SourceHeight;
	long		SourcePitch;
	BYTE*		pSource;

	long		DestPitch;
	BYTE*		pDest;

	long		i;

	// check frame is non-empty -----------------

	if (m_Width == 0)
	{
		return;
	}

	// get source and dest ----------------------

	pBitmap			=	GetBitmap();
	if (pBitmap == 0)
	{
		return;
	}
	pFIB			=	pBitmap->GetFIB();
	if (pFIB == 0)
	{
		return;
	}
	SourceWidth		=	pBitmap->GetWidth();
	SourceHeight	=	pBitmap->GetHeight();
	SourcePitch		=	FreeImage_GetPitch(pFIB);
	pSource			=	FreeImage_GetBits(pFIB);
	pSource			+=	m_SourceX * 4;
	pSource			+=	(SourceHeight - m_SourceY) * SourcePitch;

	pFIB			=	cBitmap::GetDestFIB();
	if (pFIB == 0)
	{
		return;
	}
	DestPitch		=	FreeImage_GetPitch(pFIB);
	pDest			=	FreeImage_GetBits(pFIB);
	pDest			+=	m_DestX * 4;
	pDest			+=	(s_PackHeight - m_DestY) * DestPitch;

	// check frame is in bounds -----------------

	if ((m_SourceX + m_Width) > SourceWidth || (m_SourceY + m_Height) > SourceHeight)
	{
		return;
	}
	if ((m_DestX + m_Width) > s_PackWidth || (m_DestY + m_Height) > s_PackHeight)
	{
		return;
	}

	// copy data --------------------------------

	for (i=0; i<m_Height; i++)
	{
		pSource	-=	SourcePitch;
		pDest	-=	DestPitch;
		memcpy(pDest,pSource,m_Width*4);
	}
}

//---------------------------------------------------------
//	Copy_All
//---------------------------------------------------------

void
cFrame::Copy_All(char* psBitmap)
{
	cBitmap*	pBitmap;
	long		iBegin;
	long		i;

	cBitmap::InitDestBitmap(s_PackWidth,s_PackHeight);

	pBitmap			=	0;
	iBegin			=	0;
	while (iBegin < s_nFrames)
	{
		pBitmap		=	Frame[iBegin].GetBitmap();
		pBitmap->Open();

		i			=	iBegin;
		while ((i < s_nFrames) && (Frame[i].GetBitmap() == pBitmap))
		{
			Frame[i].Copy();
			i++;
		}

		iBegin		=	i;
	}

	cBitmap::SaveDestBitmap(psBitmap);
}