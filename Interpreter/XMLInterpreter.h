#pragma once
#include<fstream>
#include<string>
#include<map>
#include "AbstractExpression.h"
#include <vector>





/// <summary>
/// XML数据表达式，这是一个终结表达式
/// </summary>
class XMLDataExpression : public AbstractExpression {
public:
	XMLDataExpression() = default;
};

/// <summary>
/// XML双引号解析，这是一个终结表达式
/// </summary>
class XMLQuotationMarksExpression : public AbstractExpression {
public:

	XMLQuotationMarksExpression() = default;
	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	std::string str;
	bool error = false;
};



/// <summary>
/// XML属性中相等符号解析
/// </summary>
class XMLEqualsExpression : public AbstractExpression {
public:
	XMLEqualsExpression() = default;
	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	std::string propertyName;
	bool turnToQuotationOperatoin = false;
private:
	XMLQuotationMarksExpression qmarkExp;
};

/// <summary>
/// XML节点属性解析
/// </summary>
class XMLPropertyExpression : public AbstractExpression {
public:

	XMLPropertyExpression() = default;

	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	std::string nodeName;
	std::vector<std::string> propertys;
private:
	XMLEqualsExpression equalExp;
};


/// <summary>
/// xml node节点解析
/// </summary>
class XMLNodeExpression : public AbstractExpression {
public:

	XMLNodeExpression() = default;
	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	std::string str;
private:
	//XMLNodeStartAndEndExpression nodeExp;
};

class Content {


};



