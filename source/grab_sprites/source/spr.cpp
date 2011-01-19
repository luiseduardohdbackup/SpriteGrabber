#include "spr.h"

#include "cINI.h"
#include "file.h"


cTex	Tex[TEX_MAX];
cSpr	Spr[SPR_MAX];

sSprT*	SPR_pTable	=	0;


//---------------------------------------------------------
//	SPR_init_once
//---------------------------------------------------------

bool
SPR_init_once(char* psTexturesINI,char* psSpriteTable)
{
	sSprT*	pST;
	cINI	iniTL;
	long	iTex;
	long	i;

	char	cVal[16];
	char	cFile[FILE_PATH_MAX];

	if (false == iniTL.Open(psTexturesINI))
	{
		return false;
	}

	SPR_pTable		=	(sSprT*)FILE_open(psSpriteTable);
	pST				=	SPR_pTable;
	if (pST == 0)
	{
		return false;
	}

	for (i=0; i<TEX_MAX; i++)
	{
		sprintf(cVal,"%03i",i);
		if (iniTL.GetString("TEXTURES",cVal,cFile,FILE_PATH_MAX))
		{
			Tex[i].OpenFile(cFile);
		}
	}

	for (i=0; i<SPR_MAX; i++)
	{
		iTex		=	pST[i].m_iBitmap;
		Spr[i].SetTexture(&Tex[iTex]);
		Spr[i].SetSource(pST[i].m_XPos,pST[i].m_YPos,pST[i].m_Width,pST[i].m_Height);
		if (pST[i].m_Flags & STF_CENTER_IS_SET)
		{
			Spr[i].SetCenter(pST[i].m_CenterX,pST[i].m_CenterY);
		}
	}

	cTex::BeginLoadingFiles();

	return true;
}


//---------------------------------------------------------
//	SPR_kill_once
//---------------------------------------------------------

void
SPR_kill_once()
{
	if (SPR_pTable)
	{
		delete[] SPR_pTable;
		SPR_pTable	=	0;
	}
}