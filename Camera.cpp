#include "StdAfx.h"
#include "Camera.h"
#include "BlockBuilder.h"
#include "block.h"
#include "Chunk.h"
#include "Utility.h"

extern CChunk g_chunk;

CCamera::CCamera(void):
	m_vecCamera(40.0f, 40.0f, 40.0f),
	m_vecLookAt(0.0f, 0.0f, 0.0f),
	m_speed(0.0f, 0.0f, 0.0f),
	m_bActiveWnd(FALSE)
{
	m_viewHoriz = D3DXToRadian(200.0f);
	m_viewVerit = D3DXToRadian(115.0f);
}

CCamera::~CCamera(void)
{
}

const D3DXVECTOR3 *CCamera::GetViewVector()
{
	float radius = 10.0f;
	m_vecLookAt.y = m_vecCamera.y + radius * cosf(m_viewVerit);
	radius = radius * sinf(m_viewVerit);
	m_vecLookAt.x = m_vecCamera.x + radius * cosf(m_viewHoriz);
	m_vecLookAt.z = m_vecCamera.z + radius * sinf(m_viewHoriz);
	// 不能垂直向上看的问题解决了 但是还有些边界量没处理好
	return &m_vecLookAt;
}

const D3DXVECTOR3 *CCamera::GetCameraVector()
{
	return &m_vecCamera;
}

void CCamera::MoveCamera()
{
    PrintOnScreen(L"deltaTicks: %f", TimeManager::Instance().GetTickDelta());
    const float accelerate = 0.1f * TimeManager::Instance().GetTickDelta();
	float radian = m_viewHoriz;
	// 应该用速度向量 减速的时候是求速度向量的模 然后减去加速度 乘上速度的单位向量

	
	// 摄像机
	if (g_keyStat[eksRight])
	{
		m_speed.z -= accelerate * cosf(radian);
		m_speed.x += accelerate * sinf(radian);
	}
	else if (g_keyStat[eksLeft])
	{
		m_speed.z += accelerate * cosf(radian);
		m_speed.x -= accelerate * sinf(radian);
	}
	if (g_keyStat[eksFoward])
	{
		m_speed.x += accelerate * cosf(radian);
		m_speed.z += accelerate * sinf(radian);
	}
	else if (g_keyStat[eksBack])
	{
		m_speed.x -= accelerate * cosf(radian);
		m_speed.z -= accelerate * sinf(radian);

	}
	if (g_keyStat[eksUp])
	{
		m_speed.y += accelerate;
		//m_vecLookAt.y += accelerate;
	}
	else if (g_keyStat[eksDown])
	{
		m_speed.y -= accelerate;	
		//m_vecLookAt.y -= accelerate;
	}

    PrintOnScreen(L"speed: %f %f %f\r\n", m_speed.x, m_speed.y, m_speed.z);

    // TODO 限制最大速度
    float speed_mod = ModVector3(m_speed);
    const float SPEED_LIMIT = 0.4f;
    if (speed_mod > SPEED_LIMIT) {
        m_speed *= SPEED_LIMIT / speed_mod;
    }

	BOOL bPress = FALSE;
	for (int i = 0; i < 6; i ++)
	{
		if (g_keyStat[i])
		{
			bPress = TRUE;
			break;
		}
	}
	if (!bPress)
	{
		//float mod = sqrtf(m_speed.x * m_speed.x + m_speed.y * m_speed.y + m_speed.z * m_speed.z);
		if (fabs(speed_mod - 0.0f) > 0.0001)
		{
			D3DXVECTOR3 unit = m_speed / speed_mod;
			speed_mod -= accelerate;
			if (speed_mod < 0.0f)
				speed_mod = 0.0f;
			m_speed = unit * speed_mod;
		}
	}
	
	const float BORDER = 0.4f;
	D3DXVECTOR3 pos[8];
	for(int i = 0; i < 8; i ++)
	{
		pos[i] = m_vecCamera + m_speed;

		if (i & 1)
		{
			pos[i].x += BORDER;
		}
		else
		{
			pos[i].x -= BORDER;
		}

		if (i & 2)
		{
			pos[i].y += BORDER;	
		}
		else
		{
			pos[i].y -= BORDER * 3.0f;
		}
		
		if (i & 4)
		{
			pos[i].z += BORDER;	
		}
		else
		{
			pos[i].z -= BORDER;	
		}
		
	}

	bool bMoveable = true;

	for(int i = 0; i < 8; i ++)
	{
		if (!g_chunk.CollisionTest(pos[i], m_speed, i))
		{
			bMoveable = false;
			break;;
		}
	}
// 	if (bMoveable)
// 	{
// 		m_vecCamera += m_speed;
// 	}
// 	else
// 	{
// 		m_speed.x = 0.0f;
// 		m_speed.y = 0.0f;
// 		m_speed.z = 0.0f;
// 	}

	m_vecCamera += m_speed;

	GetViewVector();
}

POINT CCamera::GetViewCenter()
{
	POINT center;
	RECT rect;
	GetClientRect(g_hwndMain, &rect);
	ClientToScreen(g_hwndMain, (POINT *)&rect);
	ClientToScreen(g_hwndMain, (POINT *)(&rect) + 1);
	center.x = rect.left + (rect.right - rect.left) / 2;
	center.y = rect.top + (rect.bottom - rect.top) / 2;
	
	return center;
}

void CCamera::RotateCamera()
{
	POINT pt, center;
	GetCursorPos(&pt);
	center = GetViewCenter();

	const float step = 0.002f;
	m_viewHoriz -= (pt.x - center.x) * step;
	
	m_viewVerit += (pt.y - center.y) * step;
	if (m_viewVerit <= 0.01f)
		m_viewVerit = 0.01f;
	else if (m_viewVerit >= D3DXToRadian(175.0f))
		m_viewVerit = D3DXToRadian(175.0f);
		
	GetViewVector();
	SetCursorPos(center.x, center.y);
}

void CCamera::SetCaptureMouse(BOOL bEnable)
{
	m_bActiveWnd = bEnable;
	
	if (bEnable)
	{
		// 隐藏鼠标
		POINT pt = GetViewCenter();
		::SetCursorPos(pt.x, pt.y);
		// http://blogs.msdn.com/b/oldnewthing/archive/2009/12/17/9937972.aspx
		while (::ShowCursor(FALSE) >= 0) {}
	}
	else
	{
		while (::ShowCursor(TRUE) < 0) {}
	}
}

BOOL CCamera::GetCaptureMouse()
{
	return m_bActiveWnd;
}
