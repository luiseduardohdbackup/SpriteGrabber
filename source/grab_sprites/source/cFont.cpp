#include "cFont.h"
#include "spr.h"



//---------------------------------------------------------
//	Globals
//---------------------------------------------------------

cFont	Font[FONTS_MAX];














//---------------------------------------------------------
//	Constructor / Destructor
//---------------------------------------------------------

cFont::cFont()
{
	m_iSprite		=	-1;
	m_AdjustY		=	0;
	m_LineHeight	=	32;
	m_ARGB			=	0xFFFFFFFF;
}











//---------------------------------------------------------
//	Access Functions
//---------------------------------------------------------

void
cFont::SetFont(long iSprite,long SpaceWidth,long Kerning)
{
	m_iSprite		=	iSprite;
	m_SpaceWidth	=	SpaceWidth;
	m_Kerning		=	Kerning;
}

void
cFont::SetARGB(DWORD ARGB)
{
	m_ARGB			=	ARGB;
}

void
cFont::SetLineHeight(long LineHeight)
{
	m_LineHeight	=	LineHeight;
}

void
cFont::SetAdjustY(long AdjustY)
{
	m_AdjustY		=	AdjustY;
}

//---------------------------------------------------------
//	GetWidthOfLine
//---------------------------------------------------------

long
cFont::GetWidthOfLine(char* psText)
{
	sSprT*	pST;
	char*	ps;
	char	c;
	long	w;

	if (m_iSprite < 0 || psText == 0)
	{
		return 0;
	}

	pST					=	&SPR_pTable[m_iSprite];
	ps					=	psText;
	c					=	*ps++;
	w					=	0;

	while (c != 0 && c != 10)
	{
		c				-=	32;
		if (c <= 0)
		{
			w			+=	m_SpaceWidth;
		}
		else
		{
			w			+=	pST[c].m_Width;
		}
		w				+=	m_Kerning;
		c				=	*ps++;
	}

	return w;
}

//---------------------------------------------------------
//	GetWidthOfString
//---------------------------------------------------------

long
cFont::GetWidthOfString(char* psText)
{
	sSprT*	pST;
	char*	ps;
	char	c;
	long	w;
	long	widest;

	if (m_iSprite < 0 || psText == 0)
	{
		return 0;
	}

	pST					=	&SPR_pTable[m_iSprite];
	ps					=	psText;
	c					=	*ps;
	w					=	0;
	widest				=	0;

	while (c)
	{
		w				=	0 - m_Kerning;
		c				=	*ps++;
		while (c != 0 && c != 10)
		{
			c			-=	32;
			if (c <= 0)
			{
				w		+=	m_SpaceWidth;
			}
			else
			{
				w		+=	pST[c].m_Width;
			}
			w			+=	m_Kerning;
			c			=	*ps++;
		}
		if (w	 > widest)
		{
			widest		=	w;
		}
	}

	return widest;
}

//---------------------------------------------------------
//	GetHeightOfString
//---------------------------------------------------------

long
cFont::GetHeightOfString(char* psText)
{
	return 0;
}





//---------------------------------------------------------
//	Print
//---------------------------------------------------------

void
cFont::Print(long PixelX,long PixelY,char* psText,eAlign Align)
{
	PrintSP(PixelX<<16,PixelY<<16,psText,Align);
}



void
cFont::PrintSP(long PosX,long PosY,char* psText,eAlign Align)
{
	sSprT*	pST;
	char*	ps;
	char	c;
	long	i,x,y,w;
	DWORD	ARGB;

	if (m_iSprite < 0 || psText == 0)
	{
		return;
	}

	pST					=	&SPR_pTable[m_iSprite];
	ps					=	psText;
	c					=	*ps++;
	y					=	PosY;
	y					+=	m_AdjustY << 16;
	ARGB				=	m_ARGB;

	while (c)
	{
		switch (Align)
		{
		case LEFT:
			x			=	PosX;
			break;

		case CENTER:
			x			=	PosX - (GetWidthOfLine(ps-1) << 15 & 0xFFFF0000);
			break;

		case RIGHT:
			x			=	PosX - (GetWidthOfLine(ps-1) << 16);
			break;
		}

		while (c != 0 && c != 10)
		{
			i			=	c - 32;
			if (i <= 0)
			{
				w		=	m_Kerning + m_SpaceWidth;
			}
			else
			{
				w		=	m_Kerning + pST[i].m_Width;

				Spr[m_iSprite+i].Blit(x+(pST[i].m_CenterX<<16),y,ARGB);
			}

			c			=	*ps++;
			x			+=	w << 16;
		}

		if (c == 10)
		{
			c			=	*ps++;
			y			+=	m_LineHeight << 16;
		}
	}
}

	
