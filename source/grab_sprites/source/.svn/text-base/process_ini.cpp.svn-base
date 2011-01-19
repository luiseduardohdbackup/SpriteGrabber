#include "file.h"
#include "cINI.h"
#include "string_s.h"

#include "cBitmap.h"
#include "cFrame.h"
#include "cHeader.h"

#include "process_ini.h"

//---------------------------------------------------------
//	globals
//---------------------------------------------------------

static	long	g_InfileShrinkDef;
static	long	g_ShrinkWrapDef;
static	long	g_BoxDirDef;



DWORD	g_UID[UID_MAX];
long	g_iUID			=	0;


//---------------------------------------------------------
// name: StringIsNumeric
// desc: does a filename string have digits immediatly before it's extension
//       e.g. filename0000.TGA
//---------------------------------------------------------

bool StringIsNumeric(char* ps)
{
	char*	psDigits;

	// verify string has digits
	psDigits = strrchr(ps,'.');
	if (psDigits == 0)
	{
		// '.' not found in string
		return false;
	}
	psDigits--;
	while (psDigits >= ps)
	{
		if (*psDigits >= '0' && *psDigits <= '9')
		{
			// 0-9 found in string
			return true;
		}
		psDigits--;
	}

	return false;
}

//---------------------------------------------------------
// name: IncrementString
// desc: increments the digits in a filename string
//       e.g. filename0000.TGA becomes filename0001.TGA
//---------------------------------------------------------

bool IncrementString(char* ps)
{
	char*	psDigits;
	char*	psDigit;

	// verify string has digits
	psDigits = strrchr(ps,'.');
	if (psDigits == 0)
	{
		// '.' not found in string
		return false;
	}
	psDigits--;
	while (*psDigits < '0' || *psDigits > '9')
	{
		if (psDigits == ps)
		{
			return false;
		}
		psDigits--;
	}

	// next string
	psDigit = psDigits;
	while (*psDigit >= '0' && *psDigit <='9')
	{
		(*psDigit)++;
		if (*psDigit <= '9')
		{
			return true;
		}
		*psDigit = '0';
		psDigit--;
	}

	// end of sequence
	return false;
}

//---------------------------------------------------------
//	PrefixString
//---------------------------------------------------------

bool PrefixString(char* psDest,long DestSize,char* psSource)
{
	long	StringSize;
	long	SourceSize;

	if (psSource == 0)
	{
		return false;
	}

	StringSize	=	(long)strlen(psDest);
	SourceSize	=	(long)strlen(psSource);

	if ((SourceSize + StringSize) >= DestSize)
	{
		return false;
	}

	memmove(psDest+SourceSize,psDest,StringSize);
	memmove(psDest,psSource,SourceSize);
//	memcpy(psDest+SourceSize,psDest,StringSize);
//	memcpy(psDest,psSource,SourceSize);

	psDest[StringSize + SourceSize] = 0;

	return true;
}


DWORD MakeUID(char* psString)
{
	DWORD	uid,top,bot;
	char*	ps;

	uid		=	0;
	ps		=	psString;

	while (*ps)
	{
		uid		+=	*ps++;
		top		=	(uid << 9);
		bot		=	(uid >> (32-9));
		uid		=	top + bot;
	}

	return uid;
}





//---------------------------------------------------------
//	process_ini_passive
//---------------------------------------------------------

bool
process_ini_passive()
{
	return true;
}



//---------------------------------------------------------
//	add_bitmaps_and_frames
//---------------------------------------------------------

bool
add_bitmaps_and_frames(char* cBinPrefix,char* cResPrefix,char* psHeaderFile)
{
	char	cSection[FILE_PATH_MAX];
	char	cInfile[FILE_PATH_MAX];
	char	cTemp[FILE_PATH_MAX];


	cHeader	Header;
	long	Frames;
	long    SkipFrames;
	long	FramesUsed;
	long    TotalFrames;
	
	bool	bBox;
	long	BoxW;
	long	BoxH;
	long	BoxX;
	long	BoxY;
	long	BoxDir;

	BYTE	InfileACO;
	DWORD	InfileARGBFrom;
	DWORD	InfileARGBTo;
	long	InfileShrink;
	long	ShrinkWrap;

	long	iBitmap;
	long	iFrame;
	long	i;

	DWORD	UID;

	// setup header file ------------------------

	Header.Append("#pragma once\n\n// created by sprite grabber\n\n");

	// enumerate sections -----------------------

	cSection[0]			=	0;

	while (ini.EnumSection(cSection,cSection,FILE_PATH_MAX))
	{
		if (strcmp("OUTPUT",cSection) == 0)
		{
			continue;
		}
		printf("        : %s\n",cSection);

		// add #define in header file -----------

		FramesUsed		=	cFrame::GetFramesUsed();
		Header.DefineValue(cSection,FramesUsed);

		// check infile -------------------------

		if (!ini.GetString(cSection,"infile",cInfile,FILE_PATH_MAX))
		{
			printf("ERROR   : NO INFILE\n");
			return false;
		}
		PrefixString(cInfile,FILE_PATH_MAX,cResPrefix);
		if (FILE_size(cInfile) == 0)
		{
			printf("ERROR   : MISSING FILE [%s]\n",cInfile);
			return false;
		}

		// get params ---------------------------

		InfileACO		=	(BYTE)ini.GetLong(cSection,"infile_aco",0);
		InfileARGBFrom	=	ini.GetHex(cSection,"infile_argb_from",0);
		InfileARGBTo	=	ini.GetHex(cSection,"infile_argb_to",0);
		InfileShrink	=	ini.GetLong(cSection,"infile_shrink",g_InfileShrinkDef);
		ShrinkWrap		=	ini.GetLong(cSection,"shrink_wrap",g_ShrinkWrapDef);

		// are we in box mode? ------------------

		bBox			=	ini.GetValue(cSection,"box_w",BoxW,-1);
		bBox			|=	ini.GetValue(cSection,"box_h",BoxH,BoxW);
		bBox			|=	ini.GetValue(cSection,"box_x",BoxX,0);
		bBox			|=	ini.GetValue(cSection,"box_y",BoxY,0);
		bBox			|=	ini.GetValue(cSection,"box_dir",BoxDir,g_BoxDirDef);

		if (bBox)
		{
			// verify we can do box mode --------

			Frames		=	ini.GetLong(cSection,"frames",1);
			if (Frames < 1)
			{
				printf("WARNING : NO FRAMES [frames]\n");
				return true;
			}

			if (BoxW < 0)
			{
				printf("ERROR   : NO BOX WIDTH [box_w]\n");
				return false;
			}
			if (BoxW <= 0 || BoxH <= 0)
			{
				printf("ERROR   : BOX TOO SMALL [box_w | box_h]\n");
				return false;
			}

			// add bitmap -----------------------

			iBitmap				=	cBitmap::Add(cInfile,InfileACO,InfileARGBFrom,InfileARGBTo,InfileShrink);
			if (iBitmap < 0)
			{
				printf("ERROR   : BUFFER OVERUN [TOO MANY BITMAPS - PLEASE UPGRADE LICENSE]\n");
				return false;
			}

			// add MODE_BOX frames --------------

			cFrame::Add(MODE_BOX,iBitmap,BoxX,BoxY,BoxW,BoxH,ShrinkWrap,BoxDir);
			for (i=1; i<Frames; i++)
			{
				iFrame			=	cFrame::Add(MODE_BOX_N,iBitmap,0,0,BoxW,BoxH,ShrinkWrap,BoxDir);
				if (iFrame < 0)
				{
					printf("ERROR   : BUFFER OVERUN [TOO MANY BITMAPS - PLEASE UPGRADE LICENSE]\n");
					return false;
				}
			}
		}
		else
		{
			// verify we can do seq mode --------

			Frames		=	ini.GetLong(cSection,"frames",0);
			if (Frames < 1)
			{
				printf("WARNING : NO FRAMES [frames]\n");
				return true;
			}

			SkipFrames		=	ini.GetLong(cSection,"skipframes",0);
			
			// check file is numeric
			strcpy_s(cTemp,cInfile);

			if (!StringIsNumeric(cTemp) && Frames > 1)
			{
				printf("ERROR   : FILENAME IS NON-NUMERIC (%s)\n",cTemp);
				return true;
			}

			// check files are present

			for (i=1; i<Frames; i++)
			{
				if (IncrementString(cTemp) == false)
				{
					printf("ERROR   : FILENAME NUMERIC TOO SMALL (%s)\n",cTemp);
					return false;
				}
				if (FILE_size(cTemp) == 0)
				{
					printf("ERROR   : FILE MISSING (%s)\n",cTemp);
					return false;
				}
			}

			// add bitmaps and files ------------

			strcpy_s(cTemp,cInfile);

			TotalFrames = 0;
			for (i=0; i<Frames; i++)
			{
				if (SkipFrames && (i % SkipFrames) != 0)
				{
					IncrementString(cTemp);
					continue;
				}

				++TotalFrames;
				iBitmap				=	cBitmap::Add(cTemp,InfileACO,InfileARGBFrom,InfileARGBTo,InfileShrink);
				if (iBitmap < 0)
				{
					printf("ERROR   : BUFFER OVERUN [TOO MANY BITMAPS - PLEASE UPGRADE LICENSE]\n");
					return false;
				}

				iFrame				=	cFrame::Add(MODE_SEQ,iBitmap,0,0,0,0,ShrinkWrap,BoxDir);
				if (iFrame < 0)
				{
					printf("ERROR   : BUFFER OVERUN [TOO MANY SPRITES - PLEASE UPGRADE LICENSE]\n");
					return false;
				}

				IncrementString(cTemp);
			}
		}

		// add UID and frames to UID list
		// user defined UID will override

		UID				=	MakeUID(cSection);

		g_UID[g_iUID]	=	UID;
		g_UID[g_iUID+1]	=	TotalFrames;

		g_iUID			+=	2;
	};

	if (cSection[0] == INI_DUP_SECTION)
	{
		printf("        : %s\n",cSection+1);
		printf("ERROR   : DUPLICATE\n");
		return false;
	}

	// save header file -------------------------

	Header.Save(psHeaderFile);

	return true;
}







//---------------------------------------------------------
//	process_ini
//---------------------------------------------------------

//	entry:	ini is already open
//			in the right folder

bool
process_ini(char* cBinPrefix,char* cResPrefix,long Force)
{
	char	cOutBitmap[FILE_PATH_MAX];
	char	cOutHeader[FILE_PATH_MAX];
	char	cOutTable[FILE_PATH_MAX];

	QWORD	qw;
	long	Border;

	// reset bitmaps and frames -----------------

	cBitmap::Free_All();
	cFrame::Free_All();

	// check for passive mode -------------------

	if (ini.GetLong("OUTPUT","passive_mode"))
	{
		return process_ini_passive();	// not implimented yet
	}

	// read output section ----------------------

	g_InfileShrinkDef		=	ini.GetLong("OUTPUT","infile_shrink",1);
	g_ShrinkWrapDef			=	ini.GetLong("OUTPUT","shrink_wrap",0);
	g_BoxDirDef				=	ini.GetLong("OUTPUT","box_dir",0);

	Border					=	ini.GetLong("OUTPUT","border",0);

	// check required outfiles are defined

	if (false == ini.GetString("OUTPUT","bitmap",cOutBitmap,FILE_PATH_MAX,"out.png"))
	{
		printf("ERROR   : NO OUTPUT BITMAP DEFINED [bitmap]\n");
		return false;
	}
	if (false == ini.GetString("OUTPUT","header",cOutHeader,FILE_PATH_MAX,"out.h"))
	{
		printf("ERROR   : NO OUTPUT HEADER DEFINED [header]\n");
		return false;
	}
	if (false == ini.GetString("OUTPUT","table",cOutTable,FILE_PATH_MAX,"out.spritetable"))
	{
		printf("ERROR   : NO OUTPUT TABLE DEFINED [table]\n");
		return false;
	}

	PrefixString(cOutBitmap,FILE_PATH_MAX,cBinPrefix);
	PrefixString(cOutHeader,FILE_PATH_MAX,cResPrefix);
	PrefixString(cOutTable,FILE_PATH_MAX,cResPrefix);

	// enum sections and add bitmaps and frames
	// also check dates on input bitmaps

	if (false == add_bitmaps_and_frames(cBinPrefix,cResPrefix,cOutHeader))
	{
		return false;
	}

	// check date of ini file and all bitmaps
	// if output files are newer, return true

	qw	=	FILE_time_last_write(cOutBitmap);
	if (Force == 0 && qw > cBitmap::GetNewestFile())
	{
		printf("UP2DATE : NO PROCESSING REQUIRED\n");
		return true;
	}

	// open bitmaps
	// place boxes
	// shink wrap

	if (false == cFrame::PlaceBoxAndShrinkWrap_All())
	{
		return false;
	}

	// position frames on final output bitmap

	if (false == cFrame::Pack_All(Border))
	{
		return false;
	}

	// copy all and save bitmap

	cFrame::Copy_All(cOutBitmap);

	// create and save table

	cFrame::SaveTable(cOutTable);

	return true;
}




