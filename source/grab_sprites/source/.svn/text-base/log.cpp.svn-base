#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include "string_s.h"

#include "file.h"
#include "log.h"

char	LOG_cFile[FILE_PATH_MAX];


//---------------------------------------------------------
//	LOG_create
//---------------------------------------------------------

void
LOG_create(char* psFile)
{
	strcpy(LOG_cFile,psFile);

	FILE_save(LOG_cFile,(void*)"LOGFILE\r\n\r\n",11);
}

//---------------------------------------------------------
//	LOG_create_AD
//---------------------------------------------------------

void
LOG_create_AD(char* psFile)
{
	char	cFile[FILE_PATH_MAX];

	strcpy(cFile,FILE_cAppData);
	strcat(cFile,psFile);

	LOG_create(cFile);
}

//---------------------------------------------------------
//	LOG_create_DT
//---------------------------------------------------------

void
LOG_create_DT(char* psFile)
{
	char	cFile[FILE_PATH_MAX];

	strcpy(cFile,FILE_cDesktop);
	strcat(cFile,psFile);

	LOG_create(cFile);
}

//---------------------------------------------------------
//	LOG_append
//---------------------------------------------------------

bool
LOG_append(char* psFile,long Line,char* psString)
{
	static	char	cFile[256];

			char	cLine[256];
			char	cTemp[256];
			char	cData[1024];

	if (LOG_cFile[0] == 0)
	{
		return false;
	}

	cData[0]				=	0;

	if (strcmp(cFile,psFile))
	{
		strcpy_s(cFile,psFile);
		sprintf(cTemp,"\r\nFILE : %s\r\n",psFile);
		strcat_s(cData,cTemp);
	}

	sprintf(cLine,"%4i : %s",Line,psString);

	strcat_s(cData,cLine);
	strcat_s(cData,"\r\n");

#ifdef _WINDOWS
	OutputDebugString(cData);
#endif

	return FILE_append(LOG_cFile,cData,(long)strlen(cData));
}

