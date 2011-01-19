#pragma once

#define FONTS_MAX			16

#include "base.h"


enum eAlign
{
	LEFT,
	RIGHT,
	CENTER
};




class cFont
{
public:

	cFont();


	void	SetFont(long iSprite,long SpaceWidth=4,long Kerning=1);
	void	SetARGB(DWORD ARGB);
	void	SetLineHeight(long LineHeight);
	void	SetAdjustY(long AdjustY);

	long	GetWidthOfLine(char* psText);
	long	GetWidthOfString(char* psText);
	long	GetHeightOfString(char* psText);

	void	Print(long PixelX,long PixelY,char* psText,eAlign Align=LEFT);
	void	PrintSP(long PosX,long PosY,char* psText,eAlign Align=LEFT);

private:

	long	m_iSprite;
	long	m_Kerning;
	long	m_AdjustY;
	long	m_SpaceWidth;
	long	m_LineHeight;
	DWORD	m_ARGB;
};

extern	cFont	Font[FONTS_MAX];



