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
	// ͨ�� XMLAbstractExpression �̳�
	void RaiseError(const AbstractError& error) override;
	std::unique_ptr<AbstractError> xmlError;
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
	//�ַ��Ϸ��Լ��
	inline bool VertifyCharValidation(const char& chr);
	std::string str;
	//��������ڵ��ַ�
	std::vector<char> notAllowedChar;
private:

};

/// <summary>
/// XML��������ȷ��Ž���
/// ���Ա��ʽֻ��������Ч��ʽ   propertyName�հ��ַ�=value
///value����һ�����ʽ��������������ϵvalue��ʲô����
/// </summary>
class XMLEqualsExpression : public XMLExpression {
public:
	XMLEqualsExpression(XMLInterpreter& controler) :XMLExpression(controler),qmarkExp(controler), node(nullptr) {}
	~XMLEqualsExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	//�հ��ַ�����
	inline bool VertifyCharValidation(const char& chr);
	inline void SetNode(Node& node) { this->node = &node; }

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
	//�Ե�һ���ַ����м��
	void HandledSpecialChar(const char*& str);

	//�ڵ�������ȡ
	void CheckNodeType(const char*& str);
	std::string nodeName;
private:
	XMLEqualsExpression equalExp;
	bool matchStart = false;
	bool hasBeenClosed = false;
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



