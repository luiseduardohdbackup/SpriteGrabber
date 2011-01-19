#pragma once

#include "base.h"
#include "includes.h"

#define PACK_MAX			4



#define MODE_NONE			0
#define MODE_SEQ			1
#define MODE_BOX			2
#define MODE_PAS			3
#define MODE_BOX_N			6
#define MODE_PAS_N			7

#define STF_CENTER_IS_SET	1

//---------------------------------------------------------
//	Struct
//---------------------------------------------------------

struct sTable
{
	WORD	m_iBitmap;
	WORD	m_XPos;
	WORD	m_YPos;
	WORD	m_Width;
	WORD	m_Height;
	short	m_CenterX;
	short	m_CenterY;
	WORD	m_Flags;
};


//---------------------------------------------------------
//	Class
//---------------------------------------------------------

class cFrame
{
public:

					cFrame::cFrame();
					cFrame::~cFrame();

			bool	PlaceBox(long iPrev);
			bool	ShrinkWrap();

			long	GetWidth();
			long	GetHeight();
		cBitmap*	GetBitmap();

			void	SetPos(long XPos,long YPos);	// position on destination bitmap


	static	bool	PlaceBoxAndShrinkWrap_All();

	static	bool	PlaceBox_All();
	static	bool	ShrinkWrap_All();


	static	void	Free_All();
	static	long	Add(long Mode,long iBitmap,long BoxX,long BoxY,long BoxW,long BoxH,long ShrinkWrap,long BoxDir=0);
	static	long	GetPixelsUsed();
	static	long	GetFramesUsed();

	static	void	SaveTable(char* psFile);

	static	bool	Pack_All(long Border);

			void	Copy();
	static	void	Copy_All(char* psBitmap);	// saves bitmap

	static	void	Sort(long Method=0);
	static	cFrame*	GetFirst();

private:

	static	bool	Pack_TrySize(long Pixels,long Method);

			long	m_Mode;
			long	m_iBitmap;

			long	m_SourceX;
			long	m_SourceY;
			long	m_DestX;
			long	m_DestY;
			long	m_Width;
			long	m_Height;
			long	m_CenterX;
			long	m_CenterY;

			long	m_BoxDir;			// 0 = horizontal | 1 = vertical
			long	m_BoxWidth;
			long	m_BoxHeight;
			long	m_ShrinkWrap;		// shrink wrap

//			bool	m_bPacked;			// has been added to packer
			bool	m_bSorted;

	static	long	s_nFrames;

	static	long	s_PackWidth;
	static	long	s_PackHeight;
	static	long	s_PackBorder;





//	only need a one way link

			cFrame*	m_pNext;
	static	cFrame*	s_pFirst;
};

extern	cFrame	Frame[FRAME_MAX];

