// include the basic windows header files and the Direct3D header file

#include "stdafx.h"
#include "BlockBuilder.h"
#include "camera.h"
#include "chunk.h"
#include "block.h"
#include "ColorVertices.h"
#include "SelectBlock.h"
#include "SelectVertices.h"
#include "SkyLight.h"
#include "ToolBox.h"
#include "Utility.h"

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")


// global declarations
LPDIRECT3D9 g_d3d;
LPDIRECT3DDEVICE9 g_d3ddev;
HWND g_hwndMain;
BOOL g_keyStat[eksMax];
DWORD g_keyBind[] = {'W', 'S', 'A', 'D', VK_SPACE, VK_SHIFT, '1', '2', '3', '4'};
DWORD g_screenWidth, g_screenHeight;
CChunk g_chunk;
CColorVertices g_blockVertices;
CCamera g_camera;
CSelectBlock g_select(&g_camera);
CSelectVertices g_selectVertices;
CSkyLight g_skyLight;
LPDIRECT3DTEXTURE9 g_texture;
DWORD g_color;
CToolBox g_toolbox;

// function prototypes
void InitD3D(HWND hWnd);
void RenderFrame(DWORD deltaTicks);
void CleanD3D(void);
void InitGraphics(void);
void InitLight(void);    // sets up the light and the material
void UpdateScene();
void UpdateAllBlocks();


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"DXHWND";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"DXHWND", L"块块世界 1.5",
                          WS_CAPTION|WS_SYSMENU, 0, 0, 1350, 700,
                          NULL, NULL, hInstance, NULL);
	g_hwndMain = hWnd;
    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    InitD3D(hWnd);

    InitDebugFont();
    
    // enter the main loop:

    MSG msg;
    DWORD prevTick = GetTickCount();

    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;
		// 限制帧数
		const float constFps = 60.0f;
		float timeInOneFps = 1000.0f/constFps;
		DWORD timeBegin = GetTickCount();

		UpdateScene();
		RenderFrame(timeBegin - prevTick);
		
		DWORD timeSpend = GetTickCount() - timeBegin;
        if (timeSpend < timeInOneFps) {
            Sleep(DWORD(timeInOneFps - timeSpend));
        }
        prevTick = GetTickCount();
    }

    FpsCounter::Free();
    // clean up DirectX and COM
    CleanD3D();
    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
    switch(message)
    {
		case WM_LBUTTONDOWN:
			g_toolbox.RemoveBlock();
			break;
		case WM_RBUTTONDOWN:
			g_toolbox.AddBlock();
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
			for (i = 0; i < eksMax; i ++)
			{
				if (wParam == g_keyBind[i])
				{
					g_keyStat[i] = message == WM_KEYDOWN;
				}
			}
			if (wParam == VK_ESCAPE)
			{
				g_camera.SetCaptureMouse(FALSE);
				// DestroyWindow(hWnd);
			}
			if (wParam == '1')
			{
				g_toolbox.ChooseColor();
			}
			if (wParam == '2')
			{
				g_toolbox.StrawColor();
			}
			if (wParam == 'Y')
			{
				g_toolbox.Save();
			}
			if (wParam == 'U')
			{
				g_toolbox.Load();
			}
			return 0;
		case WM_ACTIVATE:
			g_camera.SetCaptureMouse(wParam != 0);
			return 0;
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	g_screenWidth = rect.right;
	g_screenHeight = rect.bottom;
	
    g_d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = g_screenWidth;
    d3dpp.BackBufferHeight = g_screenHeight;
    d3dpp.EnableAutoDepthStencil = TRUE;
    //d3dpp.AutoDepthStencilFormat = D3DFMT_D16; 
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; //这个比16的精度好很多 条纹的问题解决了

    // create a device class using this information and the info from the d3dpp stuct
    HRESULT hr = g_d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &g_d3ddev);
	if (FAILED(hr))
	{
		_com_error err(hr);
		MessageBox(hWnd, err.ErrorMessage(), L"CreateDevice", 0);
		PostQuitMessage(0);
	}

    InitGraphics();    // call the function to initialize the cube
    InitLight();    // call the function to initialize the light and material

    g_d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
}

void DrawRect(IDirect3DDevice9* dev, int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT BarRect = { x, y, x + w, y + h }; 
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 0,  0); 
} 

void DrawCrosshairs()
{
	POINT center;
	center.x = g_screenWidth / 2;
	center.y = g_screenHeight / 2;
    DrawRect(g_d3ddev, center.x - 10, center.y, 22, 2, g_color);
    DrawRect(g_d3ddev, center.x, center.y - 10, 2, 22, g_color); 
}

void UpdateAllBlocks()
{
    //g_chunk.TraverseBlocks(CChunk::CalcGeometry, 0);
    int nBlocks = g_chunk.GetRenderCount();
    //g_chunk.TraverseBlocks(CChunk::GetRenderCount, &nBlocks);
    g_blockVertices.CreateBuffer(nBlocks);
    g_blockVertices.GenerateVertices(g_chunk);
}

void UpdateScene()
{
	g_select.SelectBlockFromView(g_chunk);

	// 画选择线框 
	if (g_select.GetSelect())
	{
		// TODO 想做一个"反色"的线框 可是效果不怎么好
		DWORD color = g_select.GetSelect()->GetColor();
		int r = (color & 0x00ff0000) >> 16;
		int g = (color & 0x0000ff00) >> 8;
		int b = color & 0x000000ff;
		color = D3DCOLOR_XRGB(r + 85, g + 85, b + 85);
		g_selectVertices.GenerateVertices(g_select.GetPos(), g_select.GetFace(), color);
	}
}

// this is the function used to render a single frame
void RenderFrame(DWORD deltaTicks)
{
    g_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    g_d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_d3ddev->BeginScene();
    
    // select which vertex format we are using

    // set the view transform
    D3DXMATRIX matView;
    if (g_camera.GetCaptureMouse())
    {
		g_camera.MoveCamera(deltaTicks);
		g_camera.RotateCamera();
	}
    D3DXMatrixLookAtLH(&matView,
		g_camera.GetCameraVector(),    // the camera position
		g_camera.GetViewVector(),      // the look-at position
		&D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction
    g_d3ddev->SetTransform(D3DTS_VIEW, &matView);

    // set the projection transform
    D3DXMATRIX matProjection;
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),
                               (float)g_screenWidth / (float)g_screenHeight,
                               0.1f,    // the near view-plane
                               128.0f);    // the far view-plane
    g_d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

    //// set the world transform
    //static float index = 0.0f; index+=0.03f;
    //D3DXMATRIX matRotateY;
    //D3DXMatrixRotationY(&matRotateY, index);
    //g_d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));

	//g_skyLight.Update();
	
	g_blockVertices.Draw();
	
	if (g_select.GetSelect())
	{
		// 让线框透过方块显示
		g_d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		g_selectVertices.Draw();
	}
    DrawCrosshairs();
    
    FpsCounter::Instance()->Feed();
    wchar_t msg[64];
    _snwprintf_s(msg, ARRAYSIZE(msg), L"FPS:%d", FpsCounter::Instance()->Get());
    DrawDebugText(600, 0, 0xff4444ff, msg);
    RewindDebugPrint();

    g_d3ddev->EndScene(); 
    g_d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    g_d3ddev->Release();
    g_d3d->Release();
}

// this is the function that puts the 3D models into video RAM
void InitGraphics(void)
{
	g_color = D3DCOLOR_XRGB(0, 0, 255);
    // Test
    g_chunk.GenBlocks();
	g_chunk.LoadFromFile(L"data\\1.chk");
    g_blockVertices.CreateIndexBuffer();
	g_selectVertices.CreateBuffer();
	g_selectVertices.CreateIndexBuffer();

	
// 	if (FAILED(D3DXCreateTextureFromFile(g_d3ddev, L"cobblestone.png", &g_texture)))
// 	{
// 		_RPT0(_CRT_ERROR, "D3DXCreateTextureFromFile FAILED\r\n");
// 	}
	
	UpdateAllBlocks();
}

// this is the function that sets up the lights and materials
void InitLight(void)
{
	g_skyLight.Update();
	
    D3DMATERIAL9 material;    // create the material struct

    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white

    g_d3ddev->SetMaterial(&material);    // set the globably-used material to &material
}