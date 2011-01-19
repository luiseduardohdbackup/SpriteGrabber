#pragma once

#include "base.h"

#define INI_DATA_EXPAND		2048
#define INI_DUP_SECTION		-1

#define ASCII_LF			10
#define ASCII_CR			13
#define ASCII_DEL			127





class cINI
{
public:

	cINI();
	~cINI();

	// user functions -------------------------------------

	bool	Open(char* psFile,bool bSecureSave=false);
	bool	Open_AD(char* psFile,bool bSecureSave=false);



	bool	GetString(char* psSection,char* psKey,char* psDest,long DestSize,char* psDefault=0,bool bIncludeWS=false);
	bool	SetString(char* psSection,char* psKey,char* psValue);

	bool	GetValue(char* psSection,char* psKey,long& Value,long Default=0);
	bool	SetValue(char* psSection,char* psKey,long Value);

	long	GetLong(char* psSection,char* psKey,long Default=0);
	DWORD	GetHex(char* psSection,char* psKey,DWORD Default=0);

	bool	EnumSection(char* psSection,char* psDest,long DestSize);
	bool	EnumKey(char* psSection,char* psDest,long DestSize);

private:

	// scan functions -------------------------------------

	void	Scan_Reset();		// to start of buffer

	bool	Scan_isEOF();		// end of file
	bool	Scan_isEOL();		// end of line
	bool	Scan_isWS();		// white space

	bool	Scan_toValue();		// to value (from key)
	bool	Scan_toNextLine();	// to next line

	bool	Scan_isSection(char* psSection);
	bool	Scan_findSection(char* psSection,bool bFromStart=true);

	bool	Scan_isKey(char* psKey);
	bool	Scan_findKey(char* psKey);

	long	Scan_sizeValue(bool bIncludeWS=false);

	// data functions -------------------------------------

	void	Data_Save(bool bForceSave=false);
	long	Data_Size();
	void	Data_Free();

	void	Data_Append(char* psString);
	void	Data_Insert(char* psString,long SizeRemove);
	void	Data_Expand(long MinSize=0);

	// member variables -----------------------------------

	char	m_cFilename[512];	// filename

	char*	m_psData;			// data start
	char*	m_psDataEnd;		// data end
	long	m_BufferSize;		// buffer size
	char*	m_psScan;			// scan through data

	bool	m_bSecureSave;		// save on each write?
	bool	m_bChanged;			// has the file changed?
};

extern	class	cINI	ini;


