#pragma once
#include<fstream>
#include<string>
#include<map>
#include "AbstractExpression.h"
#include <vector>
#include "XMLError.h"



class XMLExpression : public XMLAbstractExpression {
public:
	XMLExpression(XMLInterpreter& controler) :XMLAbstractExpression(controler) {}
protected:
	// 通过 XMLAbstractExpression 继承
	void RaiseError(const AbstractError& error) override;

};

/// <summary>
/// XML双引号解析，这是一个终结表达式
/// </summary>
class XMLQuotationMarksExpression : public XMLExpression {
public:
	XMLQuotationMarksExpression(XMLInterpreter& controler) :XMLExpression(controler) {}
	~XMLQuotationMarksExpression() = default;
	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	std::string str;
};

/// <summary>
/// XML属性中相等符号解析
/// </summary>
class XMLEqualsExpression : public XMLExpression {
public:
	XMLEqualsExpression(XMLInterpreter& controler) :XMLExpression(controler),qmarkExp(controler){}
	~XMLEqualsExpression() = default;
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
class XMLPropertyExpression : public XMLExpression {
public:

	XMLPropertyExpression(XMLInterpreter& controler) :XMLExpression(controler), equalExp(controler){}
	~XMLPropertyExpression() = default;
	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	std::string nodeName;
	std::vector<std::string> propertys;
private:
	XMLEqualsExpression equalExp;
};


/// <summary>
/// XML解释器：控制解释流程，错误处理等
/// </summary>
class XMLInterpreter {

public:
	XMLInterpreter():pexp(*this){}
	//处理错误
	void OnXMLErrorOccured(const AbstractError& e);
	void start(const char* data) {
		const char* x = data;
		pexp.interpret(x);
	}
private:
	XMLPropertyExpression pexp;
	
};



