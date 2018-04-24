#pragma once

#ifndef XML_HELPER_H_
#define XML_HELPER_H_

#include <string>
#include <vector>
#include "rapidxml.hpp"


// 对 rapidxml 再做一个封装
namespace XmlHelper
{
	class XMLNode
	{
	public:
		XMLNode(rapidxml::xml_node<char>* node);
		~XMLNode();

	private:

	};


	class XMLDocument
	{
	public:
		XMLDocument();
		~XMLDocument();

		// 解析一个 xml 文件
		XMLNode Parse(const char *filename);

		// 存为一个 xml 文件
		void Save(const char *filename);

	private:
		std::vector<char> xml_content;
		rapidxml::xml_document<char> doc;
		XMLNode root;
	};

}

#endif // XML_HELPER_H_
