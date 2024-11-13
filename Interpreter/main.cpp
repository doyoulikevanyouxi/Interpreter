
#include"XMLInterpreter.h"
#include<iostream>

int main() {
	
	const char* data = R"(< nodeName w1=  "int20" w2="20"  w3="string">)";
	XMLPropertyExpression x;
	//XMLEqualsExpression x;
	x.interpret(data);

	return 0;
}