#include <math.h>
#include "maths.h"
#include "base.h"


float	MATH_sintab_f[MATH_SINTAB_SIZE];
long	MATH_sintab_l[MATH_SINTAB_SIZE];
long	MATH_atan32[0x1000];


void
MATH_init_once()
{
	long	i;
	long	j;
	double	r;
	float	fAngle;

	for (i=0; i<MATH_SINTAB_SIZE; i++)
	{
		fAngle				=	(float)(i - (MATH_SINTAB_SIZE/4) & (MATH_SINTAB_SIZE-1));
		fAngle				*=	(float)MATH_PI;
		fAngle				/=	(MATH_SINTAB_SIZE/2);

		MATH_sintab_f[i]	=	cos(fAngle);
	}
	
	MATH_sintab_f[0]					=	0.0f;
	MATH_sintab_f[MATH_SINTAB_SIZE/2]	=	0.0f;


	for (i=0; i<MATH_SINTAB_SIZE; i++)
	{
		MATH_sintab_l[i]	=	(long)(MATH_sintab_f[i] * 65536.0f);
	}

	for (i=0; i<0x10000; i+=0x10)
	{
		r					=	atan2((double)(i+8),(double)0x10000);	// i+(step_size>>1)
		r					*=	(180/3.14159265359);					// radians->degrees
		r					*=	0x020000000;							// 45' = 0x20000000
		r					/=	45;
		j					=	(long)r;

		MATH_atan32[i>>4]	=	j;
	}
}





long
MATH_mult(long v1,long v2)
{
	QWORD	r;

	r		=	(QWORD)v1 * (QWORD)v2;
	r		>>=	16;

	return (long)r;
}

DWORD
MATH_angle(long x,long y)
{
	bool	bInvX;
	bool	bInvY;
	long	i;
	DWORD	Angle;

	bInvX			=	false;
	bInvY			=	false;
	if (x < 0)
	{
		x			=	0 - x;
		bInvX		=	true;
	}
	if (y < 0)
	{
		y			=	0 - y;
		bInvY		=	true;
	}
	if ((x | y) & 0xFFF00000)
	{
		x			>>=	12;
		y			>>=	12;
	}

	if (y > x)
	{
		i			=	(x << 12) / y;
		Angle		=	MATH_atan32[i];
	}
	else if (x == y)
	{
		Angle		=	0x20000000;
	}
	else
	{
		i			=	(y << 12) / x;
		Angle		=	0x40000000 - MATH_atan32[i];
	}

	if (!bInvY)
	{
		Angle		=	0x80000000 - Angle;
	}
	if (bInvX)
	{
		Angle		=	0 - Angle;
	}

	return Angle;
}



