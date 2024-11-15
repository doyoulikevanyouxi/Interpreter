#pragma once
#include<fstream>
#include<string>
#include<map>
#include "AbstractExpression.h"
#include <vector>
#include "XMLError.h"
#include<stack>
enum NodeType {
	defualt,
	systems
};
struct Node {
	NodeType type;
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
	std::unique_ptr<AbstractError> xmlError;
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
	//字符合法性检擦
	inline bool VertifyCharValidation(const char& chr);
	std::string str;
	//不允许存在的字符
	std::vector<char> notAllowedChar;
private:

};

/// <summary>
/// XML属性中相等符号解析
/// 属性表达式只有以下有效形式   propertyName空白字符=value
///value由另一个表达式处理，本解析不关系value是什么样的
/// </summary>
class XMLEqualsExpression : public XMLExpression {
public:
	XMLEqualsExpression(XMLInterpreter& controler) :XMLExpression(controler),qmarkExp(controler), node(nullptr) {}
	~XMLEqualsExpression() = default;
	// 通过 AbstractExpression 继承
	void interpret(const char*& str) override;
	//空白字符处理
	inline bool VertifyCharValidation(const char& chr);
	inline void SetNode(Node& node) { this->node = &node; }

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
	//对第一个字符进行检查
	void HandledSpecialChar(const char*& str);

	//节点类型萃取
	void CheckNodeType(const char*& str);
	std::string nodeName;
private:
	XMLEqualsExpression equalExp;
	bool matchStart = false;
	bool hasBeenClosed = false;
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



