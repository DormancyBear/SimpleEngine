////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>
#include "textureclass.h"
#include "lightshaderclass.h"


// 一个 mesh( 网格 )是单次渲染的基本单位
// 一个模型是由多个 mesh 组成的
class MeshType
{
private:
	// 如何从这个 VertexType 映射到 VertexShader 里的输入结构?
	// 答案是通过顶点布局( vertex input layout )
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texcoord;
		D3DXVECTOR3 normal;
	};

public:
	MeshType();
	~MeshType();

	bool InitializeBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*);
	int GetIndexCount();

	std::vector<VertexType> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureClass> textures;

private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
};


// 像 3DMax, Maya 这些建模软件都是用的离线渲染, 而游戏引擎则是实时渲染
// 两者所用的材质系统完全不同, 最终 3DMax 提供给引擎的只有网格模型 + 贴图, 其他数据都是无效冗余的
// 所以我们需要把 3DMax 导出的 model format 转换成引擎自用的 format( 剔除无用数据 )
// create our own model format and write a parser to convert those different formats into our own format.
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, std::string);
	void Render(LightShaderClass* shader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
		D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);

private:
	void LoadModel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fileName);

private:
	std::vector<MeshType> m_model;
	std::string m_modelDirectory;	// 存放模型文件的文件夹( 默认所有纹理也都放在这个目录下 )
};

#endif
