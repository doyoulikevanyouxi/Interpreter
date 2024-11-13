#pragma once
#include<fstream>
#include<string>
#include<map>
#include "AbstractExpression.h"
#include <vector>





/// <summary>
/// XML���ݱ��ʽ������һ���ս���ʽ
/// </summary>
class XMLDataExpression : public AbstractExpression {
public:
	XMLDataExpression() = default;
};

/// <summary>
/// XML˫���Ž���������һ���ս���ʽ
/// </summary>
class XMLQuotationMarksExpression : public AbstractExpression {
public:

	XMLQuotationMarksExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	std::string str;
	bool error = false;
};



/// <summary>
/// XML��������ȷ��Ž���
/// </summary>
class XMLEqualsExpression : public AbstractExpression {
public:
	XMLEqualsExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	std::string propertyName;
	bool turnToQuotationOperatoin = false;
private:
	XMLQuotationMarksExpression qmarkExp;
};

/// <summary>
/// XML�ڵ����Խ���
/// </summary>
class XMLPropertyExpression : public AbstractExpression {
public:

	XMLPropertyExpression() = default;

	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	std::string nodeName;
	std::vector<std::string> propertys;
private:
	XMLEqualsExpression equalExp;
};


/// <summary>
/// xml node�ڵ����
/// </summary>
class XMLNodeExpression : public AbstractExpression {
public:

	XMLNodeExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	std::string str;
private:
	//XMLNodeStartAndEndExpression nodeExp;
};

class Content {


};



