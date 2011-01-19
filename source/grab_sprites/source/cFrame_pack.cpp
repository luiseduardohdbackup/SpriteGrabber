#include "cBitmap.h"
#include "cFrame.h"
#include "cNode.h"


long	cFrame::s_PackWidth;
long	cFrame::s_PackHeight;
long	cFrame::s_PackBorder;


//---------------------------------------------------------
//	Pack_TrySize
//---------------------------------------------------------

bool
cFrame::Pack_TrySize(long Pixels,long Method)
{
	long	w,h;
	cNode	Node;
	cNode*	pNode;
	cFrame*	pFrame;

	// quick check for size too small -----------

	if (Pixels > (s_PackWidth * s_PackHeight))
	{
		return false;
	}

	// insert all frames ------------------------


	Node.SetPos(0,0,s_PackWidth+s_PackBorder,s_PackHeight+s_PackBorder);

						cFrame::Sort(Method);
	pFrame			=	cFrame::GetFirst();
	while (pFrame)
	{
		w			=	pFrame->GetWidth();
		h			=	pFrame->GetHeight();

		// only pack non-empty frames -----------

		if (w)
		{
			pNode	=	Node.Insert(w+s_PackBorder,h+s_PackBorder);
			if (pNode == 0)
			{
				return false;
			}
			pFrame->SetPos(pNode->GetXPos(),pNode->GetYPos());
		}

		pFrame		=	pFrame->m_pNext;
	}

	// all done - passed OK ---------------------

	return true;
}

//---------------------------------------------------------
//	Pack_All
//---------------------------------------------------------

bool
cFrame::Pack_All(long Border)
{
	long	Pixels;
	long	Method;

	Pixels		=	GetPixelsUsed();

	// go through and check all dest sizes ------

	s_PackWidth				=	64;
	s_PackHeight			=	64;
	s_PackBorder			=	Border;
	if (s_PackBorder < 0)
	{
		s_PackBorder		=	0;
	}
	else if (s_PackBorder > 2)
	{
		s_PackBorder		=	2;
	}

	Method					=	0;

	while (false == Pack_TrySize(Pixels,Method))
	{
		Method++;
		if (Method < 4)
		{
			continue;
		}
		Method				=	0;

		// @Newtoy - change the packing logic to always pack in squares
		s_PackHeight	<<=	1;
		s_PackWidth		<<=	1;
	}

#ifdef VER_DEMO

	if (s_PackWidth > 256)
	{
		printf("ERROR   : TEXTURE ATLAS TOO BIG - PLEASE UPGRADE LICENSE\n");
		return false;
	}

#endif

	return true;
}


