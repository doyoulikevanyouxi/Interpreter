///XML解析会产生的各种错误

#pragma once
#include "AbstractError.h"
#include<string>
/// <summary>
/// 无效字符串
/// </summary>
class XMLInvalidString :public AbstractError
{
public:  
	XMLInvalidString(const char* str);
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	std::string str;
};

/// <summary>
/// 意外的终止符
/// </summary>
class XMLUnexpectedTerminalSymbol : public AbstractError {
public:
	XMLUnexpectedTerminalSymbol(const char chr);
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	char symbol;
};

/// <summary>
/// 未知的字符串
/// </summary>
class XMLUnkownString : public AbstractError {
public:
	XMLUnkownString(const char* str);

	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	std::string str;
};

class XMLInvalidNode : public AbstractError {
public:
	XMLInvalidNode(const char* str);
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	std::string str;
};

/// <summary>
/// 非法字符
/// </summary>
class XMLInvalidSymbol : public AbstractError {
public:
	XMLInvalidSymbol(const char str);
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	char ch;
};