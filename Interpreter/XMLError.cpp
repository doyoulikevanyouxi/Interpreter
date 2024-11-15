#include "XMLError.h"
#include<iostream>

XMLInvalidString::XMLInvalidString(const char* start, const char* end)
{
	message.assign(start, end);
}

void XMLInvalidString::ErrorMessage() const
{
	std::cout << "��Ч���ַ��� " << message << std::endl;
}

XMLUnexpectedTerminalSymbol::XMLUnexpectedTerminalSymbol(const char* start, const char* end, const char chr, bool findOrNot):symbol(chr),find(findOrNot)
{
	message.assign(start, end);
}

void XMLUnexpectedTerminalSymbol::ErrorMessage() const
{
	if(find)
		std::cout << "���ʽ " << message << " ����������ս�� " <<symbol<< std::endl;
	else
		std::cout << "���ʽ " << message << " δ�ҵ�ƥ����ս�� "<<symbol << std::endl;
}


void XMLInvalidNode::ErrorMessage() const
{
	std::cout <<"��Ч�Ľڵ�"<<std::endl;
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
	std::cout << "���ʽ " << message << " �а�����������ַ� " << symbol << std::endl;
}
