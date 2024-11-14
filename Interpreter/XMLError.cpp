#include "XMLError.h"
#include<iostream>
XMLInvalidString::XMLInvalidString(const char* str):str(str)
{
}

void XMLInvalidString::ErrorMessage() const
{
	std::cout << "��Ч���ַ���:" << str << std::endl;
}

XMLUnexpectedTerminalSymbol::XMLUnexpectedTerminalSymbol(const char chr) : symbol(chr)
{
}

void XMLUnexpectedTerminalSymbol::ErrorMessage() const
{
	std::cout << "������ս��:" << symbol << std::endl;
}

XMLUnkownString::XMLUnkownString(const char* str):str(str)
{
}

void XMLUnkownString::ErrorMessage() const
{
	std::cout << "�޷��������ַ�����" << str << std::endl;
}


XMLInvalidNode::XMLInvalidNode(const char* str) : str(str)
{
}

void XMLInvalidNode::ErrorMessage() const
{
	std::cout <<"��Ч�Ľڵ�:" << str <<std::endl;
}

XMLInvalidSymbol::XMLInvalidSymbol(const char str):ch(str)
{
}

void XMLInvalidSymbol::ErrorMessage() const
{
	std::cout << "�Ƿ��ַ�:" << ch << std::endl;
}
