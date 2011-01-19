#pragma once

#include "base.h"

#include "cTex.h"
#include "cSpr.h"
#include "sprite_defines.h"



#define STF_CENTER_IS_SET	1

//---------------------------------------------------------
//	Struct
//---------------------------------------------------------

struct sSprT
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
//	Externs
//---------------------------------------------------------

extern	cTex	Tex[TEX_MAX];
extern	cSpr	Spr[SPR_MAX];

extern	sSprT*	SPR_pTable;

extern	bool	SPR_init_once(char* psTexturesINI,char* psSpriteTable);
extern	void	SPR_kill_once();




