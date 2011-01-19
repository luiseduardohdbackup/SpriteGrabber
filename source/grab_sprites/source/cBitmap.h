#pragma once

#include <FreeImage.h>

#include "file.h"
#include "base.h"
#include "includes.h"

#define FIB						FIBITMAP

//---------------------------------------------------------
//	Class
//---------------------------------------------------------

class cBitmap
{
public:

					cBitmap::cBitmap();
					cBitmap::~cBitmap();

			bool	Open();
			void	Free();

			bool	CalcBitsAndPitch();

			long	GetWidth();
			long	GetHeight();
			FIB*	GetFIB();

			bool	IsClearH(long PixelX,long PixelY,long Width);
			bool	IsClearV(long PixelX,long PixelY,long Height);

			void	DoAlphaCutOff();
			void	DoConvertARGB();
			void	DoUnMultAlpha();
			void	DoUnMultAlpha_Component(BYTE* pC,BYTE Alpha);


	static	bool	Open_All();
	static	void	Free_All();

	static	long	Add(char* psFile,BYTE AlphaCutOff,DWORD ARGB_From,DWORD ARGB_To,long Shrink);




	static	void	InitDestBitmap(long Width,long Height);
	static	void	SaveDestBitmap(char* psFile);
	static	FIB*	GetDestFIB();


	static	QWORD	GetNewestFile();
	static	void	SetNewestFile(QWORD qwTime);


private:

			char	m_cFile[FILE_PATH_MAX];

			FIB*	m_pFIB;
			BYTE*	m_pBits;
			long	m_Pitch;
			long	m_ShiftA;
			long	m_ShiftR;
			long	m_ShiftG;
			long	m_ShiftB;
			DWORD	m_MaskA;
			DWORD	m_MaskR;
			DWORD	m_MaskG;
			DWORD	m_MaskB;

			long	m_Width;
			long	m_Height;

			// effects to apply on bitmap

			BYTE	m_AlphaCutOff;	//
			DWORD	m_ARGB_From;
			DWORD	m_ARGB_To;
			long	m_Shrink;		// 1 | 2 | 4 | 8 (max)


	static	long	s_nBitmaps;
	static	FIB*	s_pDestFIB;

	static	cBitmap*s_pActive;

	static	QWORD	s_qwNewestFile;
};



extern	cBitmap	Bitmap[BITMAP_MAX];


struct cSavedBitmap
{
	char	m_cFile[FILE_PATH_MAX];
	int		m_Width;
	int		m_Height;
	
	static	long	s_nSavedBitmaps;
};

extern cSavedBitmap SavedBitmap[SAVED_BITMAP_MAX];