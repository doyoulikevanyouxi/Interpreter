#pragma once
#include<functional>
class AbstractError;
class XMLInterpreter;
struct Node;
class XMLAbstractExpression {
public:
	XMLAbstractExpression(XMLInterpreter& controler) : xmlControler(&controler) {}
	virtual void interpret(const char*& str) = 0;
protected:
	virtual void RaiseError(const AbstractError& error) = 0;
protected:
	XMLInterpreter* xmlControler;
};