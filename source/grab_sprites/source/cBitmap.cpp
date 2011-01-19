#include <FreeImage.h>

#include "string_s.h"
#include "endian.h"

#include "cBitmap.h"





cBitmap	Bitmap[BITMAP_MAX];

long		cBitmap::s_nBitmaps			=	0;
FIB*		cBitmap::s_pDestFIB			=	0;

cBitmap*	cBitmap::s_pActive			=	0;

QWORD		cBitmap::s_qwNewestFile		=	0;

//---------------------------------------------------------
//	Constructor / Destructor
//---------------------------------------------------------

cBitmap::cBitmap()
{
	m_pFIB		=	0;
	m_Shrink	=	0;
}


cBitmap::~cBitmap()
{
	Free();
}


//---------------------------------------------------------
//	Open
//---------------------------------------------------------

bool
cBitmap::Open()
{
	FREE_IMAGE_FORMAT	FIF;
	long				BPP;
	FIBITMAP*			pFIB;


	// allow one open bitmap --------------------

#ifdef BITMAP_ONLY_ONE_OPEN
	if (s_pActive && s_pActive != this)
	{
		s_pActive->Free();
		s_pActive		=	0;
	}
#endif

	// check if file already open ---------------

	if (m_pFIB)
	{
		return true;
	}

	// check file and open ----------------------

 	if (m_cFile[0] == 0)
	{
		printf("ERROR   : NO BITMAP\n");
		return false;
	}
	FIF					=	FreeImage_GetFIFFromFilename(m_cFile);
	m_pFIB				=	FreeImage_Load(FIF,m_cFile,0);
	if (m_pFIB == 0)
	{
		printf("ERROR   : BAD BITMAP [%s]\n",m_cFile);
		return false;
	}

	// get BPP and metrics ----------------------

	BPP					=	FreeImage_GetBPP(m_pFIB);
	m_Width				=	FreeImage_GetWidth(m_pFIB);
	m_Height			=	FreeImage_GetHeight(m_pFIB);

	// convert to 32bit -------------------------

	if (BPP == 8)
	{
		FreeImage_SetTransparentIndex(m_pFIB,0);

		pFIB			=	FreeImage_ConvertTo32Bits(m_pFIB);
		Free();
		m_pFIB			=	pFIB;
	}
	else if (BPP != 32)
	{
		Free();
		printf("ERROR   : INVALID BPP [%s]\n",m_cFile);
		return false;
	}
	
	// un pre-multiply PSD ----------------------
	
	if (FreeImage_GetFIFFromFilename(m_cFile) == FIF_PSD)
	{
		if (!CalcBitsAndPitch())
		{
			return false;
		}
		DoUnMultAlpha();
	}

	// set alpha for specified RGB values -------

	if (m_ARGB_From != m_ARGB_To)
	{
		if (!CalcBitsAndPitch())
		{
			return false;
		}
		DoConvertARGB();
	}

	// alpha cut off ----------------------------

	if (m_AlphaCutOff)
	{
		if (!CalcBitsAndPitch())
		{
			return false;
		}
		DoAlphaCutOff();
	}

	// shrink -----------------------------------

	if (m_Shrink > 1)
	{
		m_Width			/=	m_Shrink;
		m_Height		/=	m_Shrink;

		pFIB			=	FreeImage_Rescale(m_pFIB,m_Width,m_Height,FILTER_BOX);
		Free();
		m_pFIB			=	pFIB;
	}

	// get bits and pitch -----------------------

	if (!CalcBitsAndPitch())
	{
		return false;
	}

	s_pActive			=	this;
	return true;
}

//---------------------------------------------------------
//	Free
//---------------------------------------------------------

void
cBitmap::Free()
{
	if (m_pFIB)
	{
		FreeImage_Unload(m_pFIB);
		m_pFIB		=	0;
		m_pBits		=	0;
		m_Pitch		=	0;
	}

	if (s_pActive == this)
	{
		s_pActive	=	0;
	}
}




//---------------------------------------------------------
//	CalcBitsAndPitch
//---------------------------------------------------------

long
LowestBit(DWORD Value)
{
	long	Shift;
	DWORD	Mask;
	
	if (Value == 0)
	{
		return 32;
	}
	
	Shift				=	0;
	Mask				=	1;
	
	while ((Value & Mask) == 0)
	{
		Shift			+=	1;
		Mask			<<=	1;
	}
	
	return Shift;
}	

bool
cBitmap::CalcBitsAndPitch()
{
	m_pBits				=	FreeImage_GetBits(m_pFIB);
	m_Pitch				=	FreeImage_GetPitch(m_pFIB);

	m_MaskR				=	FreeImage_GetRedMask(m_pFIB);
	m_MaskG				=	FreeImage_GetGreenMask(m_pFIB);
	m_MaskB				=	FreeImage_GetBlueMask(m_pFIB);
	m_MaskA				=	~(m_MaskR | m_MaskG | m_MaskB);

	if (m_MaskA == 0)
	{
		Free();
		printf("ERROR   : BITMAP HAS NO ALPHA CHANNEL\n");
		return false;
	}
	
	m_ShiftR			=	LowestBit(m_MaskR);
	m_ShiftG			=	LowestBit(m_MaskG);
	m_ShiftB			=	LowestBit(m_MaskB);
	m_ShiftA			=	LowestBit(m_MaskA);

	return true;
}











//---------------------------------------------------------
//	GetWidth
//---------------------------------------------------------

long
cBitmap::GetWidth()
{
	return m_Width;
}

//---------------------------------------------------------
//	GetHeight
//---------------------------------------------------------

long
cBitmap::GetHeight()
{
	return m_Height;
}

//---------------------------------------------------------
//	GetFIB
//---------------------------------------------------------

FIBITMAP*
cBitmap::GetFIB()
{
	return m_pFIB;
}




//---------------------------------------------------------
//	IsClearH
//---------------------------------------------------------

bool
cBitmap::IsClearH(long PixelX,long PixelY,long Width)
{
	BYTE*	pAlpha;
	long	i;

	pAlpha		=	m_pBits;
	pAlpha		+=	PixelX * 4;
	pAlpha		+=	((m_Height - 1) - PixelY) * m_Pitch;
	pAlpha		+=	m_ShiftA / 8;

	for (i=0; i<Width; i++)
	{
		if (*pAlpha > 0)
		{
			return false;
		}
		pAlpha	+=	4;
	}

	return true;
}

//---------------------------------------------------------
//	IsClearV
//---------------------------------------------------------

bool
cBitmap::IsClearV(long PixelX,long PixelY,long Height)
{
	BYTE*	pAlpha;
	long	i;

	pAlpha		=	m_pBits;
	pAlpha		+=	PixelX * 4;
	pAlpha		+=	(m_Height - PixelY) * m_Pitch;
	pAlpha		+=	m_ShiftA / 8;

	for (i=0; i<Height; i++)
	{
		pAlpha	-=	m_Pitch;
		if (*pAlpha > 0)
		{
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------
//	DoAlphaCutOff
//---------------------------------------------------------

void
cBitmap::DoAlphaCutOff()
{
	BYTE*	pAlpha;
	long	x,y;

	pAlpha		=	m_pBits;
	pAlpha		+=	m_ShiftA / 8;

	for (y=0; y<m_Height; y++)
	{
		for (x=0; x<m_Width; x++)
		{
			if (*pAlpha <= m_AlphaCutOff)
			{
				*pAlpha	=	0;
			}
			pAlpha		+=	4;
		}
		pAlpha			-=	4 * m_Width;
		pAlpha			+=	m_Pitch;
	}
}

//---------------------------------------------------------
//	DoConvertARGB
//---------------------------------------------------------

void
cBitmap::DoConvertARGB()
{
	DWORD*	pARGB;
	DWORD	From;
	DWORD	To;

	long	x,y;

	From			=	((m_ARGB_From & 0xFF000000) >> 24) << m_ShiftA;
	From			+=	((m_ARGB_From & 0x00FF0000) >> 16) << m_ShiftR;
	From			+=	((m_ARGB_From & 0x0000FF00) >>  8) << m_ShiftG;
	From			+=	((m_ARGB_From & 0x000000FF) >>  0) << m_ShiftB;

	To				=	((m_ARGB_To & 0xFF000000) >> 24) << m_ShiftA;
	To				+=	((m_ARGB_To & 0x00FF0000) >> 16) << m_ShiftR;
	To				+=	((m_ARGB_To & 0x0000FF00) >>  8) << m_ShiftG;
	To				+=	((m_ARGB_To & 0x000000FF) >>  0) << m_ShiftB;

	pARGB			=	(DWORD*)m_pBits;

	for (y=0; y<m_Height; y++)
	{
		for (x=0; x<m_Width; x++)
		{
			if (*pARGB == From)
			{
				*pARGB	=	To;
			}
			pARGB++;
		}
		pARGB			-=	m_Width;
		pARGB			+=	m_Pitch / 4;
	}
}




//---------------------------------------------------------
//	DoUnMultAlpha
//---------------------------------------------------------

void
cBitmap::DoUnMultAlpha()
{
	BYTE*	pAlpha;
	BYTE	Alpha;
	long	x,y;


	pAlpha		=	m_pBits;
	pAlpha		+=	m_ShiftA / 8;
	
	for (y=0; y<m_Height; y++)
	{
		for (x=0; x<m_Width; x++,pAlpha+=4)
		{
			if (*pAlpha == 0 || *pAlpha == 255)
			{
				continue;
			}
			Alpha	=	*pAlpha;
			if (m_ShiftA)
			{
				DoUnMultAlpha_Component(pAlpha-1,Alpha);
				DoUnMultAlpha_Component(pAlpha-2,Alpha);
				DoUnMultAlpha_Component(pAlpha-3,Alpha);
			}
			else
			{
				DoUnMultAlpha_Component(pAlpha+1,Alpha);
				DoUnMultAlpha_Component(pAlpha+2,Alpha);
				DoUnMultAlpha_Component(pAlpha+3,Alpha);
			}
		}
		pAlpha			-=	4 * m_Width;
		pAlpha			+=	m_Pitch;
	}
}

//---------------------------------------------------------
//	DoUnMultAlpha_Component
//---------------------------------------------------------

void
cBitmap::DoUnMultAlpha_Component(BYTE* pC,BYTE Alpha)
{
	float	fAlpha;
	float	fCm;
	float	fCu;

	fAlpha		=	(float)Alpha / 255.0f;
	
	fCm			=	(float)*pC;
	
	fCu			=	fCm - (255.0f * (1.0f - fAlpha));
	fCu			/=	fAlpha;
//	fCu			+=	0.5f;
	
	if (fCu > 255.0f)
	{
		fCu		=	255.0f;
	}

	*pC			=	(BYTE)fCu;
}





//---------------------------------------------------------
//	Free_All
//---------------------------------------------------------

void
cBitmap::Free_All()
{
	long	i;

	for (i=0; i<s_nBitmaps; i++)
	{
		Bitmap[i].Free();
	}

	s_nBitmaps		=	0;
}

//---------------------------------------------------------
//	Add
//---------------------------------------------------------

//	in:		filename of bitmap
//	out:	index of bitmap
//			returns -1 if out of buffer space

long
cBitmap::Add(char* psFile,BYTE AlphaCutOff,DWORD ARGB_From,DWORD ARGB_To,long Shrink)
{
	long	i;
	QWORD	qw;

	// compare dates here -----------------------

	qw					=	FILE_time_last_write(psFile);
	if (qw > s_qwNewestFile)
	{
		s_qwNewestFile	=	qw;
	}

	// check if file is already referenced ------

	for (i=0; i<s_nBitmaps; i++)
	{
		if (strcmp(Bitmap[i].m_cFile,psFile) == 0)
		{
			if (Bitmap[i].m_AlphaCutOff == AlphaCutOff &&
				Bitmap[i].m_Shrink == Shrink)
			{
				return i;
			}
		}
	}

	// check out of buffer space ----------------

	s_nBitmaps++;
	if (s_nBitmaps > BITMAP_MAX)
	{
		return -1;
	}

	// success ----------------------------------

	strcpy_s(Bitmap[i].m_cFile,psFile);

	Bitmap[i].m_AlphaCutOff	=	AlphaCutOff;
	Bitmap[i].m_ARGB_From	=	ARGB_From;
	Bitmap[i].m_ARGB_To		=	ARGB_To;

	if (Shrink > 0 && Shrink <= 8)
	{
		Bitmap[i].m_Shrink	=	Shrink;
	}

	return i;
}










//---------------------------------------------------------
//	InitDestBitmap
//---------------------------------------------------------

void
cBitmap::InitDestBitmap(long Width,long Height)
{
	if (s_pDestFIB)
	{
		FreeImage_Unload(s_pDestFIB);
	}

	s_pDestFIB			=	FreeImage_Allocate(Width,Height,32,0xFF0000,0x00FF00,0x0000FF);

	if (0)
	{
		DWORD*	pDest;
		long	i;

		pDest			=	(DWORD*)FreeImage_GetBits(s_pDestFIB);
		i				=	Width * Height;
		while (i--)
		{
			*pDest++	=	0xFF00FF00;
		}
	}
}

//---------------------------------------------------------
//	SaveDestBitmap
//---------------------------------------------------------

void
cBitmap::SaveDestBitmap(char* psFile)
{
	FREE_IMAGE_FORMAT	Format;

	DWORD*	pBits;
	long	Width;
	long	Height;
	long	i;



	pBits			=	(DWORD*)FreeImage_GetBits(s_pDestFIB);
	Width			=	FreeImage_GetWidth(s_pDestFIB);
	Height			=	FreeImage_GetHeight(s_pDestFIB);
	i				=	Width * Height;

	while (i--)
	{
		if ((*pBits & 0xFF000000) == 0)
		{
			*pBits	=	0;
		}
		pBits++;
	}

	Format			=	FreeImage_GetFIFFromFilename(psFile);
	FreeImage_Save(Format,s_pDestFIB,psFile);
	FreeImage_Unload(s_pDestFIB);

	s_pDestFIB	=	0;
	
	// update the saved bitmaps - no asserts here? WTF?
	if (cSavedBitmap::s_nSavedBitmaps < SAVED_BITMAP_MAX)
	{
		strcpy_s(SavedBitmap[cSavedBitmap::s_nSavedBitmaps].m_cFile,psFile);
		SavedBitmap[cSavedBitmap::s_nSavedBitmaps].m_Width = Width;
		SavedBitmap[cSavedBitmap::s_nSavedBitmaps].m_Height= Height;
		++cSavedBitmap::s_nSavedBitmaps;
	}
}

//---------------------------------------------------------
//	GetDestFIB
//---------------------------------------------------------

FIBITMAP*
cBitmap::GetDestFIB()
{
	return s_pDestFIB;
}















//---------------------------------------------------------
//	SetNewestFile
//---------------------------------------------------------


void
cBitmap::SetNewestFile(QWORD qwTime)
{
	s_qwNewestFile		=	qwTime;
}

//---------------------------------------------------------
//	GetNewestFile
//---------------------------------------------------------

QWORD
cBitmap::GetNewestFile()
{
	return s_qwNewestFile;
}


cSavedBitmap SavedBitmap[SAVED_BITMAP_MAX];

long		cSavedBitmap::s_nSavedBitmaps			=	0;
