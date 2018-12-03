#pragma once

#include <limits>
class CBlock;


class CCamera
{
public:
	CCamera(void);
	~CCamera(void);
	
    void MoveCamera(DWORD deltaTicks);
	void RotateCamera();
	const D3DXVECTOR3 *GetCameraVector();
	const D3DXVECTOR3 *GetViewVector();
	POINT GetViewCenter();
	void SetCaptureMouse(BOOL bEnable);
	BOOL GetCaptureMouse();
	
	static void SelectBlockFromView(CBlock *block, int index, int level, void *udata);

private:
	D3DXVECTOR3 m_vecCamera;
	D3DXVECTOR3 m_vecLookAt;
	float m_viewHoriz;
	float m_viewVerit;
	D3DXVECTOR3 m_speed;
	BOOL m_bActiveWnd;
};
