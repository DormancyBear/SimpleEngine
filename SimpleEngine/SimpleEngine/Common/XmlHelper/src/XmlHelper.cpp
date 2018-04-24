#include <fstream>
#include "XmlHelper.h"
#include "rapidxml_print.hpp"

using namespace XmlHelper;

XMLNode XMLDocument::Parse(const char *filename)
{
	std::ifstream inputfile;
	inputfile.open(filename);

	// 获取文件长度
	inputfile.seekg(0, std::ios_base::end);
	int fileLength = inputfile.tellg();

	// 读入数据
	inputfile.seekg(0, std::ios_base::beg);
	xml_content.resize(fileLength + 1, 0);	// rapidxml 要求解析的是一个 zero-terminated string
	inputfile.read(&xml_content[0], fileLength);

	inputfile.close();

	// xml 解析
	doc.parse<0>(&xml_content[0]);
	root = XMLNode(doc.first_node());

	return root;
}

void XMLDocument::Save(const char *filename)
{
	std::ofstream outputfile;
	outputfile.open(filename);

	// 生成 xml string 到输出流
	outputfile << doc;

	outputfile.close();
}