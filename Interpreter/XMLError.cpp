#include "XMLError.h"
#include<iostream>
XMLInvalidString::XMLInvalidString(const char* str):str(str)
{
}

void XMLInvalidString::ErrorMessage() const
{
	std::cout << "无效的字符串:" << str << std::endl;
}

XMLUnexpectedTerminalSymbol::XMLUnexpectedTerminalSymbol(const char chr) : symbol(chr)
{
}

void XMLUnexpectedTerminalSymbol::ErrorMessage() const
{
	std::cout << "意外的终结符:" << symbol << std::endl;
}

XMLUnkownString::XMLUnkownString(const char* str):str(str)
{
}

void XMLUnkownString::ErrorMessage() const
{
	std::cout << "无法解析的字符串：" << str << std::endl;
}


XMLInvalidNode::XMLInvalidNode(const char* str) : str(str)
{
}

void XMLInvalidNode::ErrorMessage() const
{
	std::cout <<"无效的节点:" << str <<std::endl;
}

XMLInvalidSymbol::XMLInvalidSymbol(const char str):ch(str)
{
}

void XMLInvalidSymbol::ErrorMessage() const
{
	std::cout << "非法字符:" << ch << std::endl;
}
