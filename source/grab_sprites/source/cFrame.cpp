#include "endian.h"

#include "cBitmap.h"
#include "cFrame.h"





cFrame	Frame[FRAME_MAX];
sTable	Table[FRAME_MAX];



long	cFrame::s_nFrames		=	0;

cFrame*	cFrame::s_pFirst		=	0;

//---------------------------------------------------------
//	Constructor / Destructor
//---------------------------------------------------------

cFrame::cFrame()
{
	m_iBitmap	=	0;
}

cFrame::~cFrame()
{
}





//---------------------------------------------------------
//	GetWidth
//---------------------------------------------------------

long
cFrame::GetWidth()
{
	return m_Width;
}

//---------------------------------------------------------
//	GetHeight
//---------------------------------------------------------

long
cFrame::GetHeight()
{
	return m_Height;
}

//---------------------------------------------------------
//	GetBitmap
//---------------------------------------------------------

cBitmap*
cFrame::GetBitmap()
{
	return &Bitmap[m_iBitmap];
}







//---------------------------------------------------------
//	SetPos
//---------------------------------------------------------

void
cFrame::SetPos(long XPos,long YPos)
{
	m_DestX		=	XPos;
	m_DestY		=	YPos;
}






//---------------------------------------------------------
//	PlaceBox
//---------------------------------------------------------

bool
cFrame::PlaceBox(long iPrev)
{
	long	BitmapWidth;
	long	BitmapHeight;


	BitmapWidth			=	Bitmap[m_iBitmap].GetWidth();
	BitmapHeight		=	Bitmap[m_iBitmap].GetHeight();

	if (m_Mode == MODE_SEQ)
	{
		m_SourceX		=	0;
		m_SourceY		=	0;
		m_Width			=	BitmapWidth;
		m_Height		=	BitmapHeight;
		return true;
	}

	if (m_Mode != MODE_BOX_N && m_Mode != MODE_PAS_N)
	{
		return true;
	}

	if ((m_Mode & 3) != (Frame[iPrev].m_Mode & 3))
	{
		printf("ERROR   : INTERNAL DATA ERROR\n");
		return false;
	}


	if (m_BoxDir == 0)
	{
		// horizontal ---------------------------

		m_SourceX		=	Frame[iPrev].m_SourceX + m_Width;
		m_SourceY		=	Frame[iPrev].m_SourceY;
		if ((m_SourceX + m_Width) > BitmapWidth)
		{
			m_SourceX	=	0;
			m_SourceY	=	Frame[iPrev].m_SourceY + m_Height;
			if ((m_SourceY + m_Height) > BitmapHeight)
			{
				printf("ERROR   : TOO MANY FRAMES ON BITMAP\n");
				return false;
			}
		}
	}
	else
	{
		// vertival -----------------------------

		m_SourceY		=	Frame[iPrev].m_SourceY + m_Height;
		m_SourceX		=	Frame[iPrev].m_SourceX;
		if ((m_SourceY + m_Height) > BitmapHeight)
		{
			m_SourceY	=	0;
			m_SourceX	=	Frame[iPrev].m_SourceX + m_Width;
			if ((m_SourceX + m_Width) > BitmapWidth)
			{
				printf("ERROR   : TOO MANY FRAMES ON BITMAP\n");
				return false;
			}
		}
	}

	return true;
}

//---------------------------------------------------------
//	ShrinkWrap
//---------------------------------------------------------

//	1.	down
//	2.	up
//	3.	right
//	4.	left

bool
cFrame::ShrinkWrap()
{
	long	BoxCenterX;
	long	BoxCenterY;

	FIBITMAP*	pFIB;

	if (m_ShrinkWrap == 0)
	{
		m_CenterX		=	m_Width  >> 1;
		m_CenterY		=	m_Height >> 1;

		return true;
	}

	pFIB				=	Bitmap[m_iBitmap].GetFIB();

	if (m_Mode == MODE_SEQ)
	{
		m_Width			=	Bitmap[m_iBitmap].GetWidth();
		m_Height		=	Bitmap[m_iBitmap].GetHeight();
	}

	//-------------------------------------------

	BoxCenterX			=	m_SourceX + (m_Width / 2);
	BoxCenterY			=	m_SourceY + (m_Height / 2);

	//-------------------------------------------

	while (Bitmap[m_iBitmap].IsClearH(m_SourceX,m_SourceY,m_Width) && m_Height)
	{
		m_SourceY++;
		m_Height--;
	}

	if (m_Height == 0)
	{
		m_Width			=	0;
		m_Height		=	0;
		m_SourceX		=	0;
		m_SourceY		=	0;
		return true;
	}

	while (Bitmap[m_iBitmap].IsClearH(m_SourceX,m_SourceY+(m_Height-1),m_Width))
	{
		m_Height--;
	}

	while (Bitmap[m_iBitmap].IsClearV(m_SourceX,m_SourceY,m_Height))
	{
		m_SourceX++;
		m_Width--;
	}

	while (Bitmap[m_iBitmap].IsClearV(m_SourceX+(m_Width-1),m_SourceY,m_Height))
	{
		m_Width--;
	}

	//-------------------------------------------

	m_CenterX			=	BoxCenterX - m_SourceX;
	m_CenterY			=	BoxCenterY - m_SourceY;

	return true;
}








//---------------------------------------------------------
//	PlaceBox_All
//---------------------------------------------------------
/*
bool
cFrame::PlaceBox_All()
{
	long	i;

	for (i=0; i<s_nFrames; i++)
	{
		if (false == Frame[i].PlaceBox(i-1))
		{
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------
//	ShrinkWrap_All
//---------------------------------------------------------

bool
cFrame::ShrinkWrap_All()
{
	long	i;

	for (i=0; i<s_nFrames; i++)
	{
		if (Frame[i].m_ShrinkWrap)
		{
			if (false == Frame[i].ShrinkWrap())
			{
				return false;
			}
		}
	}

	return true;
}

*/

//---------------------------------------------------------
//	PlaceBoxAndShrinkWrap_All
//---------------------------------------------------------

bool
cFrame::PlaceBoxAndShrinkWrap_All()
{
	cBitmap*	pBitmap;
	long		iBegin;
	long		i;

	pBitmap			=	0;
	iBegin			=	0;
	while (iBegin < s_nFrames)
	{
		pBitmap		=	Frame[iBegin].GetBitmap();
		pBitmap->Open();

		i			=	iBegin;
		while ((i < s_nFrames) && (Frame[i].GetBitmap() == pBitmap))
		{
			if (false == Frame[i].PlaceBox(i-1))
			{
				return false;
			}
			i++;
		}

		i			=	iBegin;
		while ((i < s_nFrames) && (Frame[i].GetBitmap() == pBitmap))
		{
			if (false == Frame[i].ShrinkWrap())
			{
				return false;
			}
			i++;
		}

 		iBegin		=	i;
	}

	return true;
}







//---------------------------------------------------------
//	Free_All
//---------------------------------------------------------

void
cFrame::Free_All()
{
	s_nFrames	=	0;
	s_pFirst	=	0;
}




//---------------------------------------------------------
//	Add
//---------------------------------------------------------

//	in:		Mode		mode
//			iBitmap		index of source bitmap
//			BoxX		x pos
//			BoxY		y pos
//			BoxW		width
//			BoxH		height
//			Border		border (0 or 1) -- ? should border be property of texture?
//			ShrinkWrap	shrink wrap

//	out:	index of frame
//			returns -1 if out of buffer space

long
cFrame::Add(long Mode,long iBitmap,long BoxX,long BoxY,long BoxW,long BoxH,long ShrinkWrap,long BoxDir)
{
	long	i;

	// check out of buffer space ----------------

	i	=	s_nFrames++;
	if (s_nFrames > FRAME_MAX)
	{
		return -1; 
	}

	// found ------------------------------------

	Frame[i].m_Mode			=	Mode;
	Frame[i].m_iBitmap		=	iBitmap;
	Frame[i].m_SourceX		=	BoxX;
	Frame[i].m_SourceY		=	BoxY;
	Frame[i].m_Width		=	BoxW;
	Frame[i].m_Height		=	BoxH;
	Frame[i].m_CenterX		=	0;
	Frame[i].m_CenterY		=	0;

	Frame[i].m_BoxDir		=	BoxDir;
	Frame[i].m_BoxWidth		=	BoxW;
	Frame[i].m_BoxHeight	=	BoxH;
	Frame[i].m_ShrinkWrap	=	ShrinkWrap;

	// success ----------------------------------

	return i;
}


//---------------------------------------------------------
//	GetPixelsUsed
//---------------------------------------------------------

long
cFrame::GetPixelsUsed()
{
	long	i;
	long	Pixels;

	Pixels		=	0;
	for (i=0; i<s_nFrames; i++)
	{
		Pixels	+=	Frame[i].m_Width * Frame[i].m_Height;
	}

	return Pixels;
}

//---------------------------------------------------------
//	GetFramesUsed
//---------------------------------------------------------

long
cFrame::GetFramesUsed()
{
	return s_nFrames;
}

//---------------------------------------------------------
//	SaveTable
//---------------------------------------------------------

void
cFrame::SaveTable(char* psFile)
{
	long	i;
	WORD	Flags;

	for (i=0; i<s_nFrames; i++)
	{
		Table[i].m_iBitmap		=	0;
		Table[i].m_XPos			=	(WORD)Frame[i].m_DestX;
		Table[i].m_YPos			=	(WORD)Frame[i].m_DestY;
		Table[i].m_Height		=	(WORD)Frame[i].m_Height;
		Table[i].m_Width		=	(WORD)Frame[i].m_Width;
		Table[i].m_CenterX		=	(short)Frame[i].m_CenterX;
		Table[i].m_CenterY		=	(short)Frame[i].m_CenterY;

		//printf("Outputting data for: %i %i, %i %i, %i %i, %i %i %s\n", Frame[i].m_DestX, Frame[i].m_DestY, Frame[i].m_SourceX, Frame[i].m_SourceY, Frame[i].m_Height, Frame[i].m_Width, Frame[i].m_CenterX, Frame[i].m_CenterY, Bitmap[Frame[i].m_iBitmap].m_cFile);
		
		Flags					=	0;
		if (Frame[i].m_ShrinkWrap)
		{
			Flags				|=	STF_CENTER_IS_SET;
		}
		Table[i].m_Flags		=	Flags;		
	}

	ENDIAN_WORD_ARRAY((WORD*)Table,8*s_nFrames);

	FILE_save(psFile,&Table,sizeof(sTable)*s_nFrames);
}



void
cFrame::Sort(long Method)
{
	cFrame*	pPrev;
	cFrame*	pBest;

	long long	Best;
	long long	Size;
	long long	w,h,i;

	for (i=0; i<s_nFrames; i++)
	{
		Frame[i].m_bSorted	=	false;
	}

	pPrev					=	0;
	do
	{
		pBest				=	0;
		Best				=	-1;
		for (i=0; i<s_nFrames; i++)
		{
			if (Frame[i].m_bSorted)
			{
				continue;		
			}

			w				=	Frame[i].m_Width;
			h				=	Frame[i].m_Height;
			switch (Method)
			{
			default:
			case 0:
				Size		=	w * h;
				break;
			case 1:
				Size		=	(w*w) * (h*h);
				break;
			case 2:
				Size		=	w * 1;
				break;
			case 3:
				Size		=	h * 1;
				break;
			}

			if (Size <= Best)
			{
				continue;
			}

			Best			=	Size;
			pBest			=	&Frame[i];
		}
		if (pBest == 0)
		{
			return;
		}
		if (pPrev == 0)
		{
			s_pFirst		=	pBest;
		}
		else
		{
			pPrev->m_pNext	=	pBest;
		}
		pPrev				=	pBest;
		pBest->m_pNext		=	0;
		pBest->m_bSorted	=	true;
	}
	while (pBest);
}

cFrame*
cFrame::GetFirst()
{
	return s_pFirst;
}

