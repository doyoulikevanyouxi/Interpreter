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
	// ͨ�� XMLAbstractExpression �̳�
	void RaiseError(const AbstractError& error) override;

};

/// <summary>
/// XML˫���Ž���������һ���ս���ʽ
/// </summary>
class XMLQuotationMarksExpression : public XMLExpression {
public:
	XMLQuotationMarksExpression(XMLInterpreter& controler) :XMLExpression(controler) {}
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
	XMLEqualsExpression(XMLInterpreter& controler) :XMLExpression(controler),qmarkExp(controler){}
	~XMLEqualsExpression() = default;
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
class XMLPropertyExpression : public XMLExpression {
public:

	XMLPropertyExpression(XMLInterpreter& controler) :XMLExpression(controler), equalExp(controler){}
	~XMLPropertyExpression() = default;
	// ͨ�� AbstractExpression �̳�
	void interpret(const char*& str) override;
	std::string nodeName;
	std::vector<std::string> propertys;
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
	void start(const char* data) {
		const char* x = data;
		pexp.interpret(x);
	}
private:
	XMLPropertyExpression pexp;
	
};



