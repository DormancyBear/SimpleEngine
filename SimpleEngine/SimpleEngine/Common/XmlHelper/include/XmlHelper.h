#ifndef XML_HELPER_H_
#define XML_HELPER_H_

#pragma once


#include <string>
#include <vector>
#include <memory>
#include "rapidxml.hpp"


// 为了防止之后升级 rapidxml 时, 一旦 rapidxml 接口发生变化, 我这边项目代码需要大面积修改的情况
// 对 rapidxml 再做一个封装
// 外部项目需要用到 rapidxml 时, 只需包含这一个头文件
namespace XmlHelper
{
	enum XMLNodeType
	{
		XNT_Document,      //!< A document node. Name and value are empty.
		XNT_Element,       //!< An element node. Name contains element name. Value contains text of first data node.
		XNT_Data,          //!< A data node. Name is empty. Value contains data text.
		XNT_CData,         //!< A CDATA node. Name is empty. Value contains data text.
		XNT_Comment,       //!< A comment node. Name is empty. Value contains comment text.
		XNT_Declaration,   //!< A declaration node. Name and value are empty. Declaration parameters (version, encoding and standalone) are in node attributes.
		XNT_Doctype,       //!< A DOCTYPE node. Name is empty. Value contains DOCTYPE text.
		XNT_PI             //!< A PI node. Name contains target. Value contains instructions.
	};

	class XMLAttribute
	{
	public:
		XMLAttribute(rapidxml::xml_attribute<char>* attr);
		XMLAttribute(rapidxml::xml_document<char>& doc, char* name, char* value);

		std::string Value() const;

		rapidxml::xml_attribute<char>* GetStoredXmlAttribute();

	private:
		rapidxml::xml_attribute<char>* _attr;
	};

	class XMLNode
	{
	public:
		XMLNode(rapidxml::xml_node<char>* node);	// 读取
		XMLNode(rapidxml::xml_document<char>& doc, XMLNodeType type, char* name);	// 写入

		void AppendNode(std::shared_ptr<XMLNode> const & node);
		void AppendAttribute(std::shared_ptr<XMLAttribute> const & attr);

		std::shared_ptr<XMLNode> FirstNode(std::string name) const;
		std::shared_ptr<XMLNode> NextSibling(std::string name) const;

		std::shared_ptr<XMLAttribute> FirstAttribute(std::string name) const;
		
		rapidxml::xml_node<char>* GetStoredXmlNode();

	private:
		rapidxml::xml_node<char>* _node;
		std::string _name;
	};


	class XMLDocument
	{
	public:
		XMLDocument();

		// 解析一个 xml 文件
		std::shared_ptr<XMLNode> Parse(std::string fileName);

		// 存为一个 xml 文件
		void Save(const char *filename);

		std::shared_ptr<XMLNode> AllocateNode(XMLNodeType type, std::string name);
		std::shared_ptr<XMLAttribute> AllocateAttribute(std::string name, std::string value);

		// 一个 xml 文件只有一个根节点
		void AssignRootNode(std::shared_ptr<XMLNode> const & root_node);

	private:
		// The string must persist for the lifetime of the document.
		std::vector<char> xml_content;

		// xml_document 不仅代表了 DOM 树的根节点, 它还同时 public 继承自 memory_pool
		// 即所有 node, attribute 都由 xml_document 对象 allocate 得到( 并进行管理 )
		// xml_document 超出作用域时, 会把所有已申请的内存全部释放掉
		// 所以这里要把 xml_document 保存下来
		std::shared_ptr<rapidxml::xml_document<char>> _doc;
		std::shared_ptr<XMLNode> _root;
	};

}

#endif // XML_HELPER_H_
