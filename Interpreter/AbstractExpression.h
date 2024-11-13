#pragma once
#include<functional>
class AbstractError;
class XMLInterpreter;
class AbstractExpression
{
public:
	virtual void interpret(const char *& str) = 0;
};

class XMLAbstractExpression : public AbstractExpression {
public:
	XMLAbstractExpression(XMLInterpreter& controler) : xmlControler(&controler) {}
protected:
	virtual void RaiseError(const AbstractError& error) = 0;
protected:
	XMLInterpreter* xmlControler;
};