/**
* 利用 Assimp 导入多种模型格式
* 将各种建模软件独有的 model format 转换为引擎自用 xml 格式
*/


#include <iostream>
#include <fstream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "XmlHelper.h"

#pragma comment(lib, "XmlHelper.lib")


XmlHelper::XMLDocument model_doc;
std::shared_ptr<XmlHelper::XMLNode> root_node;


void InputModelFilename(std::string& filename);
void LoadModel(std::string filename);
void processNode(aiNode *node, const aiScene *scene);
void processMesh(aiMesh *mesh, const aiScene *scene);


int main()
{
	std::string fileName;

	InputModelFilename(fileName);

	LoadModel(fileName);

	return 0;
}


void InputModelFilename(std::string& filename)
{
	// Loop until we have a file name.
	bool done = false;
	std::ifstream fin;

	while (!done)
	{
		// Ask the user for the filename.
		std::cout << "Enter model filename: ";

		// Read in the filename.
		std::cin >> filename;

		// Attempt to open the file.
		
		fin.open(filename);

		if (fin.good())
		{
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
		}
		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
			std::cout << std::endl;
			std::cout << "File " << filename << " could not be opened." << std::endl << std::endl;
		}
	}

	return;
}


void LoadModel(std::string filename)
{
	Assimp::Importer importer;

	// pFlags 用于指示 Assimp 自动对模型做一些处理 =>
	// aiProcess_Triangulate: 将多边形统统转换成三角形, 一些建模软件是支持五边形六边形的, 而 DX 中我们只渲染三角形
	// aiProcess_FixInfacingNormals: 建模软件往往是双面显示, 而 DX 将顺时针视为正面
	// aiProcess_ConvertToLeftHanded: 大部分建模软件都是右手坐标系, 而 DX 本身是左手坐标系
	const aiScene *scene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FixInfacingNormals | aiProcess_ConvertToLeftHanded);

	// 场景和根节点不为空, 场景数据是否完整
	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		system("pause");
		return;
	}

	root_node = model_doc.AllocateNode(XmlHelper::XMLNodeType::XNT_Element, "model");
	root_node->AppendAttribute(model_doc.AllocateAttribute("mesh_count", std::to_string(scene->mNumMeshes)));
	model_doc.AssignRootNode(root_node);

	processNode(scene->mRootNode, scene);

	model_doc.Save("model.xml");
}


// 递归处理场景中的所有节点
void processNode(aiNode *node, const aiScene *scene)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// aiNode::mMeshes 中存的都是到 aiScene::mMeshes 中的索引
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}

	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


void processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::shared_ptr<XmlHelper::XMLNode> mesh_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "mesh");
	{
		std::shared_ptr<XmlHelper::XMLNode> vertices_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "vertices");
		vertices_node->AppendAttribute(model_doc.AllocateAttribute("vertex_count", std::to_string(mesh->mNumVertices)));
		{
			for (size_t i = 0; i < mesh->mNumVertices; i++)
			{
				std::shared_ptr<XmlHelper::XMLNode> vertex_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "vertex");
				{
					// 顶点位置
					std::shared_ptr<XmlHelper::XMLNode> position_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "position");
					{
						std::shared_ptr<XmlHelper::XMLNode> x_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "x");
						x_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(mesh->mVertices[i].x)));
						position_node->AppendNode(x_node);

						std::shared_ptr<XmlHelper::XMLNode> y_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "y");
						y_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(mesh->mVertices[i].y)));
						position_node->AppendNode(y_node);

						std::shared_ptr<XmlHelper::XMLNode> z_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "z");
						z_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(mesh->mVertices[i].z)));
						position_node->AppendNode(z_node);
					}
					vertex_node->AppendNode(position_node);

					// 法线
					std::shared_ptr<XmlHelper::XMLNode> normal_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "normal");
					{
						std::shared_ptr<XmlHelper::XMLNode> x_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "x");
						x_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(mesh->mNormals[i].x)));
						normal_node->AppendNode(x_node);

						std::shared_ptr<XmlHelper::XMLNode> y_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "y");
						y_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(mesh->mNormals[i].y)));
						normal_node->AppendNode(y_node);

						std::shared_ptr<XmlHelper::XMLNode> z_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "z");
						z_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(mesh->mNormals[i].z)));
						normal_node->AppendNode(z_node);
					}
					vertex_node->AppendNode(normal_node);

					// 纹理坐标
					std::shared_ptr<XmlHelper::XMLNode> texcoord_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "texcoord");
					{
						float u_coord, v_coord;
						// 暂时只用到第一套纹理坐标
						if (mesh->mTextureCoords[0])
						{
							u_coord = mesh->mTextureCoords[0][i].x;
							v_coord = mesh->mTextureCoords[0][i].y;
						}
						else
						{
							u_coord = 0.0f;
							v_coord = 0.0f;
						}

						std::shared_ptr<XmlHelper::XMLNode> u_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "u");
						
						u_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(u_coord)));
						texcoord_node->AppendNode(u_node);

						std::shared_ptr<XmlHelper::XMLNode> v_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "v");
						v_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(v_coord)));
						texcoord_node->AppendNode(v_node);
					}
					vertex_node->AppendNode(texcoord_node);
				}
				vertices_node->AppendNode(vertex_node);
			}
		}
		mesh_node->AppendNode(vertices_node);
		

		// 处理索引
		std::shared_ptr<XmlHelper::XMLNode> faces_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "faces");
		faces_node->AppendAttribute(model_doc.AllocateAttribute("face_count", std::to_string(mesh->mNumFaces)));
		{
			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				std::shared_ptr<XmlHelper::XMLNode> face_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "face");
				{
					// 因为使用了 aiProcess_Triangulate, 这里的面总是三角形
					for (size_t j = 0; j < face.mNumIndices; j++)
					{
						std::shared_ptr<XmlHelper::XMLNode> index_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "index");
						index_node->AppendAttribute(model_doc.AllocateAttribute("value", std::to_string(face.mIndices[j])));
						face_node->AppendNode(index_node);
					}
				}
				faces_node->AppendNode(face_node);
			}
		}
		mesh_node->AppendNode(faces_node);


		// 一个 mesh 对应一个材质, 一个 model 可拥有多个 mesh
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::shared_ptr<XmlHelper::XMLNode> material_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "material");

			// 在一个材质对象内部, 每种纹理类型( diffuse, specular... )都对应了一个纹理数组
			int diffuse_count = material->GetTextureCount(aiTextureType_DIFFUSE);
			std::shared_ptr<XmlHelper::XMLNode> diffuse_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "diffuse");
			diffuse_node->AppendAttribute(model_doc.AllocateAttribute("diffuse_count", std::to_string(diffuse_count)));
			for (size_t i = 0; i < diffuse_count; i++)
			{
				aiString texturePath;
				material->GetTexture(aiTextureType_DIFFUSE, i, &texturePath);

				std::shared_ptr<XmlHelper::XMLNode> texture_node = model_doc.AllocateNode(XmlHelper::XNT_Element, "texture");
				texture_node->AppendAttribute(model_doc.AllocateAttribute("value", texturePath.C_Str()));
				diffuse_node->AppendNode(texture_node);
			}
			material_node->AppendNode(diffuse_node);

			mesh_node->AppendNode(material_node);
		}
	}
	root_node->AppendNode(mesh_node);
}
