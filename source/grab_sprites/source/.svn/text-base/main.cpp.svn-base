#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <crtdbg.h>
#include <direct.h>
#include <conio.h>
#endif



#include <stdlib.h>
#include <stdio.h>




#include "FreeImage.h"

#include "includes.h"
#include "endian.h"


#include "cINI.h"
#include "string_s.h"
#include "cBitmap.h"
#include "cFrame.h"
#include "cHeader.h"
#include "process_ini.h"








char	cPrefixBin[FILE_PATH_MAX];
char	cPrefixRes[FILE_PATH_MAX];















int main(int argc,char* argv[])
{
	char	cOutTextureList[FILE_PATH_MAX];
	char	cOutSpriteTable[FILE_PATH_MAX];
	char	cOutHeaderFile[FILE_PATH_MAX];
	char	cOutUIDsFile[FILE_PATH_MAX];
	
	char	cOutBitmap[FILE_PATH_MAX];

	char	cCWD[FILE_PATH_MAX];
	char	cTemp[FILE_PATH_MAX];


	char	cBin[FILE_PATH_MAX];
	char	cRes[FILE_PATH_MAX];


	cINI	iniMain;
	cINI	iniTL;

	char	cSection[FILE_PATH_MAX];
	char	cValue[16];

	char*	ps;
	long	i;

	sTable*	pTable;
	WORD	iBitmap;
	long	iSprite;
	long	Sprites;

	long	Force;
	long	HaltOnExit;

	cHeader	HeaderIn;
	cHeader	HeaderOut;

	// ------------------------------------------

#ifdef _WINDOWS
#ifdef _DEBUG

	long	Flags;

	Flags	=	_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	Flags	|=	_CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF;
	Flags	&=	~_CRTDBG_CHECK_ALWAYS_DF;
				_CrtSetDbgFlag(Flags);

#endif
#endif

	// ------------------------------------------
	
	FreeImage_Initialise();

	FILE_init_once();
	
	FILE_getcwd(cCWD,FILE_PATH_MAX);
	printf("CWD     : %s\n",cCWD);

	// open main ini file -----------------------

	if (argc > 1)
	{
		strcpy_s(cTemp,argv[1]);
	}
	else
	{
		strcpy_s(cTemp,"grab_sprites.txt");
	}

	printf("FILE    : %s\n",cTemp);

	if (iniMain.Open(cTemp) == false)
	{
		printf("ERROR   : MISSING FILE\n");
		goto FAILED;
	}

	// PHASE 0 ----------------------------------

	iniMain.GetString("OUTPUT","header",cOutHeaderFile, FILE_PATH_MAX,"sprite_defines.h");
	iniMain.GetString("OUTPUT","table", cOutSpriteTable,FILE_PATH_MAX,"sprite_table.bin");
	iniMain.GetString("OUTPUT","list",  cOutTextureList,FILE_PATH_MAX,"texture_list.ini");
	iniMain.GetString("OUTPUT","uids",  cOutUIDsFile,   FILE_PATH_MAX,"sprite_uids.bin");

	iniMain.GetString("OUTPUT","prefix_bin",cPrefixBin, FILE_PATH_MAX);
	iniMain.GetString("OUTPUT","prefix_res",cPrefixRes, FILE_PATH_MAX,cPrefixBin);

	Force			=	iniMain.GetLong("OUTPUT","force");
	HaltOnExit		=	iniMain.GetLong("OUTPUT","halt_on_exit");

#ifdef VER_DEMO

	if (cPrefixBin[0] || cPrefixRes[0])
	{
		printf("ERROR   : prefix_* UNAVAILABLE IN THIS VERSION - PLEASE UPGRADE LICENSE\n");
		goto FAILED;
	}
	HaltOnExit		=	1;

#endif

#ifdef _DEBUG

	HaltOnExit		=	1;

#endif

	// PHASE 1 ----------------------------------

	printf("\nPHASE   : 1 -------------------------------------------------------------------\n");

	cSection[0]				=	0;
	while (iniMain.EnumSection(cSection,cSection,FILE_PATH_MAX))
	{
		if (strcmp("OUTPUT",cSection) == 0)
		{
			continue;
		}

		printf("\nSECTION : %s\n",cSection);

		if (iniMain.GetString(cSection,"infile",cTemp,FILE_PATH_MAX) == false)
		{
			printf("ERROR   : NO INFILE SPECIFIED\n");
			goto FAILED;
		}
		strcpy(cBin,cTemp);
		strcpy(cRes,cTemp);
		PrefixString(cBin,FILE_PATH_MAX,cPrefixBin);
		PrefixString(cRes,FILE_PATH_MAX,cPrefixRes);

		ps			=	cRes;
		if (false == ini.Open(ps))
		{
			printf("ERROR   : INFILE IS MISSING [%s]\n",ps);
			goto FAILED;
		}
		cBitmap::SetNewestFile(FILE_time_last_write(ps));

		// create bin and res prefix strings --------------

		ps			=	strrchr(cBin,'/');
		if (ps)
		{
			ps[1]	=	0;
		}
		ps			=	strrchr(cRes,'/');
		if (ps)
		{
			ps[1]	=	0;
		}

		if (false == process_ini(cBin,cRes,Force))
		{
			goto FAILED;
		}
	};

	if (cSection[0] == INI_DUP_SECTION)
	{
		printf("TEXTURE : %s\n",cSection+1);
		printf("ERROR   : DUPLICATE\n");
		goto FAILED;
	}





	// PHASE 2 ----------------------------------

	printf("\nPHASE   : 2 -------------------------------------------------------------------\n");

	iBitmap					=	0;
	iSprite					=	0;

	// create texture list

	FILE_delete(cOutTextureList);
	iniTL.Open(cOutTextureList);

	HeaderOut.Append("#pragma once\n\n");
	HeaderOut.Append("// master sprite header file\n");
	HeaderOut.Append("// created by Sprite Grabber\n\n");
	HeaderOut.Append("// www.jamiewoodhouse.co.uk\n\n");

	cSection[0]				=	0;
	while (iniMain.EnumSection(cSection,cSection,FILE_PATH_MAX))
	{
		if (strcmp("OUTPUT",cSection) == 0)
		{
			continue;
		}

		printf("\nSECTION : %s\n",cSection);

		iniMain.GetString(cSection,"infile",cTemp,FILE_PATH_MAX);

		strcpy(cBin,cTemp);
		strcpy(cRes,cTemp);
		PrefixString(cBin,FILE_PATH_MAX,cPrefixBin);
		PrefixString(cRes,FILE_PATH_MAX,cPrefixRes);

		// open ini file ----------------------------------

		ps		=	cRes;
		ini.Open(ps);

		// create bin and res prefix strings --------------

		ps			=	strrchr(cBin,'/');
		if (ps)
		{
			ps[1]	=	0;
		}
		ps			=	strrchr(cRes,'/');
		if (ps)
		{
			ps[1]	=	0;
		}

		// get pathname for bitmap ------------------------

		ini.GetString("OUTPUT","bitmap",cOutBitmap,FILE_PATH_MAX,"out.png");
		PrefixString(cOutBitmap,FILE_PATH_MAX,cBin);

		sprintf_s(cValue,"%03i",iBitmap);
		iniTL.SetString("TEXTURES",cValue,cOutBitmap);

		// append to master header ------------------------

		HeaderOut.Append("// ");
		HeaderOut.Append(cOutBitmap);
		HeaderOut.Append(" ");
		
		// find the bitmap and output the size
		long bitmapLength = (long)strlen(cOutBitmap);
		cSavedBitmap *currentSavedBitmap = NULL;
		for (int it = 0; it < cSavedBitmap::s_nSavedBitmaps; ++it)
			if (strcmp(SavedBitmap[it].m_cFile, cOutBitmap) == 0)
				currentSavedBitmap = &SavedBitmap[it];
		if (currentSavedBitmap)
		{
			char	cValue[32];
			sprintf(cValue,"%ix%i ", currentSavedBitmap->m_Width, currentSavedBitmap->m_Height);
			HeaderOut.Append(cValue);
			bitmapLength += (long)strlen(cValue);
		}
		
		i			=	56 - bitmapLength;
		while (i-- > 0)
		{
			HeaderOut.Append("-");
		}
		HeaderOut.Append("\n");

		ini.GetString("OUTPUT","header",cTemp,FILE_PATH_MAX,"out.h");
		PrefixString(cTemp,FILE_PATH_MAX,cRes);

		HeaderIn.Open(cTemp);
 		while (HeaderIn.ReadValue(cTemp,sizeof(cTemp),i))
		{
			HeaderOut.DefineValue(cTemp,i+iSprite);
		}
		HeaderOut.Append("\n");

		// create local table (open from file) ------------

		ini.GetString("OUTPUT","table",cTemp,FILE_PATH_MAX,"out.spritetable");
		PrefixString(cTemp,FILE_PATH_MAX,cRes);

		Sprites		=	FILE_size(cTemp) / sizeof(sTable);
		pTable		=	(sTable*)FILE_open(cTemp);

		// append local table to master table -------------

		if (iBitmap)
		{
			for (i=0; i<Sprites; i++)
			{
				pTable[i].m_iBitmap	+=	iBitmap;
			}
			FILE_append(cOutSpriteTable,pTable,Sprites*sizeof(sTable));
		}
		else
		{
			FILE_save(cOutSpriteTable,pTable,Sprites*sizeof(sTable));
		}

		delete[] pTable;

		// advance to next bitmap -------------------------

		iBitmap++;
		iSprite		+=	Sprites;
	};

	HeaderOut.Append("\n// total number of sprites and textures --------------------\n");
	HeaderOut.DefineValue("SPR_MAX",iSprite);
	HeaderOut.DefineValue("TEX_MAX",iBitmap);
	HeaderOut.Save(cOutHeaderFile);

	// save UIDs (UID,frames,UID,frames,UID,frames,...) ---

	ENDIAN_DWORD_ARRAY(g_UID,g_iUID);
	FILE_save(cOutUIDsFile,g_UID,g_iUID*4);

	printf("\nPASSED");
	goto DONE;

FAILED:

	printf("\nFAILED\n");
	return 1;
DONE:

	printf("\n\n");
	printf("Sprite Grabber v%s",VER);
	printf(" - www.jamiewoodhouse.co.uk");

#ifndef VER_DEMO
	printf(" - FULL");
#endif


#ifdef _WINDOWS
	if (HaltOnExit)
	{
		printf("\n\n");
		printf("Press Any Key");
		_getch();
	}
#endif

	FreeImage_DeInitialise();
	return 0;
}

