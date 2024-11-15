///XML����������ĸ��ִ���

#pragma once
#include "AbstractError.h"
/// <summary>
/// ��Ч�ַ���
/// </summary>
class XMLInvalidString :public AbstractError
{
public:  
	XMLInvalidString(const char* start,const char* end);
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
};

/// <summary>
/// �������ֹ��
/// </summary>
class XMLUnexpectedTerminalSymbol : public AbstractError {
public:
	XMLUnexpectedTerminalSymbol(const char* start, const char* end,const char chr,bool findOrNot = true);
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	char symbol;
	bool find;
};

class XMLUnexpectedSymbol : public AbstractError {
public:
	XMLUnexpectedSymbol(const char* start, const char* end,const char chr ='\0');
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	char symbol;
};

class XMLInvalidNode : public AbstractError {
public:
	XMLInvalidNode() =default;
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
};

class XMLError : public AbstractError {
public:
	XMLError(const char* str);
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	std::string str;
};