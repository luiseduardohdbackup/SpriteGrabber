#pragma once

#define HEADER_CHUNK_SIZE	1024

class cHeader
{
public:

			cHeader::cHeader();
			cHeader::~cHeader();

	void	Open(char* psFile);
	void	ReadReset();
	bool	ReadValue(char* psDest,long DestSize,long& Value);

	void	Append(char* psData,long DataSize=0);
	void	DefineValue(char* psDefine,long Value);


	bool	Save(char* psFile,bool bForce=false);

private:

	void	GrowTo(long DataSize);

	char*	m_psData;
	char*	m_psRead;
	long	m_DataSize;
	long	m_DataUsed;
	char*	m_psAppend;
};

