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


class AllowedChars {
public:
	enum Range {
		defualt = 1 << 0,
		None = 1 << 1,
		number = 1 << 2,
		character = 1 << 3,
		specifyChar = 1 << 4
	};
	//是否是允许值--允许值表示，允许该字符通过测试，但不是期望的值，允许值是一个范围，期望值是一个具体的
	bool Allowed(const char& ch);
	std::vector<char> allowedChar;	//存在几个问题，该字符的，但是该期望可能和其他字符的期望是一样的，这就导致，另一个期望无法弹出栈。
	//例如 < >   <  /> 中< 与/的期望字符都是> 以及 </ x >  中<的期望不止一个/
//期望数据 范围类型
	Range allowedRange = Range::defualt;
};

class ExpectedChars {
public:
	//是否是期望值
	bool Matched(const char& ch);

public:
	//为了提高速度，本身期望数据的不会产生变动，char数据应该进行排序.
	// 实际情况是，排序可以不适用，因为期望数据不可能太多，绝大部分情况为10个以内
	//存放期望的数据 固定类型
	std::vector<char> expected;
	
	bool invalid = false;
};

/// <summary>
/// XML输入期望解释器：这个是根据输入的字符进行接下来字符的输入约束，这个相比下面的解释器更加的灵活
/// 相应的，因为根据输入会创建期望值栈，需要更多的性能和空间
/// </summary>
class XMLEexpectIntergret {
public:
	XMLEexpectIntergret() = default;
	void interpret(const char* str);
	ExpectedChars charExpect(const char& chr,bool isExpected = false);
private:
	//首先有一个期望字符栈，该栈的栈顶表示下一个字符的期望值，以及非期望值
	std::stack<ExpectedChars> expect;
	AllowedChars allowedChar;
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



