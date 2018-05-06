#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "ShaderManagerClass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textclass.h"


const bool FULL_SCREEN = false;	// 全屏模式下无法调试 Orz
const bool VSYNC_ENABLED = false;	// 垂直同步, 限制在60帧
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, int, int, float);

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ShaderManagerClass* m_ShaderManager;
	LightClass* m_Light;
	TextClass* m_Text;
};

#endif