#include "stdafx.h"
#include "Player.h"


D3DXVECTOR3 CPlayer::GetFeetPosition()
{
	D3DXVECTOR3 pos(m_headPos);
	pos.y -= 3.0f;	
	return pos;
}
