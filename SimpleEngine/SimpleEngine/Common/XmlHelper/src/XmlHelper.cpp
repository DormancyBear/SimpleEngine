#include <fstream>
#include <filesystem>

#include "XmlHelper.h"
#include "rapidxml_print.hpp"

using namespace XmlHelper;


XMLDocument::XMLDocument()
{
	_doc = std::make_shared<rapidxml::xml_document<char>>();
}


std::shared_ptr<XMLNode> XMLDocument::Parse(std::string fileName)
{
	// 获取文件长度
	int fileLength = std::filesystem::file_size(fileName);

	// 读入数据
	std::ifstream inputfile(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	inputfile.seekg(0, std::ios::beg);
	xml_content.resize(fileLength + 1, 0);	// rapidxml 要求解析的是一个 zero-terminated string
	inputfile.read(xml_content.data(), fileLength);
	inputfile.close();

	// xml 解析
	_doc->parse<0>(&xml_content[0]);
	_root = std::make_shared<XMLNode>(_doc->first_node());

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


std::shared_ptr<XMLNode> XMLDocument::AllocateNode(XMLNodeType type, std::string name)
{
	// rapidxml 内部通过指针来访问我们提供的字符串
	// 所以必须确保字符串的生命周期, 不要用临时变量( 变量一旦销毁数据就没了 )
	return std::make_shared<XMLNode>(*_doc, type, _doc->allocate_string(name.c_str()));
}


std::shared_ptr<XMLAttribute> XMLDocument::AllocateAttribute(std::string name, std::string value)
{
	return std::make_shared<XMLAttribute>(*_doc, _doc->allocate_string(name.c_str()), _doc->allocate_string(value.c_str()));
}


void XMLDocument::AssignRootNode(std::shared_ptr<XMLNode> const & root_node)
{
	// 通用 XML 声明, 读 xml 时 first_node() 不把这个声明视为节点
	rapidxml::xml_node<char>* declnode = _doc->allocate_node(rapidxml::node_declaration);
	declnode->append_attribute(_doc->allocate_attribute("version", "1.0"));
	declnode->append_attribute(_doc->allocate_attribute("encoding", "utf-8"));
	_doc->append_node(declnode);

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


void XMLNode::AppendNode(std::shared_ptr<XMLNode> const & node)
{
	_node->append_node(node->GetStoredXmlNode());
}


void XMLNode::AppendAttribute(std::shared_ptr<XMLAttribute> const & attr)
{
	_node->append_attribute(attr->GetStoredXmlAttribute());
}


// Gets first child node, optionally matching node name.
std::shared_ptr<XMLNode> XMLNode::FirstNode(std::string name) const
{
	rapidxml::xml_node<char>* result_node = _node->first_node(name.c_str());
	if (result_node)
	{
		return std::make_shared<XMLNode>(result_node);
	}
	else
	{
		return nullptr;
	}
}


// Gets next sibling node, optionally matching node name.
// Behaviour is undefined if node has no parent. Use parent() to test if node has a parent.
std::shared_ptr<XMLNode> XMLNode::NextSibling(std::string name) const
{
	rapidxml::xml_node<char>* result_node = _node->next_sibling(name.c_str());
	if (result_node)
	{
		return std::make_shared<XMLNode>(result_node);
	}
	else
	{
		return nullptr;
	}
}


std::shared_ptr<XMLAttribute> XMLNode::FirstAttribute(std::string name) const
{
	rapidxml::xml_attribute<char>* result_attr = _node->first_attribute(name.c_str());
	if (result_attr)
	{
		return std::make_shared<XMLAttribute>(result_attr);
	}
	else
	{
		return nullptr;
	}
}


rapidxml::xml_node<char>* XMLNode::GetStoredXmlNode()
{
	return _node;
}


XMLAttribute::XMLAttribute(rapidxml::xml_attribute<char>* attr)
{
	_attr = attr;
}


XMLAttribute::XMLAttribute(rapidxml::xml_document<char>& doc, char* name, char* value)
{
	_attr = doc.allocate_attribute(name, value);
}


std::string XMLAttribute::Value() const
{
	return std::string(_attr->value());
}


rapidxml::xml_attribute<char>* XMLAttribute::GetStoredXmlAttribute()
{
	return _attr;
}
