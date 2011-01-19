#pragma once
/*
#include "file.h"

extern	char	cOutBitmap[FILE_PATH_MAX];
extern	char	cOutHeader[FILE_PATH_MAX];
extern	char	cOutTable[FILE_PATH_MAX];
*/

#define UID_MAX						0x10000

extern	DWORD	g_UID[UID_MAX];
extern	long	g_iUID;


extern	bool	PrefixString(char* psDest,long DestSize,char* psSource);

extern	bool	process_ini(char* cBinPrefix,char* cResPrefix,long Force);




