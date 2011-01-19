#pragma once

//---------------------------------------------------------
//	Class
//---------------------------------------------------------

class cNode
{
public:

			cNode::cNode();
			cNode::~cNode();

	void	SetPos(long XPos,long YPos,long Width,long Height);
	void	SetUsed(bool bUsed=true);

	long	GetXPos();
	long	GetYPos();

	cNode*	Insert(long Width,long Height);

	cNode*	m_pChild[2];
	long	m_XPos;
	long	m_YPos;
	long	m_Width;
	long	m_Height;

	bool	m_bUsed;
};
