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
	// ͨ�� XMLAbstractExpression �̳�
	void RaiseError(const AbstractError& error) override;
};

/// <summary>
/// XML˫���Ž���������һ���ս���ʽ
/// </summary>
class XMLQuotationMarksExpression : public XMLExpression {
public:
	XMLQuotationMarksExpression(XMLInterpreter& controler) :XMLExpression(controler){}
	~XMLQuotationMarksExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	std::string str;
};

/// <summary>
/// XML��������ȷ��Ž���
/// </summary>
class XMLEqualsExpression : public XMLExpression {
public:
	XMLEqualsExpression(XMLInterpreter& controler) :XMLExpression(controler),qmarkExp(controler), node(nullptr) {}
	~XMLEqualsExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	inline void SetNode(Node& node) { this->node = &node; }
public:
	std::string propertyName;
private:
	XMLQuotationMarksExpression qmarkExp;
	Node* node;
};

/// <summary>
/// XML�ڵ����Խ���
/// </summary>
class XMLPropertyExpression : public XMLExpression {
public:

	XMLPropertyExpression(XMLInterpreter& controler) :XMLExpression(controler), equalExp(controler){}
	~XMLPropertyExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	std::string nodeName;
private:
	XMLEqualsExpression equalExp;
};


/// <summary>
/// XML�����������ƽ������̣��������
/// </summary>
class XMLInterpreter {
public:
	XMLInterpreter():pexp(*this){}
	//�������
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



