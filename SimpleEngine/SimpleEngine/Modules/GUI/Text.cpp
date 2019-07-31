#include <string>

#include <GUI/Text.h>
#include <NativePlatform/NativePlatform.h>
#include <GUI/GUIManager.h>

namespace SimpleEngine
{
	namespace UI
	{
		Text::Text()
		{
			// Create the font object.
			m_Font = new FontClass;
			// Initialize the font object.
			m_Font->Initialize("./media/Fonts/fontdata.txt", L"./media/Fonts/font.dds");

			// Create the font shader object.
			m_FontShader = new FontShaderClass;
			// Initialize the font shader object.
			m_FontShader->Initialize(DirectXPlatformManager::Instance().GetDevice(), NativePlatform::Instance().GetWindowHandle());

			// Initialize the first sentence.
			InitializeSentence(m_sentence1, 16, DirectXPlatformManager::Instance().GetDevice());
			// Now update the sentence vertex buffer with the new string information.
			UpdateSentence(m_sentence1, "Hello", 20, 20, 1.0f, 1.0f, 1.0f, DirectXPlatformManager::Instance().GetDeviceContext());

			// Initialize the first sentence.
			InitializeSentence(m_sentence2, 16, DirectXPlatformManager::Instance().GetDevice());
			// Now update the sentence vertex buffer with the new string information.
			UpdateSentence(m_sentence2, "Goodbye", 20, 40, 1.0f, 1.0f, 0.0f, DirectXPlatformManager::Instance().GetDeviceContext());

			InitializeSentence(m_sentence3, 32, DirectXPlatformManager::Instance().GetDevice());
			UpdateSentence(m_sentence3, "Mouse Coordinate", 20, 60, 0.0f, 1.0f, 0.0f, DirectXPlatformManager::Instance().GetDeviceContext());

			InitializeSentence(m_operationSentence, 42, DirectXPlatformManager::Instance().GetDevice());
			UpdateSentence(m_operationSentence, "Operation: W A S D UP DOWN LEFT RIGHT R F", 20, 80, 1.0f, 1.0f, 0.0f,
				DirectXPlatformManager::Instance().GetDeviceContext());
		}


		Text::Text(const Text& other)
		{
		}


		Text::~Text()
		{
			// Release the font shader object.
			if (m_FontShader)
			{
				m_FontShader->Shutdown();
				delete m_FontShader;
				m_FontShader = 0;
			}

			// Release the font object.
			if (m_Font)
			{
				m_Font->Shutdown();
				delete m_Font;
				m_Font = 0;
			}
		}

		bool Text::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
		{
			bool result;


			// Draw the first sentence.
			result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}

			// Draw the second sentence.
			result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}

			result = RenderSentence(deviceContext, m_sentence3, worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}

			result = RenderSentence(deviceContext, m_operationSentence, worldMatrix, orthoMatrix);
			if (!result)
			{
				return false;
			}

			return true;
		}


		bool Text::InitializeSentence(std::shared_ptr<SentenceType>& sentence, int maxLength, ID3D11Device* device)
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;
			int i;


			// Create a new sentence object.
			sentence = std::make_shared<SentenceType>();
			if (!sentence)
			{
				return false;
			}

			// Initialize the sentence buffers to null.
			sentence->vertexBuffer = 0;
			sentence->indexBuffer = 0;

			// Set the maximum length of the sentence.
			sentence->maxLength = maxLength;

			// Set the number of vertices in the vertex array.
			sentence->vertexCount = 6 * maxLength;

			// Set the number of indexes in the index array.
			sentence->indexCount = sentence->vertexCount;

			// Create the vertex array.
			vertices = new VertexType[sentence->vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Create the index array.
			indices = new unsigned long[sentence->indexCount];
			if (!indices)
			{
				return false;
			}

			// Initialize vertex array to zeros at first.
			memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

			// Initialize the index array.
			for (i = 0; i < sentence->indexCount; i++)
			{
				indices[i] = i;
			}

			// Set up the description of the dynamic vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * sentence->vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Create the vertex buffer.
			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &sentence->vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * sentence->indexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = device->CreateBuffer(&indexBufferDesc, &indexData, &sentence->indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Release the vertex array as it is no longer needed.
			delete[] vertices;
			vertices = 0;

			// Release the index array as it is no longer needed.
			delete[] indices;
			indices = 0;

			return true;
		}


		bool Text::UpdateSentence(std::shared_ptr<SentenceType> sentence, std::string text, int positionX, int positionY, float red, float green, float blue,
			ID3D11DeviceContext* deviceContext)
		{
			VertexType* vertices;
			float drawX, drawY;
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			VertexType* verticesPtr;


			// Store the color of the sentence.
			sentence->red = red;
			sentence->green = green;
			sentence->blue = blue;

			// Check for possible buffer overflow.
			if (text.length() > sentence->maxLength)
			{
				return false;
			}

			// Create the vertex array.
			vertices = new VertexType[sentence->vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Initialize vertex array to zeros at first.
			memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

			// Calculate the X and Y pixel position on the screen to start drawing to.
			drawX = (float)(((NativePlatform::Instance().GetScreenWidth() / 2) * -1) + positionX);
			drawY = (float)((NativePlatform::Instance().GetScreenWidth() / 2) - positionY);

			// Use the font class to build the vertex array from the sentence text and sentence draw location.
			m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

			// Lock the vertex buffer so it can be written to.
			result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the vertex buffer.
			verticesPtr = (VertexType*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

			// Unlock the vertex buffer.
			deviceContext->Unmap(sentence->vertexBuffer, 0);

			// Release the vertex array as it is no longer needed.
			delete[] vertices;
			vertices = 0;

			return true;
		}


		bool Text::RenderSentence(ID3D11DeviceContext* deviceContext, std::shared_ptr<SentenceType> sentence, D3DXMATRIX worldMatrix,
			D3DXMATRIX orthoMatrix)
		{
			// Set vertex buffer stride and offset.
			unsigned int stride = sizeof(VertexType);
			unsigned int offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Create a pixel color vector with the input sentence color.
			D3DXVECTOR4 pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

			D3DXMATRIX viewMatrix;
			GUIManager::Instance().GetCamera()->GetViewMatrix(viewMatrix);
			bool result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture(),
				pixelColor);
			if (!result)
			{
				false;
			}

			return true;
		}


		bool Text::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
		{
			std::string mouseString;

			// Setup the mouse string.
			mouseString = "Mouse X: ";
			mouseString += std::to_string(mouseX);
			mouseString += "  Mouse Y: ";
			mouseString += std::to_string(mouseY);

			// Update the sentence vertex buffer with the new string information.
			bool result = UpdateSentence(m_sentence3, mouseString, 20, 60, 1.0f, 1.0f, 1.0f, deviceContext);
			if (!result)
			{
				return false;
			}

			return true;
		}


		bool Text::SetFps(int fps, ID3D11DeviceContext* deviceContext)
		{
			char tempString[16];
			char fpsString[16];
			float red, green, blue;
			bool result;


			// Truncate the fps to below 10,000.
			if (fps > 9999)
			{
				fps = 9999;
			}

			// Convert the fps integer to string format.
			_itoa_s(fps, tempString, 10);

			// Setup the fps string.
			strcpy_s(fpsString, "Fps: ");
			strcat_s(fpsString, tempString);

			// If fps is 60 or above set the fps color to green.
			if (fps >= 60)
			{
				red = 0.0f;
				green = 1.0f;
				blue = 0.0f;
			}

			// If fps is below 60 set the fps color to yellow.
			if (fps < 60)
			{
				red = 1.0f;
				green = 1.0f;
				blue = 0.0f;
			}

			// If fps is below 30 set the fps color to red.
			if (fps < 30)
			{
				red = 1.0f;
				green = 0.0f;
				blue = 0.0f;
			}

			// Update the sentence vertex buffer with the new string information.
			result = UpdateSentence(m_sentence1, fpsString, 20, 20, red, green, blue, deviceContext);
			if (!result)
			{
				return false;
			}

			return true;
		}


		bool Text::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
		{
			char tempString[16];
			char cpuString[16];
			bool result;


			// Convert the cpu integer to string format.
			_itoa_s(cpu, tempString, 10);

			// Setup the cpu string.
			strcpy_s(cpuString, "Cpu: ");
			strcat_s(cpuString, tempString);
			strcat_s(cpuString, "%");

			// Update the sentence vertex buffer with the new string information.
			result = UpdateSentence(m_sentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
			if (!result)
			{
				return false;
			}

			return true;
		}
	}
}