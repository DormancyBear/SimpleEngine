#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#pragma once


#include <d3d11.h>
#include <d3dx10math.h>
#include "textureclass.h"


class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;	// 这个 character 在 font image 中占的像素宽度
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void* vertices, std::string sentence, float drawX, float drawY);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};

#endif