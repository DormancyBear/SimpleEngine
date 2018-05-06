#pragma once

#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_


#include "d3dclass.h"
#include "lightshaderclass.h"


// facade pattern
// 每个 Shader 本身各有一个类, 但外部访问 Shader 统统通过这个门面类, 而不直接包含那个 Shader 类
class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderLightShader(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*,
		D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float);

private:
	LightShaderClass* m_LightShader;
};


#endif