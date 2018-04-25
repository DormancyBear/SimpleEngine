#include <fstream>
#include "XmlHelper.h"
#include "rapidxml_print.hpp"

using namespace XmlHelper;


XMLDocument::XMLDocument()
{
	_doc = new rapidxml::xml_document<char>();

	// 通用 XML 声明
	rapidxml::xml_node<char>* declnode = _doc->allocate_node(rapidxml::node_declaration);
	declnode->append_attribute(_doc->allocate_attribute("version", "1.0"));
	declnode->append_attribute(_doc->allocate_attribute("encoding", "utf-8"));
	_doc->append_node(declnode);
}


XMLNode* XMLDocument::Parse(const char *filename)
{
	std::ifstream inputfile;
	inputfile.open(filename, std::ios_base::binary);

	// 获取文件长度
	inputfile.seekg(0, std::ios_base::end);
	int fileLength = inputfile.tellg();

	// 读入数据
	inputfile.seekg(0, std::ios_base::beg);
	xml_content.resize(fileLength + 1, 0);	// rapidxml 要求解析的是一个 zero-terminated string
	inputfile.read(&xml_content[0], fileLength);

	inputfile.close();

	// xml 解析
	_doc->parse<0>(&xml_content[0]);
	_root = new XMLNode(_doc->first_node());

	return _root;
}


void XMLDocument::Save(const char *filename)
{
	std::ofstream outputfile;
	outputfile.open(filename);

	// 生成 xml string 到输出流
	outputfile << *_doc;

	outputfile.close();
}


XMLNode* XMLDocument::AllocateNode(XMLNodeType type, std::string name)
{
	return new XMLNode(*_doc, type, _doc->allocate_string(name.c_str()));;
}


XMLAttribute* XMLDocument::AllocateAttribute(std::string name, std::string value)
{
	return new XMLAttribute(*_doc, _doc->allocate_string(name.c_str()), _doc->allocate_string(value.c_str()));
}


void XMLDocument::AssignRootNode(XMLNode* const & root_node)
{
	_doc->append_node(root_node->GetStoredXmlNode());
	_root = root_node;
}


XMLNode::XMLNode(rapidxml::xml_node<char>* node)
{
	_node = node;

	if (_node != nullptr)
	{
		_name = _node->name();
	}
}


XMLNode::XMLNode(rapidxml::xml_document<char>& doc, XMLNodeType type, char* name)
{
	rapidxml::node_type nodeType;
	switch (type)
	{
	case XNT_Document:
		nodeType = rapidxml::node_document;
		break;

	case XNT_Element:
		nodeType = rapidxml::node_element;
		break;

	case XNT_Data:
		nodeType = rapidxml::node_data;
		break;

	case XNT_CData:
		nodeType = rapidxml::node_cdata;
		break;

	case XNT_Comment:
		nodeType = rapidxml::node_comment;
		break;

	case XNT_Declaration:
		nodeType = rapidxml::node_declaration;
		break;

	case XNT_Doctype:
		nodeType = rapidxml::node_doctype;
		break;

	case XNT_PI:
	default:
		nodeType = rapidxml::node_pi;
		break;
	}

	_node = doc.allocate_node(nodeType, name, 0);
	_name = name;
}


void XMLNode::AppendNode(XMLNode* const & node)
{
	_node->append_node(node->GetStoredXmlNode());
}


void XMLNode::AppendAttribute(XMLAttribute* const & attr)
{
	_node->append_attribute(attr->GetStoredXmlAttribute());
}


rapidxml::xml_node<char>* XMLNode::GetStoredXmlNode()
{
	return _node;
}


XMLAttribute::XMLAttribute(rapidxml::xml_document<char>& doc, char* name, char* value)
{
	_attr = doc.allocate_attribute(name, value);
}


rapidxml::xml_attribute<char>* XMLAttribute::GetStoredXmlAttribute()
{
	return _attr;
}