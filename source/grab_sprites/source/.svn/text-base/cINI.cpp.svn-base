#include <stdlib.h>

#include "cINI.h"
#include "file.h"
#include "string_s.h"


//---------------------------------------------------------
//	Constructor / Destructor
//---------------------------------------------------------

cINI::cINI()
{
	m_cFilename[0]	=	0;

	m_psData		=	0;
	m_psDataEnd		=	0;
	m_BufferSize	=	0;
	m_psScan		=	0;

	m_bSecureSave	=	false;
	m_bChanged		=	false;
}

cINI::~cINI()
{
	Data_Save(true);
	Data_Free();
}

//---------------------------------------------------------
//	Open
//---------------------------------------------------------

//	notes:	opens or creates a file
//	in:		if bSecureSave save on each write (put)

bool
cINI::Open(char* psFile,bool bSecureSave)
{
 	Data_Free();

	strcpy_s(m_cFilename,psFile);

	m_BufferSize	=	FILE_size(psFile);
	if (m_BufferSize == 0)
	{
		return false;
	}

	m_psData		=	(char*)FILE_open(psFile);
	m_psDataEnd		=	m_psData + m_BufferSize;
	m_psScan		=	m_psData;
	m_bSecureSave	=	bSecureSave;

	return true;
}

//---------------------------------------------------------
//	Open_AD
//---------------------------------------------------------

//	notes:	opens or creates a file in application data folder
//	in:		if bSecureSave save on each write (put)

bool
cINI::Open_AD(char* psFile,bool bSecureSave)
{
	char	cFile[FILE_PATH_MAX];

	strcpy_s(cFile,FILE_cAppData);
	strcat_s(cFile,psFile);

	return Open(cFile,bSecureSave);
}


//---------------------------------------------------------
//	GetString
//---------------------------------------------------------

bool
cINI::GetString(char* psSection,char* psKey,char* psDest,long DestSize,char* psDefault,bool bIncludeWS)
{
	long	ValueSize;
	
	if (psSection == 0 || psKey == 0)
	{
		return false;
	}
	if (psDest == 0)
	{
		return false;
	}
	
	// scan for section
	
	if (Scan_findSection(psSection))
	{
		if (Scan_findKey(psKey))
		{
			if (Scan_toValue())
			{
				ValueSize	=	Scan_sizeValue(bIncludeWS);
				if (ValueSize < DestSize)
				{
					// string fits in buffer ok
					
					memcpy(psDest,m_psScan,ValueSize);
					psDest[ValueSize]	=	0;
					return true;
				}
				else
				{
					// string is too long for buffer
					
					if (DestSize >= 24)
					{
						strcpy_s(psDest,DestSize,"<INI::GETSTRING_ERROR>");
					}
					else
					{
						*psDest			=	0;
					}
					return true;
				}
			}
		}
	}
	
	if (psDefault)
	{
		strcpy_s(psDest,DestSize,psDefault);
		return true;
	}
	
	return false;
}


//---------------------------------------------------------
//	SetString
//---------------------------------------------------------

bool
cINI::SetString(char* psSection,char* psKey,char* psValue)
{
	char	cTemp[512];
	long	SizeRemove;

	if (psSection == 0 || psKey == 0 || psValue == 0)
	{
		return false;
	}

	// scan for section

	if (Scan_findSection(psSection))
	{
		// found existing section

		if (Scan_findKey(psKey))
		{
			// modify existing key

			if (Scan_toValue())
			{
				SizeRemove	=	Scan_sizeValue(true);
			}
			else
			{
				SizeRemove	=	0;
			}

			Data_Insert(psValue,SizeRemove);
		}
		else
		{
			// create a new key

			sprintf_s(cTemp,"%s = %s\n",psKey,psValue);
			Data_Insert(cTemp,0);
		}
	}
	else
	{
		// create new section at end of file

		if (m_psData == 0)
		{
			sprintf_s(cTemp,"[%s]\n",psSection);
		}
		else
		{
			sprintf_s(cTemp,"\n[%s]\n",psSection);
		}
		Data_Append(cTemp);

		// create new key

		sprintf_s(cTemp,"%s = %s\n",psKey,psValue);
		Data_Append(cTemp);
	}

	Data_Save();
	return true;
}


//---------------------------------------------------------
//	GetValue
//---------------------------------------------------------

bool
cINI::GetValue(char* psSection,char* psKey,long& Value,long Default)
{
	char	cValue[16];

	if (GetString(psSection,psKey,cValue,sizeof(cValue)))
	{
		Value	=	strtol(cValue,NULL,10);
		return true;
	}
	else
	{
		Value	=	Default;
		return false;
	}
}


//---------------------------------------------------------
//	SetValue
//---------------------------------------------------------

bool
cINI::SetValue(char* psSection,char* psKey,long Value)
{
	char	cValue[16];
	
	sprintf_s(cValue,"%i",Value);
	
	return SetString(psSection,psKey,cValue);
}

//---------------------------------------------------------
//	GetLong
//---------------------------------------------------------

long
cINI::GetLong(char* psSection,char* psKey,long Default)
{
	long	Value;

	GetValue(psSection,psKey,Value,Default);
	return Value;
}

//---------------------------------------------------------
//	GetHex
//---------------------------------------------------------

DWORD
cINI::GetHex(char* psSection,char* psKey,DWORD Default)
{
	char	cValue[16];
	DWORD	Value;

	if (GetString(psSection,psKey,cValue,sizeof(cValue)))
	{
		Value	=	strtoul(cValue,NULL,16);
		return Value;
	}
	else
	{
		return Default;
	}
}

//---------------------------------------------------------
//	EnumSection
//---------------------------------------------------------

bool
cINI::EnumSection(char* psSection,char* psDest,long DestSize)
{
	char*	ps;
	long	Length;

	if (psSection == 0 || psSection[0] == 0)
	{
		Scan_Reset();
	}
	else
	{
		if (Scan_findSection(psSection) == false)
		{
			return false;
		}
	}

	do
	{
		// check we have a section
		if (*m_psScan != '[')
		{
			continue;
		}

		// scan section name
		m_psScan++;
		ps						=	m_psScan;
		while (*m_psScan != ']' && Scan_isEOL() == false)
		{
			m_psScan++;
		}
		Length					=	(long)(m_psScan - ps);
		if (Length >= (DestSize-1))
		{
			return false;
		}

		// copy section name
		memcpy(psDest,ps,Length);
		psDest[Length]			=	0;

		// scan for duplicate section
		if (Scan_findSection(psDest,false))
		{
			memcpy(psDest+1,ps,Length);
			psDest[Length+1]	=	0;
			psDest[0]			=	INI_DUP_SECTION;
			return false;
		}

		// section is unique
		return true;
	}
	while (Scan_toNextLine());

	return false;
}

//---------------------------------------------------------
//	EnumKey
//---------------------------------------------------------

bool
cINI::EnumKey(char* psSection,char* psDest,long DestSize)
{
	char*	ps;
	char*	psEnd;

	if (psSection == 0 || psSection[0] == 0)
	{
		return false;
	}

	if (psDest == 0 || psDest[0] == 0)
	{
		if (Scan_findSection(psSection) == false)
		{
			return false;
		}
	}
	else
	{
		Scan_findSection(psSection);
		Scan_findKey(psDest);
		Scan_toNextLine();
	}

	ps						=	psDest;
	psEnd					=	psDest + (DestSize - 1);

	do
	{
		if (Scan_isEOF() || *m_psScan == '[')
		{
			return false;
		}

		if (!Scan_isWS())
		{
			while (!Scan_isWS())
			{
				*ps++		=	*m_psScan++;
				if (ps == psEnd)
				{
					return false;
				}
			}

			*ps				=	0;
			return true;
		}

		Scan_toNextLine();
	}
	while (1);
}








//---------------------------------------------------------
//
//	Scan Functions
//
//---------------------------------------------------------

//---------------------------------------------------------
//	Scan_Reset
//---------------------------------------------------------

void
cINI::Scan_Reset()
{
	m_psScan	=	m_psData;
}

//---------------------------------------------------------
//	Scan_isEOF
//---------------------------------------------------------

bool
cINI::Scan_isEOF()
{
	if (m_psScan >= m_psDataEnd)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------
//	Scan_isEOL
//---------------------------------------------------------

bool
cINI::Scan_isEOL()
{
	if (m_psScan >= m_psDataEnd)
	{
		return true;
	}
	if (*m_psScan == ASCII_LF || *m_psScan == ASCII_CR)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------
//	Scan_isWS
//---------------------------------------------------------

bool
cINI::Scan_isWS()
{
	if (m_psScan >= m_psDataEnd)
	{
		return true;
	}
	if (*m_psScan <= ' ')
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------
//	Scan_toValue
//---------------------------------------------------------

bool
cINI::Scan_toValue()
{
	while (!Scan_isEOL())
	{
		if (!Scan_isWS() && *m_psScan != '=')
		{
			return true;
		}
		m_psScan++;
	}
	return false;
}

//---------------------------------------------------------
//	Scan_toNextLine
//---------------------------------------------------------

bool
cINI::Scan_toNextLine()
{
	while (m_psScan < m_psDataEnd && (ASCII_LF != *m_psScan && ASCII_CR != *m_psScan))
	{
		m_psScan++;
	}
	while (m_psScan < m_psDataEnd && (ASCII_LF == *m_psScan || ASCII_CR == *m_psScan))
	{
		m_psScan++;
	}
	if (m_psScan >= m_psDataEnd)
	{
		return false;
	}
	return true;
}

//---------------------------------------------------------
//	Scan_isSection
//---------------------------------------------------------

bool
cINI::Scan_isSection(char* psSection)
{
	char*	psCompare;

	if (*m_psScan != '[')
	{
		return false;
	}

	psCompare	=	psSection;
	m_psScan++;

	while (*m_psScan == *psCompare)
	{
		if (Scan_isEOL())
		{
			return false;
		}

		psCompare++;
		m_psScan++;

		if (*psCompare == 0)
		{
			if (*m_psScan == ']')
			{
				Scan_toNextLine();
				return true;
			}
			return false;
		}
	}
	return false;
}

//---------------------------------------------------------
//	Scan_findSection
//---------------------------------------------------------

bool
cINI::Scan_findSection(char* psSection,bool bFromStart)
{
	if (Data_Size() == 0)
	{
		return false;
	}

	if (bFromStart)
	{
		Scan_Reset();
	}

	if (Scan_isEOF())
	{
		return false;
	}

	do
	{
		if (*m_psScan == '[')
		{
			if (Scan_isSection(psSection))
			{
				return true;
			}
		}
	}
	while (Scan_toNextLine());

	return false;
}

//---------------------------------------------------------
//	Scan_isKey
//---------------------------------------------------------

bool
cINI::Scan_isKey(char* psKey)
{
	char*	psCompare;

	psCompare	=	psKey;

	while (*m_psScan == *psCompare)
	{
		if (Scan_isEOL())
		{
			return false;
		}

		psCompare++;
		m_psScan++;

		if (*psCompare == 0)
		{
			if (Scan_isWS())
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

//---------------------------------------------------------
//	Scan_findKey
//---------------------------------------------------------

//	in:		m_psScan is on next line from [SECTION]
//
//	out:
//	true:	m_psScan is at end of key
//	false:	m_psScan is at start of line to insert new key

bool
cINI::Scan_findKey(char* psKey)
{
	char*	psStart;

	psStart						=	m_psScan;

	do
	{
		if (*m_psScan == '[')
		{
			m_psScan			=	psStart;
			return false;
		}

		if (Scan_isKey(psKey))
		{
			return true;
		}
	}
	while (Scan_toNextLine());
	
	m_psScan					=	psStart;
	return false;	
}

//---------------------------------------------------------
//	Scan_sizeValue
//---------------------------------------------------------

//	in:		include white space?

//	out:	size in bytes of value
//	out:	m_psScan unchanged

long
cINI::Scan_sizeValue(bool bIncludeWS)
{
	char*	psTemp;
	long	Size;

	psTemp		=	m_psScan;
	Size		=	0;

	if (bIncludeWS)
	{
		while (!Scan_isEOL())
		{
			m_psScan++;
			Size++;
		}
	}
	else
	{
		while (!Scan_isWS())
		{
			m_psScan++;
			Size++;
		}
	}

	m_psScan	=	psTemp;

	return Size;
}








//---------------------------------------------------------
//
//	Data Functions
//
//---------------------------------------------------------

//---------------------------------------------------------
//	Data_Save
//---------------------------------------------------------

void
cINI::Data_Save(bool bForceSave)
{
	if (m_cFilename[0] == 0 || m_BufferSize == 0)
	{
		return;
	}

	if (bForceSave)
	{
		if (m_bChanged)
		{
			FILE_save(m_cFilename,m_psData,Data_Size());
			m_bChanged	=	false;
		}
	}
	else
	{
		if (m_bSecureSave)
		{
			FILE_save(m_cFilename,m_psData,Data_Size());
			m_bChanged	=	false;
		}
		else
		{
			m_bChanged	=	true;
		}
	}
}

//---------------------------------------------------------
//	Data_Size
//---------------------------------------------------------

long
cINI::Data_Size()
{
	return (long)(m_psDataEnd - m_psData);
}

//---------------------------------------------------------
//	Data_Free
//---------------------------------------------------------

void
cINI::Data_Free()
{
	if (m_psData)
	{
		delete[] m_psData;
	}
	
	m_psData		=	0;
	m_psDataEnd		=	0;
	m_BufferSize	=	0;
	m_psScan		=	0;
}



//---------------------------------------------------------
//	Data_Append
//---------------------------------------------------------

//	action:		appends a string to end of data

#pragma warning (disable:4996)

void
cINI::Data_Append(char* psString)
{
	long	Size;

	Size			=	(long)strlen(psString);

	Data_Expand(m_BufferSize + Size + 2);

	strcpy(m_psDataEnd,psString);

	m_psDataEnd		+=	Size;
}

#pragma warning (default : 4996)

//---------------------------------------------------------
//	Data_Insert
//---------------------------------------------------------

//	action:		insert a string at scan point

void
cINI::Data_Insert(char* psString,long SizeRemove)
{
	long	SizeA,SizeB,SizeC;
	long	SizeFinal;
	char*	psA;
	char*	psB_s;
	char*	psB_d;
	char*	psC_s;
	char*	psC_d;

	if (m_psScan < m_psData || m_psScan > m_psDataEnd)
	{
		return;
	}

	SizeA		=	(long)(m_psScan - m_psData);
	SizeB		=	(long)strlen(psString);
	SizeC		=	Data_Size() - (SizeA + SizeRemove);

	SizeFinal	=	SizeA + SizeB + SizeC;

	Data_Expand(SizeFinal + 2);

	psA			=	m_psData;
	psB_s		=	psString;
	psB_d		=	psA + SizeA;
	psC_s		=	psB_d + SizeRemove;
	psC_d		=	psB_d + SizeB;

	if (psC_s != psC_d)
	{
//		memcpy(psC_d,psC_s,SizeC);
		memmove(psC_d,psC_s,SizeC);
	}

	memcpy(psB_d,psB_s,SizeB);

	m_psDataEnd	=	m_psData + (SizeFinal);
}

//---------------------------------------------------------
//	Data_Expand
//---------------------------------------------------------

//	action:		exands buffer to meet minmum size

void
cINI::Data_Expand(long MinSize)
{
	long	OldSize;
	long	NewSize;

	char*	psNew;

	// work out old size for copy later
	OldSize			=	(long)(m_psDataEnd - m_psData);

	// check we need to expand
	if (m_BufferSize > MinSize && MinSize != 0)
	{
		// no need to expand
		return;
	}

	// make new buffer
	NewSize			=	OldSize + INI_DATA_EXPAND;
	psNew			=	new char[NewSize];

	// copy old to new data
	if (m_psData)
	{
		memcpy(psNew,m_psData,OldSize);
		delete[] m_psData;
	}

	// keep scan relative to data
	m_psScan		+=	(long)(psNew - m_psData);

	// set new vars
	m_psData		=	psNew;
	m_psDataEnd		=	m_psData + OldSize;
	m_BufferSize	=	NewSize;
}

//---------------------------------------------------------
//	Global Instance
//---------------------------------------------------------

class	cINI	ini;


