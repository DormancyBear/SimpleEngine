////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
#include "XmlHelper.h"
#include <sstream>


ModelClass::ModelClass()
{

}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFilename)
{
	// Load in the model data,
	LoadModel(device, deviceContext, modelFilename);

	for (size_t i = 0; i < m_model.size(); i++)
	{
		bool result = m_model[i].InitializeBuffers(device);
		if (!result)
		{
			return false;
		}
	}
	
	return true;
}


void ModelClass::Render(LightShaderClass* shader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
	 D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	for (size_t i = 0; i < m_model.size(); i++)
	{
		m_model[i].RenderBuffers(deviceContext);

		shader->SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix,
			m_model[i].textures.size() > 0 ? m_model[i].textures[0].GetTexture() : nullptr, lightDirection, ambientColor, diffuseColor,
			cameraPosition, specularColor, specularPower);

		shader->Render(deviceContext, m_model[i].GetIndexCount());
	}
}


void ModelClass::LoadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fileName)
{
	int firstFlagIndex = fileName.find_last_of('/');
	int secondFlagIndex = fileName.find_last_of('\\');
	int finalFlagIndex = firstFlagIndex >= secondFlagIndex ? firstFlagIndex : secondFlagIndex;
	m_modelDirectory = fileName.substr(0, finalFlagIndex + 1);

	XmlHelper::XMLDocument model_doc;
	std::shared_ptr<XmlHelper::XMLNode> root_node = model_doc.Parse(fileName);
	int mesh_count = std::stoi(root_node->FirstAttribute("mesh_count")->Value());
	m_model.resize(mesh_count);

	std::shared_ptr<XmlHelper::XMLNode> mesh_node = root_node->FirstNode("mesh");
	// 循环处理所有 mesh 节点
	for (size_t i = 0; i < mesh_count, mesh_node != nullptr; i++, mesh_node = mesh_node->NextSibling("mesh"))
	{
		std::shared_ptr<XmlHelper::XMLNode> vertices_node = mesh_node->FirstNode("vertices");
		int vertex_count = std::stoi(vertices_node->FirstAttribute("vertex_count")->Value());
		m_model[i].vertices.resize(vertex_count);

		std::shared_ptr<XmlHelper::XMLNode> vertex_node = vertices_node->FirstNode("vertex");
		for (size_t j = 0; j < vertex_count, vertex_node != nullptr; j++, vertex_node = vertex_node->NextSibling("vertex"))
		{
			std::shared_ptr<XmlHelper::XMLNode> position_node = vertex_node->FirstNode("position");
			{
				std::shared_ptr<XmlHelper::XMLNode> x_node = position_node->FirstNode("x");
				float x_coord = std::stof(x_node->FirstAttribute("value")->Value());

				std::shared_ptr<XmlHelper::XMLNode> y_node = position_node->FirstNode("y");
				float y_coord = std::stof(y_node->FirstAttribute("value")->Value());

				std::shared_ptr<XmlHelper::XMLNode> z_node = position_node->FirstNode("z");
				float z_coord = std::stof(z_node->FirstAttribute("value")->Value());


				m_model[i].vertices[j].position = D3DXVECTOR3(x_coord, y_coord, z_coord);
			}

			std::shared_ptr<XmlHelper::XMLNode> normal_node = vertex_node->FirstNode("normal");
			{
				std::shared_ptr<XmlHelper::XMLNode> x_node = normal_node->FirstNode("x");
				float x_coord = std::stof(x_node->FirstAttribute("value")->Value());

				std::shared_ptr<XmlHelper::XMLNode> y_node = normal_node->FirstNode("y");
				float y_coord = std::stof(y_node->FirstAttribute("value")->Value());

				std::shared_ptr<XmlHelper::XMLNode> z_node = normal_node->FirstNode("z");
				float z_coord = std::stof(z_node->FirstAttribute("value")->Value());

				m_model[i].vertices[j].normal = D3DXVECTOR3(x_coord, y_coord, z_coord);
			}

			std::shared_ptr<XmlHelper::XMLNode> texcoord_node = vertex_node->FirstNode("texcoord");
			{
				std::shared_ptr<XmlHelper::XMLNode> u_node = texcoord_node->FirstNode("u");
				float u_coord = std::stof(u_node->FirstAttribute("value")->Value());

				std::shared_ptr<XmlHelper::XMLNode> v_node = texcoord_node->FirstNode("v");
				float v_coord = std::stof(v_node->FirstAttribute("value")->Value());

				m_model[i].vertices[j].texcoord = D3DXVECTOR2(u_coord, v_coord);
			}
		}
	

		std::shared_ptr<XmlHelper::XMLNode> faces_node = mesh_node->FirstNode("faces");
		if (faces_node != nullptr)
		{
			std::shared_ptr<XmlHelper::XMLNode> face_node = faces_node->FirstNode("face");
			while (face_node != nullptr)
			{
				std::shared_ptr<XmlHelper::XMLNode> index_node = face_node->FirstNode("index");
				while (index_node != nullptr)
				{
					int index = std::stoi(index_node->FirstAttribute("value")->Value());
					m_model[i].indices.push_back(index);

					index_node = index_node->NextSibling("index");
				}

				face_node = face_node->NextSibling("face");
			}
		}


		std::shared_ptr<XmlHelper::XMLNode> material_node = mesh_node->FirstNode("material");
		if (material_node != nullptr)
		{
			std::shared_ptr<XmlHelper::XMLNode> diffuse_node = material_node->FirstNode("diffuse");
			int diffuse_count = std::stoi(diffuse_node->FirstAttribute("diffuse_count")->Value());
			m_model[i].textures.resize(diffuse_count);

			std::shared_ptr<XmlHelper::XMLNode> texture_node = diffuse_node->FirstNode("texture");
			for (size_t k = 0; k < vertex_count, texture_node != nullptr; k++, texture_node = texture_node->NextSibling("texture"))
			{
				// 从网上下载的模型文件, 可能里面的纹理路径是绝对路径, 全部转化成跟模型文件同级目录
				std::string texture_path = texture_node->FirstAttribute("value")->Value();
				int firstFlagIndex = texture_path.find_last_of('/');
				int secondFlagIndex = texture_path.find_last_of('\\');
				int finalFlagIndex = firstFlagIndex >= secondFlagIndex ? firstFlagIndex : secondFlagIndex;
				texture_path = m_modelDirectory + texture_path.substr(finalFlagIndex + 1);

				std::size_t wpath_size = texture_path.size() + 1;
				std::vector<wchar_t> wtexture_path(wpath_size);
				std::size_t convertedChars = 0;
				mbstowcs_s(&convertedChars, &wtexture_path[0], wpath_size, texture_path.c_str(), _TRUNCATE);

				// 每个 mesh 可能有多张纹理
				m_model[i].textures[k].Initialize(device, deviceContext, &wtexture_path[0]);
			}
		}

	}
}


MeshType::MeshType()
	:m_vertexBuffer(nullptr), m_indexBuffer(nullptr)
{
	//
}


MeshType::~MeshType()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	for (size_t i = 0; i < textures.size(); i++)
	{
		textures[i].Shutdown();
	}
}


// Initialize the vertex and index buffers.
bool MeshType::InitializeBuffers(ID3D11Device* device)
{
	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
void MeshType::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


int MeshType::GetIndexCount()
{
	return indices.size();
}
