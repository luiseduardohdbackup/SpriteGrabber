#pragma once

#define FONTS_MAX			16

#include "base.h"


enum eAlign
{
	LEFT,
	RIGHT,
	CENTER
};


struct sFont
{
	long	m_iSprite;
	long	m_Kerning;
	long	m_AdjustY;
	long	m_SpaceWidth;
	long	m_LineHeight;
};



class cText
{
public:

	cText();
	cText(char* pString=0,long PixelX=0,long PixelY=0,eAlign Align=LEFT,BYTE iFont=0,DWORD ARGB=0xFFFFFFFF);

	void	SetString(char* pString);
	void	SetPixelX(long PixelX);
	void	SetPixelY(long PixelY);
	void	SetAlign(eAlign Align);
	void	SetFont(BYTE iFont);
	void	SetARGB(DWORD ARGB);
	void	SetXPos(long XPos);
	void	SetYPos(long YPos);

	long	GetWidth();
	long	GetWidthOfLine(char* psLine);

	void	Print();

	static	bool	RegisterFont(long iSprite,long Kerning,long AdjustY,long SpaceWidth,long LineHeight);

private:

	char*	m_pString;
	long	m_XPos;
	long	m_YPos;
	BYTE	m_Align;
	BYTE	m_iFont;
	DWORD	m_ARGB;

	static	sFont	s_Font[FONTS_MAX];
	static	long	s_iFont;
};

