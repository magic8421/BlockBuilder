#include "stdafx.h"
#include "ToolBox.h"
#include "SelectBlock.h"
#include "Block.h"
#include "Chunk.h"
#include "Camera.h"

extern DWORD g_color;
extern HWND g_hwndMain;
extern CSelectBlock g_select;
extern CChunk g_chunk;
extern CCamera g_camera;

extern void UpdateAllBlocks();

void CToolBox::ChooseColor()
{
	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 

	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = g_hwndMain;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(GetBValue(g_color), GetGValue(g_color), GetRValue(g_color));
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	//g_camera.SetCaptureMouse(FALSE);
	// 因为下面弹出窗口会激发 WM_ACTIVE 所以不需要明确的开关

	if (::ChooseColor(&cc)==TRUE) 
	{
		// D3D的颜色顺序和GDI的不一样 貌似是红和蓝对调
		g_color =  D3DCOLOR_XRGB(GetRValue(cc.rgbResult), GetGValue(cc.rgbResult), GetBValue(cc.rgbResult)); 
	}
	//g_camera.SetCaptureMouse(TRUE);
}

void CToolBox::StrawColor()
{
	if (g_select.GetSelect() && g_select.GetSelect()->GetType() == CBlock::Colored)
	{
		g_color = g_select.GetSelect()->GetColor();
	}
}

void CToolBox::Save()
{
	if (g_chunk.SaveToFile(L"data\\1.chk"))
	{
		::MessageBox(g_hwndMain, L"保存成功", L"提示", 0);
	}
	else
	{
		::MessageBox(g_hwndMain, L"保存失败", L"提示", 0);
	}
}

void CToolBox::Load()
{
	if (g_chunk.LoadFromFile(L"data\\1.chk"))
	{
		::MessageBox(g_hwndMain, L"加载成功", L"提示", 0);
		UpdateAllBlocks();
	}
	else
	{
		::MessageBox(g_hwndMain, L"加载失败", L"提示", 0);
	}
}

void CToolBox::RemoveBlock()
{
	g_camera.SetCaptureMouse(TRUE);

	if (g_select.GetSelect())
	{
		int x, y, z;
		g_select.GetPos(x, y, z);
		g_chunk.RemoveBlock(x, y, z);
		UpdateAllBlocks();
	}
}

void CToolBox::AddBlock()
{
	int x, y, z;
	g_select.SelectBlockFromView(g_chunk);
	if (g_select.GetSelect())
	{
		g_select.GetPos(x, y, z);
		g_chunk.AddBlock(x, y, z, g_select.GetFace(), g_color);
		UpdateAllBlocks();
	}
}
