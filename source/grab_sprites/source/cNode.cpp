#include "cNode.h"


//---------------------------------------------------------
//	Constructor / Destructor
//---------------------------------------------------------

cNode::cNode()
{
	m_pChild[0]		=	0;
	m_pChild[1]		=	0;
	m_XPos			=	0;
	m_YPos			=	0;
	m_Width			=	0;
	m_Height		=	0;
	m_bUsed			=	false;
}


cNode::~cNode()
{
	if (m_pChild[0])
	{
		delete m_pChild[0];
		m_pChild[0]	=	0;
	}

	if (m_pChild[1])
	{
		delete m_pChild[1];
		m_pChild[1]	=	0;
	}
}


//---------------------------------------------------------
//	SetPos
//---------------------------------------------------------

void
cNode::SetPos(long XPos,long YPos,long Width,long Height)
{
	m_XPos		=	XPos;
	m_YPos		=	YPos;
	m_Width		=	Width;
	m_Height	=	Height;
}

//---------------------------------------------------------
//	SetUsed
//---------------------------------------------------------

void
cNode::SetUsed(bool bUsed)
{
	m_bUsed		=	bUsed;
}

//---------------------------------------------------------
//	GetXPos
//---------------------------------------------------------

long
cNode::GetXPos()
{
	return m_XPos;
}

//---------------------------------------------------------
//	GetYPos
//---------------------------------------------------------

long
cNode::GetYPos()
{
	return m_YPos;
}

//---------------------------------------------------------
//	Insert
//---------------------------------------------------------

cNode*
cNode::Insert(long Width, long Height)
{
	cNode*	pNode;
	long	DiffW;
	long	DiffH;

	if (m_pChild[0] || m_pChild[1])
	{
		pNode		=	m_pChild[0]->Insert(Width,Height);
		if (pNode)
		{
			return pNode;
		}
		pNode		=	m_pChild[1]->Insert(Width,Height);
		return pNode;
	}

	if (m_bUsed)
	{
		return 0;
	}

	if (Width > m_Width || Height > m_Height)
	{
		return 0;
	}

	if (Width == m_Width && Height == m_Height)
	{
		m_bUsed		=	true;
		return this;
	}

	m_pChild[0]		=	new cNode;
	m_pChild[1]		=	new cNode;

	DiffW			=	m_Width - Width;
	DiffH			=	m_Height - Height;

	if (DiffW > DiffH)
	{
		m_pChild[0]->SetPos(m_XPos,m_YPos,Width,m_Height);
		m_pChild[1]->SetPos(m_XPos+Width,m_YPos,m_Width-Width,m_Height);
	}
	else
	{
		m_pChild[0]->SetPos(m_XPos,m_YPos,m_Width,Height);
		m_pChild[1]->SetPos(m_XPos,m_YPos+Height,m_Width,m_Height-Height);
	}

	pNode			=	Insert(Width,Height);
	return pNode;
}


