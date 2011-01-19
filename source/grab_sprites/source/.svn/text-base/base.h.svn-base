#pragma once

// define types -------------------------------------------

#ifndef _MSC_VER
#include <inttypes.h>
typedef uint8_t					BYTE;
typedef uint16_t				WORD;
typedef uint32_t				DWORD;
typedef uint64_t				QWORD;
#else
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned long			DWORD;
typedef unsigned long long int	QWORD;
#endif

// macros to delete objects and arrays --------------------

#define SAFE_DELETE(a)			{if (a) {delete (a); (a) = NULL;}}
#define SAFE_DELETE_ARRAY(a)	{if (a) {delete[] (a); (a) = NULL;}}

// support for file:line on memory leaks ------------------

#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK new(_CLIENT_BLOCK,__FILE__,__LINE__)
#define new DEBUG_CLIENTBLOCK
#endif
#endif

