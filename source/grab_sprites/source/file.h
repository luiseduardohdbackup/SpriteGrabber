#pragma once

// this comment was added on Mac, to test svn





#define FILE_PATH_MAX		512

#include <stdio.h>
#include "base.h"

extern	char	FILE_cAppData[FILE_PATH_MAX];
extern	char	FILE_cDesktop[FILE_PATH_MAX];

// platform agnostic ------------------

extern	long	FILE_size(FILE* FP);
extern	long	FILE_size(char* psFile);
extern	void*	FILE_open(char* psFile,void* pDest=0);
extern	bool	FILE_save(char* psFile,void* pSource,long Size);
extern	bool	FILE_append(char* psFile,void* pSource,long Size=-1);
extern	void	FILE_verify(char* psFile);
extern	void	FILE_delete(char* psFile);

//extern	bool	FILE_mkdir(char* psDir);
extern	int		FILE_chdir(char* psDir);
extern	char*	FILE_getcwd(char* psDest,long DestSize);

// platform dependent -----------------

extern	bool	FILE_init_once(char* psGameName=0);
extern	void	FILE_name_copy(char* psDest,char* psSource);

extern	QWORD	FILE_time_last_write(char* psFile);

extern	char*	FILE_enum_files(char* psFileSpec=0);
extern	char*	FILE_enum_folders(char* psFolder);


