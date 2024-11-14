#pragma once
#include<fstream>
#include<string>
#include<map>
#include "AbstractExpression.h"
#include <vector>
#include "XMLError.h"
#include<stack>
struct Node {
	std::string NodeName;
	std::map<std::string, std::string> propertys;
};

class XMLExpression : public XMLAbstractExpression {
public:
	XMLExpression(XMLInterpreter& controler) :XMLAbstractExpression(controler) {}
public:
	bool errorOcurred = false;
protected:
	// 通过 XMLAbstractExpression 继承
	void RaiseError(const AbstractError& error) override;
};

/// <summary>
/// XML双引号解析，这是一个终结表达式
/// </summary>
class XMLQuotationMarksExpression : public XMLExpression {
public:
	XMLQuotationMarksExpression(XMLInterpreter& controler) :XMLExpression(controler){}
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
	XMLEqualsExpression(XMLInterpreter& controler) :XMLExpression(controler),qmarkExp(controler), node(nullptr) {}
	~XMLEqualsExpression() = default;
	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	inline void SetNode(Node& node) { this->node = &node; }
public:
	std::string propertyName;
private:
	XMLQuotationMarksExpression qmarkExp;
	Node* node;
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
	void start(const char* data);
	void AddNode(Node& node) {
		nodes.emplace_back(node);
	}
private:
	XMLPropertyExpression pexp;
	std::vector<Node> nodes;
	bool noError = true;
};



