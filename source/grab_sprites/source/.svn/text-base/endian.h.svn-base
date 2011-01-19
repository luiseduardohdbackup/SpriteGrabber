#pragma once

//	#define __BIG_ENDIAN__ if non-intel


#include "base.h"

#ifdef __BIG_ENDIAN__

#define	ENDIAN_WORD(v)				v = ENDIAN_flip_word(v)
#define	ENDIAN_DWORD(v)				v = ENDIAN_flip_dword(v)

#define	ENDIAN_WORD_ARRAY(p,s)		ENDIAN_flip_word_array(p,s)
#define	ENDIAN_DWORD_ARRAY(p,s)		ENDIAN_flip_dword_array(p,s)

#else

#define	ENDIAN_WORD(a)
#define	ENDIAN_DWORD(a)

#define	ENDIAN_WORD_ARRAY(a,s)
#define	ENDIAN_DWORD_ARRAY(a,s)

#endif


extern	WORD	ENDIAN_flip_word(WORD Value);
extern	DWORD	ENDIAN_flip_dword(DWORD Value);

extern	void	ENDIAN_flip_word_array(WORD* pArray,long ArraySize);
extern	void	ENDIAN_flip_dword_array(DWORD* pArray,long ArraySize);


