#include "endian.h"

WORD
ENDIAN_flip_word(WORD Value)
{
	WORD	v1,v2;

	v1		=	Value >> 8;
	v2		=	Value << 8;

	return v1 + v2;
}


DWORD
ENDIAN_flip_dword(DWORD Value)
{
	DWORD	v1,v2,v3,v4;
	
	v1		=	Value >> 24;
	v2		=	(Value & 0x00FF0000) >> 8;
	v3		=	(Value & 0x0000FF00) << 8;
	v4		=	Value << 24;

	return v1 + v2 + v3 + v4;
}


void
ENDIAN_flip_word_array(WORD* pArray,long ArraySize)
{
	WORD*	p;
	long	i;
	
	p		=	pArray;
	i		=	ArraySize;
	
	while (i)
	{
		*p	=	ENDIAN_flip_word(*p);
		p++;
		i--;
	}
}


void
ENDIAN_flip_dword_array(DWORD* pArray,long ArraySize)
{
	DWORD*	p;
	long	i;
	
	p		=	pArray;
	i		=	ArraySize;
	
	while (i)
	{
		*p	=	ENDIAN_flip_dword(*p);
		p++;
		i--;
	}
}





