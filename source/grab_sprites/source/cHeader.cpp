#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "cHeader.h"



//---------------------------------------------------------
//	DISABLE CRT WARNINGS
//---------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4996)

//---------------------------------------------------------
//	Constructor / Destructor
//---------------------------------------------------------

cHeader::cHeader()
{
	m_psData	=	new char[HEADER_CHUNK_SIZE];
	m_DataSize	=	HEADER_CHUNK_SIZE;
	m_DataUsed	=	0;
	m_psAppend	=	m_psData;
}

cHeader::~cHeader()
{
	if (m_psData)
	{
		delete[] m_psData;
	}
}



//---------------------------------------------------------
//	Open
//---------------------------------------------------------

void
cHeader::Open(char *psFile)
{
	if (m_psData)
	{
		delete[] m_psData;
	}

	m_DataSize		=	FILE_size(psFile);
	m_DataUsed		=	m_DataSize;

	if (m_DataSize == 0)
	{
		m_psData	=	0;
		m_psRead	=	0;
		m_psAppend	=	0;
	}
	else
	{
		m_DataSize++;

		m_psData	=	new char[m_DataSize];
		m_psRead	=	m_psData;
		m_psAppend	=	m_psData + m_DataUsed;
		*m_psAppend	=	0;

		FILE_open(psFile,m_psData);
	}
}

//---------------------------------------------------------
//	ReadReset
//---------------------------------------------------------

void
cHeader::ReadReset()
{
	m_psRead	=	m_psData;
}

//---------------------------------------------------------
//	ReadValue
//---------------------------------------------------------

bool
cHeader::ReadValue(char* psDest,long DestSize,long& Value)
{
	char	cValue[16]; 
//	char*	ps;
	long	i; 

	if (m_psRead == 0 || m_psRead >= m_psAppend)
	{
		return false;
	}

	m_psRead		=	strstr(m_psRead,"#define");
	if (m_psRead == 0)
	{
		return false;
	}

	m_psRead		+=	7;
	while (*m_psRead == ' ')
	{
		m_psRead++;
	}

	i				=	0;
	while (m_psRead[i] > ' ' && i < DestSize)
	{
		psDest[i]	=	m_psRead[i];
		i++;
	}
	if (i == DestSize)
	{
		return false;
	}
	psDest[i]		=	0;
	m_psRead		+=	i;

	while (*m_psRead == ' ')
	{
		m_psRead++;
	}

	i				=	0;
	while (m_psRead[i] >= '0' && m_psRead[i] <= '9' && i < 15)
	{
		cValue[i]	=	m_psRead[i];
		i++;
	}
	if (i == 0)
	{
		return false;
	}
	cValue[i]		=	0;

	Value			=	strtol(cValue,0,10);
	return true;
}






//---------------------------------------------------------
//	Append
//---------------------------------------------------------

void
cHeader::Append(char *psData, long DataSize)
{
	if (DataSize == 0)
	{
		DataSize	=	(long)strlen(psData);
	}

	GrowTo(m_DataUsed + DataSize);

	memcpy(m_psAppend,psData,DataSize);

	m_psAppend		+=	DataSize;
	m_DataUsed		+=	DataSize;
}

//---------------------------------------------------------
//	DefineValue
//---------------------------------------------------------

void
cHeader::DefineValue(char* psDefine,long Value)
{
	char	cLine[512];
	char	cValue[16];
	long	i,e;

	strcpy(cLine,"#define ");
	strcat(cLine,psDefine);

	sprintf(cValue,"%i",Value);

	i				=	(long)strlen(cLine);
	e				=	60;
	e				-=	(long)strlen(cValue);

	do
	{
		cLine[i++]	=	' ';
	}
	while (i < e);
	cLine[i]		=	0;

	strcat(cLine,cValue);
	strcat(cLine,"\n");

	Append(cLine);
}


//---------------------------------------------------------
//	Save
//---------------------------------------------------------

bool
cHeader::Save(char* psFile,bool bForce)
{
	char*	psData;
	long	i;

	if (bForce == false)
	{
		if (m_DataUsed && (m_DataUsed == FILE_size(psFile)))
		{
			psData	=	(char*)FILE_open(psFile);
			for (i=0; i<m_DataUsed; i++)
			{
				if (psData[i] != m_psData[i])
				{
					delete[] psData;
					goto SAVE;
				}
			}
			delete[] psData;
			return false;
		}
	}

SAVE:

	return FILE_save(psFile,m_psData,m_DataUsed);
}

//---------------------------------------------------------
//	GrowTo
//---------------------------------------------------------

void
cHeader::GrowTo(long DataSize)
{
	char*	psData;

	if (DataSize < m_DataSize)
	{
		return;
	}

	psData		=	new char[DataSize + HEADER_CHUNK_SIZE];

	memcpy(psData,m_psData,m_DataUsed);

	delete[] m_psData;

	m_psData	=	psData;
	m_DataSize	=	DataSize + HEADER_CHUNK_SIZE;
	m_psAppend	=	psData + m_DataUsed;
}




//---------------------------------------------------------
//	DEFAULT CRT WARNINGS
//---------------------------------------------------------

#pragma warning(pop)
