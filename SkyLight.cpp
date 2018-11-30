#include "StdAfx.h"
#include "SkyLight.h"
#include "BlockBuilder.h"

CSkyLight::CSkyLight(void)
{
}

CSkyLight::~CSkyLight(void)
{
}
	
void CSkyLight::Update()
{
	// 学习麦块风格 6个面 6个灯 这样每个面都不会黑 以后通过修改法线"平滑"光照效果也好做
    D3DLIGHT9 light;    
    ZeroMemory(&light, sizeof(light));  
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);    // set the light's color
    light.Direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    g_d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
    g_d3ddev->LightEnable(0, TRUE);    // turn on light #0
	
    light.Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f); 
    light.Direction = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    g_d3ddev->SetLight(1, &light);   
    g_d3ddev->LightEnable(1, TRUE);   

	light.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f); 
	light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	g_d3ddev->SetLight(2, &light);   
	g_d3ddev->LightEnable(2, TRUE);   

	light.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f); 
	light.Direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	g_d3ddev->SetLight(3, &light);   
	g_d3ddev->LightEnable(3, TRUE);   

	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
	light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	g_d3ddev->SetLight(4, &light);   
	g_d3ddev->LightEnable(4, TRUE);   

	light.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f); 
	light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	g_d3ddev->SetLight(5, &light);   
	g_d3ddev->LightEnable(5, TRUE);   
}
