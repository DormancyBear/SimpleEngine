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

	bool Initialize(ID3D11Device*, std::string, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*, LightShaderClass*);

	ID3D11ShaderResourceView* GetTexture();


private:
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	void LoadModel(std::string fileName);

private:
	TextureClass * m_Texture;
	std::vector<MeshType> m_model;
};

#endif
