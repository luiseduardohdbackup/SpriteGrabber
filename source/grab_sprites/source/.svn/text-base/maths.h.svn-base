#pragma once

#include "base.h"


#define MATH_PI					3.14159265359

#define MATH_SINTAB_BITS		12
#define MATH_SINTAB_SIZE		(1 << MATH_SINTAB_BITS)
#define MATH_SINTAB_SHIFT		(32 - MATH_SINTAB_BITS)

extern	float	MATH_sintab_f[MATH_SINTAB_SIZE];
extern	long	MATH_sintab_l[MATH_SINTAB_SIZE];

extern	void	MATH_init_once();

extern	long	MATH_mult(long v1,long v2);
extern	DWORD	MATH_angle(long vX,long vY);


