#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


#include "d3dclass.h"
#include "cameraclass.h"
#include "ModelClass.h"
#include "ShaderManagerClass.h"
#include "DirectionalLightClass.h"
#include "PointLightClass.h"
#include "RenderTextureClass.h"
#include "bitmapclass.h"
#include "textclass.h"


const bool FULL_SCREEN = false;	// 全屏模式下无法调试 Orz
const bool VSYNC_ENABLED = false;	// 垂直同步, 限制在60帧
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime,
		float posX, float posY, float posZ, float rotX, float rotY, float rotZ);

private:
	bool RenderSceneToTexture();
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass *m_HumanModel, *m_GroundModel;
	ShaderManagerClass* m_ShaderManager;
	RenderTextureClass* m_RenderTexture;
	DirectionalLightClass* m_DirectionalLight;
	PointLightClass *m_PointLight;
	TextClass* m_Text;
};

#endif