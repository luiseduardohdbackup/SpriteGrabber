#ifdef _WINDOWS

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK new(_CLIENT_BLOCK,__FILE__,__LINE__)
#define new DEBUG_CLIENTBLOCK
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <direct.h>

#else

#import <CoreServices/CoreServices.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

#include <stdio.h>

#include "string_s.h"
#include "file.h"



//---------------------------------------------------------
//	DATA
//---------------------------------------------------------

char	FILE_cAppData[FILE_PATH_MAX];
char	FILE_cDesktop[FILE_PATH_MAX];

//---------------------------------------------------------
//	DISABLE CRT WARNINGS
//---------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4996)




//---------------------------------------------------------
//	PLATFORM AGNOSTIC SOURCE
//---------------------------------------------------------




//---------------------------------------------------------
//	FILE_size
//---------------------------------------------------------

long
FILE_size(FILE* FP)
{
	long	Size;
	long	Pos;

	Pos		=	ftell(FP);
  	fseek(FP,0,SEEK_END);
	Size	=	ftell(FP);
	fseek(FP,Pos,SEEK_SET);
	return Size;
}

//---------------------------------------------------------
//	FILE_size
//---------------------------------------------------------

long
FILE_size(char* psFile)
{
	FILE	*FP;
	long	Size;
	char	cFile[FILE_PATH_MAX];
	
	FILE_name_copy(cFile,psFile);

	FP	=	fopen(cFile,"rb");
	if (FP == NULL)
	{
		return 0;
	}
	else
	{
		Size = FILE_size(FP);
		fclose(FP);
		return Size;
	}
}

//---------------------------------------------------------
//	FILE_open
//---------------------------------------------------------

void*
FILE_open(char* psFile,void* pDest)
{
	FILE	*FP;
	long	Size;
	char	cFile[FILE_PATH_MAX];
	
	FILE_name_copy(cFile,psFile);

	FP	=	fopen(cFile,"rb");
	if (FP==NULL)
	{
		return 0;
	}

	if (0 == (Size = FILE_size(FP)))
	{
		return pDest;
	}

	if (0 == pDest)
	{
		pDest = new char[Size];
	}

	fread(pDest,Size,1,FP);
	fclose(FP);
	return pDest;
}

//---------------------------------------------------------
//	FILE_save
//---------------------------------------------------------

bool
FILE_save(char* psFile,void* pSource,long Size)
{
	FILE	*FP;
	char	cFile[FILE_PATH_MAX];
	
	FILE_name_copy(cFile,psFile);

	FP	=	fopen(cFile,"wb");
	if (FP == NULL)
	{
		return false;
	}

	fwrite(pSource,Size,1,FP);
	fclose(FP);
	return true;
}

//---------------------------------------------------------
//	FILE_append
//---------------------------------------------------------

bool
FILE_append(char* psFile,void* pSource,long Size)
{
	FILE	*FP;
	char	cFile[FILE_PATH_MAX];
	
	FILE_name_copy(cFile,psFile);

	FP	=	fopen(cFile,"ab");
	if (FP == NULL)
	{
		return false;
	}

	if (Size < 0)
	{
		Size	=	(long)strlen((char*)pSource);
	}

	fwrite(pSource,Size,1,FP);
	fclose(FP);
	return true;
}

//---------------------------------------------------------
//	FILE_verify
//---------------------------------------------------------

void
FILE_verify(char* psFile)
{
	char	cFile[FILE_PATH_MAX];
	
	FILE_name_copy(cFile,psFile);

	if (FILE_size(cFile) == 0)
	{
		// todo: display error
	}
}

//---------------------------------------------------------
//	FILE_delete
//---------------------------------------------------------

void
FILE_delete(char* psFile)
{
	remove(psFile);
}



//---------------------------------------------------------
//	FILE_chdir
//---------------------------------------------------------

int
FILE_chdir(char* psDir)
{
#ifdef _WINDOWS
	return _chdir(psDir);
#else
	return chdir(psDir);
#endif
}

//---------------------------------------------------------
//	FILE_getcwd
//---------------------------------------------------------

char*
FILE_getcwd(char* psDest,long DestSize)
{
#ifdef _WINDOWS
	return _getcwd(psDest,DestSize);
#else
	return getcwd(psDest,DestSize);
#endif
}











//---------------------------------------------------------
//	PLATFORM DEPENDENT SOURCE
//---------------------------------------------------------

//---------------------------------------------------------
//	WINDOWS
//---------------------------------------------------------

#ifdef _WINDOWS

//---------------------------------------------------------
//	FILE_init_once - PC
//---------------------------------------------------------

bool
FILE_init_once(char* psGameName)
{
	SHGetFolderPath(0,CSIDL_DESKTOPDIRECTORY,0,SHGFP_TYPE_CURRENT,FILE_cDesktop);
	strcat(FILE_cDesktop,"\\");
	
	if (psGameName)
	{
		SHGetFolderPath(0,CSIDL_LOCAL_APPDATA,0,SHGFP_TYPE_CURRENT,FILE_cAppData);
		strcat(FILE_cAppData,"\\");
		strcat(FILE_cAppData,psGameName);
		CreateDirectory(FILE_cAppData,NULL);
		strcat(FILE_cAppData,"\\");
	}

	return true;
}

//---------------------------------------------------------
//	FILE_name_copy - PC
//---------------------------------------------------------

void
FILE_name_copy(char* psDest,char* psSource)
{
	strcpy(psDest,psSource);
}

//---------------------------------------------------------
//	FILE_time_last_write - PC
//---------------------------------------------------------

QWORD
FILE_time_last_write(char* psFile)
{
	HANDLE		hFile;
	FILETIME	ft;
	QWORD		qw;

	if (FILE_size(psFile))
	{
		hFile	=	CreateFile(
			psFile,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		GetFileTime(hFile,0,0,&ft);
		CloseHandle(hFile);

		qw	=	ft.dwLowDateTime + ((QWORD)ft.dwHighDateTime << 32);
		return qw;
	}

	return 0;
}

//---------------------------------------------------------
//	FILE_enum_files - PC
//---------------------------------------------------------

char*
FILE_enum_files(char* psFileSpec)
{
	static	HANDLE		hFile	=	0;
	WIN32_FIND_DATA		FD;

	if (psFileSpec)
	{
		if (hFile)
		{
			FindClose(hFile);
		}

		hFile		=	FindFirstFile(psFileSpec,&FD);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			hFile	=	0;
			return 0;
		}
		if ((FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			return FD.cFileName;
		}
	}

	while (1)
	{
		if (FindNextFile(hFile,&FD) == 0)
		{
			FindClose(hFile);
			hFile	=	0;
			return 0;
		}
		if ((FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			return FD.cFileName;
		}
	}
}

//---------------------------------------------------------
//	FILE_enum_folders - PC
//---------------------------------------------------------

char*
FILE_enum_folders(char* psFolder)
{
	static	HANDLE		hFile	=	0;
	WIN32_FIND_DATA		FD;
	char				cFileSpec[MAX_PATH];

	if (psFolder)
	{
		strcpy_s(cFileSpec,psFolder);
		strcat_s(cFileSpec,"\\*.*");

		hFile		=	FindFirstFile(cFileSpec,&FD);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			hFile	=	0;
			return 0;
		}
		if (FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FD.cFileName,".") && strcmp(FD.cFileName,".."))
			{
				return FD.cFileName;
			}
		}
	}

	while (1)
	{
		if (FindNextFile(hFile,&FD) == 0)
		{
			FindClose(hFile);
			hFile	=	0;
			return 0;
		}
		if (FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FD.cFileName,".") && strcmp(FD.cFileName,".."))
			{
				return FD.cFileName;
			}
		}
	}
}






//---------------------------------------------------------
//	MAC
//---------------------------------------------------------

#else

//---------------------------------------------------------
//	FILE_init_once - MAC
//---------------------------------------------------------

bool
FILE_init_once(char* psGameName)
{
	FSRef		r;
	OSErr		error;
	
#ifndef _DEBUG
#ifndef _IGNOREBUNDLE

	CFBundleRef	mainBundle;			// get
	CFURLRef	appURL;				// copy
	CFURLRef	resURL;				// copy
	CFStringRef	cfString;			// copy
	
	char		cAppPath[FILE_PATH_MAX];
	char		cResPath[FILE_PATH_MAX];
	
	// get the main bundle ----------------------

	mainBundle		=	CFBundleGetMainBundle();
	if (mainBundle == NULL)
		return false;
	
	// get application file path ----------------
	
	appURL			=	CFBundleCopyBundleURL(mainBundle);
	if (appURL == NULL)
		return false;
	
	cfString		=	CFURLCopyFileSystemPath(appURL,kCFURLPOSIXPathStyle);
	CFRelease(appURL);
	if (cfString == NULL)
		return false;

	CFStringGetCString(cfString,cAppPath,FILE_PATH_MAX,kCFStringEncodingUTF8);
	CFRelease(cfString);

	// get resource path ------------------------

	resURL			=	CFBundleCopyResourcesDirectoryURL(mainBundle);
	if (resURL == NULL)
		return false;

	cfString		=	CFURLCopyFileSystemPath(resURL,kCFURLPOSIXPathStyle);
	CFRelease(resURL);
	if (cfString == NULL)
		return false;

	CFStringGetCString(cfString,cResPath,FILE_PATH_MAX,kCFStringEncodingUTF8);
	CFRelease(cfString);

	// change directory -------------------------
	
	chdir(cAppPath);

	if (strcmp(cAppPath,cResPath))
	{
		chdir(cResPath);
	}

#endif
#endif

	// find desktop folder ----------------------
	
	error	=	FSFindFolder(kUserDomain,kDesktopFolderType,kDontCreateFolder,&r);
	if (error != noErr)
		return false;

	error	=	FSRefMakePath(&r,(UInt8*)FILE_cDesktop,FILE_PATH_MAX-1);
	if (error != noErr)
		return false;

	strcat(FILE_cDesktop,"/");

	// find appdata folder (make if need be) ----

//	TODO:
	
	
	
	return true;
}

//---------------------------------------------------------
//	FILE_name_copy = MAC
//---------------------------------------------------------

void
FILE_name_copy(char* psDest,char* psSource)
{
	char*	ps;

	strcpy(psDest,psSource);

	ps			=	strchr(psDest,'\\');
	while (ps)
	{
		*ps		=	'/';
		ps		=	strchr(psDest,'\\');
	}
}

//---------------------------------------------------------
//	FILE_time_last_write - MAC
//---------------------------------------------------------

QWORD
FILE_time_last_write(char* psFile)
{
	struct	stat	buffer;
	QWORD	qw;
	
 	stat(psFile,&buffer);
	qw	=	buffer.st_mtimespec.tv_sec;
	
	return qw;
}




#endif




//---------------------------------------------------------
//	DEFAULT CRT WARNINGS
//---------------------------------------------------------

#pragma warning(pop)
