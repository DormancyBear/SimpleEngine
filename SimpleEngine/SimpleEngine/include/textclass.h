#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#pragma once


#include <memory>
#include "FontClass.h"
#include "FontShaderClass.h"


class TextClass
{
private:

	// hold the rendering information for each text sentence
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;

		~SentenceType()
		{
			// Release the sentence vertex buffer.
			if (vertexBuffer)
			{
				vertexBuffer->Release();
				vertexBuffer = nullptr;
			}

			// Release the sentence index buffer.
			if (indexBuffer)
			{
				indexBuffer->Release();
				indexBuffer = nullptr;
			}
		}
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(std::shared_ptr<SentenceType>& sentence, int maxLength, ID3D11Device* device);
	bool UpdateSentence(std::shared_ptr<SentenceType> sentence, std::string text, int positionX, int positionY, float red, float green, float blue,
		ID3D11DeviceContext* deviceContext);
	bool RenderSentence(ID3D11DeviceContext* deviceContext, std::shared_ptr<SentenceType> sentence, D3DXMATRIX worldMatrix,
		D3DXMATRIX orthoMatrix);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;
	std::shared_ptr<SentenceType> m_sentence1;
	std::shared_ptr<SentenceType> m_sentence2;
	std::shared_ptr<SentenceType> m_sentence3;
	std::shared_ptr<SentenceType> m_operationSentence;
};

#endif