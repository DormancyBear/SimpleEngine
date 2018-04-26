// 用于 import static 3D models from Maya
// 将 .OBJ format 转化为引擎自用 format
// 输入: *.obj	输出: *.xml


//////////////
// INCLUDES //
//////////////
#include <iostream>
#include <fstream>
#include "XmlHelper.h"

using namespace std;


#pragma comment(lib, "XmlHelper.lib")


//////////////
// TYPEDEFS //
//////////////
typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType;


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);


//////////////////
// MAIN PROGRAM //
//////////////////
int main()
{
	bool result;
	char filename[256];
	int vertexCount, textureCount, normalCount, faceCount;


	// Read in the name of the model file.
	GetModelFilename(filename);

	// Read in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if(!result)
	{
		return -1;
	}

	// Display the counts to the screen for information purposes.
	cout << endl;
	cout << "Vertices: " << vertexCount << endl;
	cout << "UVs:      " << textureCount << endl;
	cout << "Normals:  " << normalCount << endl;
	cout << "Faces:    " << faceCount << endl;

	// Now read the data from the file into the data structures and then output it in our model format.
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if(!result)
	{
		return -1;
	}

	// Notify the user the model has been converted.
	cout << "\nFile has been converted." << endl;

	return 0;
}


void GetModelFilename(char* filename)
{
	bool done;
	ifstream fin;


	// Loop until we have a file name.
	done = false;
	while(!done)
	{
		// Ask the user for the filename.
		cout << "Enter model filename: ";

		// Read in the filename.
		cin >> filename;

		// Attempt to open the file.
		fin.open(filename);

		if(fin.good())
		{
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
		}
		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
			cout << endl;
			cout << "File " << filename << " could not be opened." << endl << endl;
		}
	}

	return;
}


bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while(!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if(input == 'v')
		{
			fin.get(input);
			if(input == ' ') { vertexCount++; }
			if(input == 't') { textureCount++; }
			if(input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if(input == 'f')
		{
			fin.get(input);
			if(input == ' ') { faceCount++; }
		}
		
		// Otherwise read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}


bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;


	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if(!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if(!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if(!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if(!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	ifstream fin;
	fin.open(filename);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Maya 用的是 right hand system, 而 dx11 用的是 left hand system, 需要做一个转换
	fin.get(input);
	while(!fin.eof())
	{
		if(input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if(input == ' ') 
			{ 
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++; 
			}

			// Read in the texture uv coordinates.
			if(input == 't') 
			{ 
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++; 
			}

			// Read in the normals.
			if(input == 'n') 
			{ 
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++; 
			}
		}

		// Read in the faces.
		if(input == 'f') 
		{
			fin.get(input);
			if(input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	// Write out the file header that our model format uses.
	XmlHelper::XMLDocument mesh_doc;
	std::shared_ptr<XmlHelper::XMLNode> root = mesh_doc.AllocateNode(XmlHelper::XMLNodeType::XNT_Element, "model");
	mesh_doc.AssignRootNode(root);

	std::shared_ptr<XmlHelper::XMLNode> vertices_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "vertices");
	vertices_node->AppendAttribute(mesh_doc.AllocateAttribute("vertex_count", std::to_string(faceCount * 3)));
	{
		// Now loop through all the faces and output the three vertices for each face.
		for (int i = 0; i<faceIndex; i++)
		{
			vIndex = faces[i].vIndex1 - 1;
			tIndex = faces[i].tIndex1 - 1;
			nIndex = faces[i].nIndex1 - 1;

			std::shared_ptr<XmlHelper::XMLNode> vertex1_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "vertex");
			{
				std::shared_ptr<XmlHelper::XMLNode> position_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "position");
				std::string strPosition = std::to_string(vertices[vIndex].x) + ' ' + std::to_string(vertices[vIndex].y)
					+ ' ' + std::to_string(vertices[vIndex].z);
				position_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strPosition));
				vertex1_node->AppendNode(position_node);

				std::shared_ptr<XmlHelper::XMLNode> texcoord_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "texcoord");
				std::string strTexcoord = std::to_string(texcoords[tIndex].x) + ' ' + std::to_string(texcoords[tIndex].y);
				texcoord_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strTexcoord));
				vertex1_node->AppendNode(texcoord_node);

				std::shared_ptr<XmlHelper::XMLNode> normal_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "normal");
				std::string strNormal = std::to_string(normals[vIndex].x) + ' ' + std::to_string(normals[vIndex].y)
					+ ' ' + std::to_string(normals[vIndex].z);
				normal_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strNormal));
				vertex1_node->AppendNode(normal_node);
			}
			vertices_node->AppendNode(vertex1_node);

			vIndex = faces[i].vIndex2 - 1;
			tIndex = faces[i].tIndex2 - 1;
			nIndex = faces[i].nIndex2 - 1;

			std::shared_ptr<XmlHelper::XMLNode> vertex2_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "vertex");
			{
				std::shared_ptr<XmlHelper::XMLNode> position_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "position");
				std::string strPosition = std::to_string(vertices[vIndex].x) + ' ' + std::to_string(vertices[vIndex].y)
					+ ' ' + std::to_string(vertices[vIndex].z);
				position_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strPosition));
				vertex2_node->AppendNode(position_node);

				std::shared_ptr<XmlHelper::XMLNode> texcoord_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "texcoord");
				std::string strTexcoord = std::to_string(texcoords[tIndex].x) + ' ' + std::to_string(texcoords[tIndex].y);
				texcoord_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strTexcoord));
				vertex2_node->AppendNode(texcoord_node);

				std::shared_ptr<XmlHelper::XMLNode> normal_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "normal");
				std::string strNormal = std::to_string(normals[vIndex].x) + ' ' + std::to_string(normals[vIndex].y)
					+ ' ' + std::to_string(normals[vIndex].z);
				normal_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strNormal));
				vertex2_node->AppendNode(normal_node);
			}
			vertices_node->AppendNode(vertex2_node);

			vIndex = faces[i].vIndex3 - 1;
			tIndex = faces[i].tIndex3 - 1;
			nIndex = faces[i].nIndex3 - 1;

			std::shared_ptr<XmlHelper::XMLNode> vertex3_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "vertex");
			{
				std::shared_ptr<XmlHelper::XMLNode> position_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "position");
				std::string strPosition = std::to_string(vertices[vIndex].x) + ' ' + std::to_string(vertices[vIndex].y)
					+ ' ' + std::to_string(vertices[vIndex].z);
				position_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strPosition));
				vertex3_node->AppendNode(position_node);

				std::shared_ptr<XmlHelper::XMLNode> texcoord_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "texcoord");
				std::string strTexcoord = std::to_string(texcoords[tIndex].x) + ' ' + std::to_string(texcoords[tIndex].y);
				texcoord_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strTexcoord));
				vertex3_node->AppendNode(texcoord_node);

				std::shared_ptr<XmlHelper::XMLNode> normal_node = mesh_doc.AllocateNode(XmlHelper::XNT_Element, "normal");
				std::string strNormal = std::to_string(normals[vIndex].x) + ' ' + std::to_string(normals[vIndex].y)
					+ ' ' + std::to_string(normals[vIndex].z);
				normal_node->AppendAttribute(mesh_doc.AllocateAttribute("value", strNormal));
				vertex3_node->AppendNode(normal_node);
			}
			vertices_node->AppendNode(vertex3_node);
		}
	}
	root->AppendNode(vertices_node);
	mesh_doc.Save("model.xml");
	
	// Release the four data structures.
	if(vertices)
	{
		delete [] vertices;
		vertices = 0;
	}
	if(texcoords)
	{
		delete [] texcoords;
		texcoords = 0;
	}
	if(normals)
	{
		delete [] normals;
		normals = 0;
	}
	if(faces)
	{
		delete [] faces;
		faces = 0;
	}

	return true;
}