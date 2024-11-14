///XML����������ĸ��ִ���

#pragma once
#include "AbstractError.h"
#include<string>
/// <summary>
/// ��Ч�ַ���
/// </summary>
class XMLInvalidString :public AbstractError
{
public:  
	XMLInvalidString(const char* str);
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	std::string str;
};

/// <summary>
/// �������ֹ��
/// </summary>
class XMLUnexpectedTerminalSymbol : public AbstractError {
public:
	XMLUnexpectedTerminalSymbol(const char chr);
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	char symbol;
};

/// <summary>
/// δ֪���ַ���
/// </summary>
class XMLUnkownString : public AbstractError {
public:
	XMLUnkownString(const char* str);

	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	std::string str;
};

class XMLInvalidNode : public AbstractError {
public:
	XMLInvalidNode(const char* str);
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	std::string str;
};

/// <summary>
/// �Ƿ��ַ�
/// </summary>
class XMLInvalidSymbol : public AbstractError {
public:
	XMLInvalidSymbol(const char str);
	// ͨ�� AbstractError �̳�
	void ErrorMessage() const override;
private:
	char ch;
};