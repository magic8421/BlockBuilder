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
	// ��Ϊ���浯�����ڻἤ�� WM_ACTIVE ���Բ���Ҫ��ȷ�Ŀ���

	if (::ChooseColor(&cc)==TRUE) 
	{
		// D3D����ɫ˳���GDI�Ĳ�һ�� ò���Ǻ�����Ե�
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
		::MessageBox(g_hwndMain, L"����ɹ�", L"��ʾ", 0);
	}
	else
	{
		::MessageBox(g_hwndMain, L"����ʧ��", L"��ʾ", 0);
	}
}

void CToolBox::Load()
{
	if (g_chunk.LoadFromFile(L"data\\1.chk"))
	{
		::MessageBox(g_hwndMain, L"���سɹ�", L"��ʾ", 0);
		UpdateAllBlocks();
	}
	else
	{
		::MessageBox(g_hwndMain, L"����ʧ��", L"��ʾ", 0);
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
