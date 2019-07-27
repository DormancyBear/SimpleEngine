#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


#include <Graphics/DirectXPlatformManager.h>
#include <Graphics/Camera.h>
#include "ModelClass.h"
#include "ShaderManagerClass.h"
#include "DirectionalLightClass.h"
#include "PointLightClass.h"
#include "RenderTextureClass.h"
#include "TextClass.h"


const bool FULL_SCREEN = false;	// х╚фадёй╫
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
	Camera* m_Camera{ nullptr };
	//ModelClass *m_HumanModel, *m_GroundModel;
	ShaderManagerClass* m_ShaderManager;
	RenderTextureClass* m_RenderTexture;
	DirectionalLightClass* m_DirectionalLight;
	PointLightClass *m_PointLight;
	//TextClass* m_Text;
};

#endif