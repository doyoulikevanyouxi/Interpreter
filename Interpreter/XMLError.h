///XML解析会产生的各种错误

#pragma once
#include "AbstractError.h"
/// <summary>
/// 无效字符串
/// </summary>
class XMLInvalidString :public AbstractError
{
public:  
	XMLInvalidString(const char* start,const char* end);
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
};

/// <summary>
/// 意外的终止符
/// </summary>
class XMLUnexpectedTerminalSymbol : public AbstractError {
public:
	XMLUnexpectedTerminalSymbol(const char* start, const char* end,const char chr,bool findOrNot = true);
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	char symbol;
	bool find;
};

class XMLUnexpectedSymbol : public AbstractError {
public:
	XMLUnexpectedSymbol(const char* start, const char* end,const char chr ='\0');
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	char symbol;
};

class XMLInvalidNode : public AbstractError {
public:
	XMLInvalidNode() =default;
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
};

class XMLError : public AbstractError {
public:
	XMLError(const char* str);
	// 通过 AbstractError 继承
	void ErrorMessage() const override;
private:
	std::string str;
};