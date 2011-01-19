#include "cText.h"
#include "spr.h"



sFont	cText::s_Font[FONTS_MAX];
long	cText::s_iFont				=	0;













//---------------------------------------------------------
//	Constructor
//---------------------------------------------------------

cText::cText()
{
	m_pString	=	0;
	m_XPos		=	0;
	m_YPos		=	0;
	m_Align		=	LEFT;
	m_iFont		=	0;
	m_ARGB		=	0xFFFFFFFF;
}

cText::cText(char* pString,long PixelX,long PixelY,eAlign Align,BYTE iFont,DWORD ARGB)
{
	m_pString	=	pString;
	m_XPos		=	PixelX << 16;
	m_YPos		=	PixelY << 16;
	m_Align		=	Align;
	m_iFont		=	iFont;
	m_ARGB		=	ARGB;
}










//---------------------------------------------------------
//	Access Functions
//---------------------------------------------------------

void
cText::SetString(char* pString)
{
	m_pString	=	pString;
}

void
cText::SetPixelX(long PixelX)
{
	m_XPos		=	PixelX << 16;
}

void
cText::SetPixelY(long PixelY)
{
	m_YPos		=	PixelY << 16;
}

void
cText::SetAlign(eAlign Align)
{
	m_Align		=	Align;
}

void
cText::SetFont(BYTE iFont)
{
	m_iFont		=	iFont;
}

void
cText::SetARGB(DWORD ARGB)
{
	m_ARGB		=	ARGB;
}

void
cText::SetXPos(long XPos)
{
	m_XPos		=	XPos;
}

void
cText::SetYPos(long YPos)
{
	m_YPos		=	YPos;
}





//---------------------------------------------------------
//	GetWidth
//---------------------------------------------------------

long
cText::GetWidth()
{
	sSprT*	pST;
	char*	ps;
	char	c;
	long	i;
	long	Width;
	long	Widest;

	if (m_iFont >= s_iFont || m_pString == 0)
	{
		return 0;
	}

	Widest				=	0;

	i					=	s_Font[m_iFont].m_iSprite;
	pST					=	&SPR_pTable[i];

	ps					=	m_pString;
	c					=	*ps;
	while (c)
	{
		Width			=	0 - s_Font[m_iFont].m_Kerning;
		c				=	*ps++;
		while (c != 0 && c != 10)
		{
			c			-=	32;
			if (c <= 0)
			{
				Width	+=	s_Font[m_iFont].m_SpaceWidth;
			}
			else
			{
				Width	+=	pST[c].m_Width;
			}
			Width		+=	s_Font[m_iFont].m_Kerning;
			c			=	*ps++;
		}
		if (Width > Widest)
		{
			Widest		=	Width;
		}
	}

	return Widest;
}


//---------------------------------------------------------
//	GetWidthOfLine
//---------------------------------------------------------

long
cText::GetWidthOfLine(char* psLine)
{
	sSprT*	pST;
	char*	ps;
	char	c;
	long	i;
	long	Width;

	if (m_iFont >= s_iFont || m_pString == 0)
	{
		return 0;
	}

	Width				=	0;

	i					=	s_Font[m_iFont].m_iSprite;
	pST					=	&SPR_pTable[i];

	ps					=	psLine;
	c					=	*ps++;
	while (c != 0 && c != 10)
	{
		c				-=	32;
		if (c <= 0)
		{
			Width		+=	s_Font[m_iFont].m_SpaceWidth;
		}
		else
		{
			Width		+=	pST[c].m_Width;
		}
		Width			+=	s_Font[m_iFont].m_Kerning;
		c				=	*ps++;
	}

	return Width;
}



//---------------------------------------------------------
//	Print
//---------------------------------------------------------

void
cText::Print()
{
	sSprT*	pST;
	char*	ps;
	char	c;
	long	i;
	long	w;
	long	x;

	long	LineWidth;
	long	XPos;
	long	YPos;

	if (m_iFont >= s_iFont || m_pString == 0)
	{
		return;
	}

	i					=	s_Font[m_iFont].m_iSprite;
	pST					=	&SPR_pTable[i];

	ps					=	m_pString;
	c					=	*ps++;

	YPos				=	m_YPos;

	while (c)
	{
		switch (m_Align)
		{
		case LEFT:
			XPos		=	m_XPos;
			break;
		case CENTER:
			LineWidth	=	GetWidthOfLine(ps-1);
			XPos		=	m_XPos - (LineWidth << 15);
			break;
		case RIGHT:
			LineWidth	=	GetWidthOfLine(ps-1);
			XPos		=	m_XPos - (LineWidth << 16);
			break;
		}

		while (c != 0 && c != 10)
		{
			i			=	c - 32;
			if (i <= 0)
			{
				w		=	s_Font[m_iFont].m_SpaceWidth;
			}
			else
			{
				w		=	pST[i].m_Width;
				x		=	pST[i].m_CenterX << 16;
				i		+=	s_Font[m_iFont].m_iSprite;

				Spr[i].Blit(XPos+x,YPos,m_ARGB);
			}

			c			=	*ps++;

			w			+=	s_Font[m_iFont].m_Kerning;
			XPos		+=	w << 16;
		}

		if (c == 10)
		{
			c			=	*ps++;
			YPos		+=	s_Font[m_iFont].m_LineHeight << 16;
		}
	}
}


//---------------------------------------------------------
//	RegisterFont
//---------------------------------------------------------

bool
cText::RegisterFont(long iSprite,long Kerning,long AdjustY,long SpaceWidth,long LineHeight)
{
	if (s_iFont >= FONTS_MAX)
	{
		return false;
	}

	s_Font[s_iFont].m_iSprite		=	iSprite;
	s_Font[s_iFont].m_Kerning		=	Kerning;
	s_Font[s_iFont].m_AdjustY		=	AdjustY;
	s_Font[s_iFont].m_SpaceWidth	=	SpaceWidth;
	s_Font[s_iFont].m_LineHeight	=	LineHeight;

	s_iFont++;

	return true;
}



