#include "XMLError.h"
#include<iostream>

XMLInvalidString::XMLInvalidString(const char* start, const char* end)
{
	message.assign(start, end);
}

void XMLInvalidString::ErrorMessage() const
{
	std::cout << "无效的字符串 " << message << std::endl;
}

XMLUnexpectedTerminalSymbol::XMLUnexpectedTerminalSymbol(const char* start, const char* end, const char chr, bool findOrNot):symbol(chr),find(findOrNot)
{
	message.assign(start, end);
}

void XMLUnexpectedTerminalSymbol::ErrorMessage() const
{
	if(find)
		std::cout << "表达式 " << message << " 出现意外的终结符 " <<symbol<< std::endl;
	else
		std::cout << "表达式 " << message << " 未找到匹配的终结符 "<<symbol << std::endl;
}


void XMLInvalidNode::ErrorMessage() const
{
	std::cout <<"无效的节点"<<std::endl;
}


XMLError::XMLError(const char* str):str(str)
{
}

void XMLError::ErrorMessage() const
{
	std::cout<<str<< std::endl;
}

XMLUnexpectedSymbol::XMLUnexpectedSymbol(const char* start, const char* end, const char chr):symbol(chr)
{
	message.assign(start, end);
}

void XMLUnexpectedSymbol::ErrorMessage() const
{
	std::cout << "表达式 " << message << " 中包含不允许的字符 " << symbol << std::endl;
}
