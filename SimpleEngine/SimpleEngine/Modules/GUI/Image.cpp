#include <GUI/Image.h>
#include <Graphics/DirectXPlatformManager.h>
#include <Graphics/Camera.h>
#include <NativePlatform/NativePlatform.h>
#include <GUI/GUIManager.h>

namespace SimpleEngine
{
	namespace UI
	{
		Image::Image(WCHAR const * textureFilename)
		{
			InitializeShader();
			InitializeBuffers();
			LoadTexture(textureFilename);
		}


		Image::Image(Image const & other)
		{
		}


		Image::~Image()
		{
			// Release the bitmap texture.
			ReleaseTexture();

			// Shutdown the vertex and index buffers.
			ShutdownBuffers();

			// Release the texture shader object.
			if (m_TextureShader)
			{
				m_TextureShader->Shutdown();
				delete m_TextureShader;
				m_TextureShader = nullptr;
			}
		}

		void Image::DoRender(Rect<int> bounds)
		{
			// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
			RebuildBuffers(bounds);
			// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
			RenderBuffers();
		}


		int Image::GetIndexCount()
		{
			return m_indexCount;
		}


		ID3D11ShaderResourceView* Image::GetTexture()
		{
			return m_Texture->GetTexture();
		}


		bool Image::InitializeBuffers()
		{
			VertexType* vertices;
			unsigned long* indices;
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;
			int i;

			// 一个矩形 = 两个三角形 = 六个顶点
			m_vertexCount = 6;

			// Set the number of indices in the index array.
			m_indexCount = m_vertexCount;

			// Create the vertex array.
			vertices = new VertexType[m_vertexCount];
			if (!vertices)
			{
				return false;
			}

			// Create the index array.
			indices = new unsigned long[m_indexCount];
			if (!indices)
			{
				return false;
			}

			// Initialize vertex array to zeros at first.
			memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

			// Load the index array with data.
			for (i = 0; i < m_indexCount; i++)
			{
				indices[i] = i;
			}

			// 在 2D Rendering 中, 存储 square 的 vertex buffer 需要设为 dynamic vertex buffer => D3D11_USAGE_DYNAMIC + D3D11_CPU_ACCESS_WRITE
			// 场景中的模型数据往往用 static vertex buffer( 自第一次向显存里提供数据后, we can't change the data inside the buffer )
			// 而 dynamic vertex buffer 在创建之后, 仍然可以在每帧去修改里面的数据
			// 当然, dynamic 比 static 要慢很多, 但比每帧 destroy and recreate a static vertex buffer 要好得多
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Now create the vertex buffer.
			result = DirectXPlatformManager::Instance().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// 虽然顶点坐标会变, 但这几个 index 指向的一直是同样的顶点, 所以这里不需要设为 dynamic
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = DirectXPlatformManager::Instance().GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Release the arrays now that the vertex and index buffers have been created and loaded.
			delete[] vertices;
			vertices = 0;

			delete[] indices;
			indices = 0;

			return true;
		}


		void Image::ShutdownBuffers()
		{
			// Release the index buffer.
			if (m_indexBuffer)
			{
				m_indexBuffer->Release();
				m_indexBuffer = 0;
			}

			// Release the vertex buffer.
			if (m_vertexBuffer)
			{
				m_vertexBuffer->Release();
				m_vertexBuffer = 0;
			}

			return;
		}

		bool Image::RebuildBuffers(Rect<int> bounds)
		{
			// 如果这个 image 相对于上一帧的尺寸大小没有变化的话, 就不用浪费时间去修改 dynamic vertex buffer, 直接绑定到渲染流水线就可以用了 (每帧都需要重新绑定)
			if (image_bounds_ == bounds)
			{
				return true;
			}
			image_bounds_ = bounds;

			// Create the vertex array.
			VertexType* vertices = new VertexType[m_vertexCount];
			if (!vertices)
			{
				return false;
			}

			vertices[0].position = D3DXVECTOR3(image_bounds_.left, image_bounds_.top, 0.0f);  // Top left.
			vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);
			vertices[1].position = D3DXVECTOR3(image_bounds_.right, image_bounds_.bottom, 0.0f);  // Bottom right.
			vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);
			vertices[2].position = D3DXVECTOR3(image_bounds_.left, image_bounds_.bottom, 0.0f);  // Bottom left.
			vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

			vertices[3].position = D3DXVECTOR3(image_bounds_.left, image_bounds_.top, 0.0f);  // Top left.
			vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);
			vertices[4].position = D3DXVECTOR3(image_bounds_.right, image_bounds_.top, 0.0f);  // Top right.
			vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);
			vertices[5].position = D3DXVECTOR3(image_bounds_.right, image_bounds_.bottom, 0.0f);  // Bottom right.
			vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

			// Lock the vertex buffer so it can be written to.
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT result = DirectXPlatformManager::Instance().GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			// Get a pointer to the data in the vertex buffer.
			VertexType *verticesPtr = (VertexType*)mappedResource.pData;

			// Copy the data into the vertex buffer.
			memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

			// Unlock the vertex buffer.
			DirectXPlatformManager::Instance().GetDeviceContext()->Unmap(m_vertexBuffer, 0);

			// Release the vertex array as it is no longer needed.
			delete[] vertices;
			vertices = 0;

			return true;
		}

		void Image::RenderBuffers()
		{
			// Set vertex buffer stride and offset.
			unsigned int stride = sizeof(VertexType);
			unsigned int offset = 0;
			// Set the vertex buffer to active in the input assembler so it can be rendered.
			DirectXPlatformManager::Instance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
			// Set the index buffer to active in the input assembler so it can be rendered.
			DirectXPlatformManager::Instance().GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
			DirectXPlatformManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
			GUIManager::Instance().GetWorldMatrix(worldMatrix);
			GUIManager::Instance().GetCamera()->GetViewMatrix(viewMatrix);
			// 2D Rendering 中需要用 ortho matrix
			DirectXPlatformManager::Instance().GetOrthoMatrix(orthoMatrix);

			// Render the bitmap with the texture shader.
			m_TextureShader->Render(DirectXPlatformManager::Instance().GetDeviceContext(),
				GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, GetTexture());

			return;
		}


		bool Image::LoadTexture(WCHAR const * filename)
		{
			bool result;


			// Create the texture object.
			m_Texture = new TextureClass;
			if (!m_Texture)
			{
				return false;
			}

			// Initialize the texture object.
			result = m_Texture->Initialize(filename);
			if (!result)
			{
				return false;
			}

			return true;
		}

		void Image::ReleaseTexture()
		{
			// Release the texture object.
			if (m_Texture)
			{
				m_Texture->Shutdown();
				delete m_Texture;
				m_Texture = 0;
			}

			return;
		}

		bool Image::InitializeShader()
		{
			bool result;

			// Create the texture shader object.
			m_TextureShader = new TextureShaderClass;
			if (!m_TextureShader)
			{
				return false;
			}

			// Initialize the texture shader object.
			result = m_TextureShader->Initialize(DirectXPlatformManager::Instance().GetDevice(), NativePlatform::Instance().GetWindowHandle());
			if (!result)
			{
				MessageBox(NativePlatform::Instance().GetWindowHandle(), L"Could not initialize the texture shader object.", L"Error", MB_OK);
				return false;
			}

			return true;
		}
	}
}