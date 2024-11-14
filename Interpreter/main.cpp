
#include"XMLInterpreter.h"
#include<iostream>
#include<fstream>
int main() {
	XMLInterpreter p;
	std::string fileName = "test.txt";
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cout << "无法打开文件 " << fileName << std::endl;
		return 0;
	}
	std::string str;
	std::string st;
	while (std::getline(file, str)) {
		st += str;
	}
	p.start(st.c_str());
	file.close();
	return 0;
}