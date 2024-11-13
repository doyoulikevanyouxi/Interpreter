
#include"XMLInterpreter.h"
#include<iostream>

int main() {
	
	const char* data = R"(<xnode w1="int20" w2="20"  w3="string" x/>)";
	XMLInterpreter p;
	p.start(data);
	

	return 0;
}