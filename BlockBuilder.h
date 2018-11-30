#pragma once

#include "resource.h"

enum EKeyStat {
	eksFoward,
	eksBack,
	eksLeft,
	eksRight,
	eksUp,
	eksDown,
	eksBreak,
	eksDelete,
	eksColor,
	eksFill,
	eksMax
};

extern HWND g_hwndMain;
extern BOOL g_keyStat[eksMax];
extern DWORD g_screenWidth;
extern DWORD g_screenHeight;
extern LPDIRECT3D9 g_d3d;
extern LPDIRECT3DDEVICE9 g_d3ddev;
