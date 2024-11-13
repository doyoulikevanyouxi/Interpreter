#include "XMLInterpreter.h"
#include <iostream>

//辅助函数：用于跳过字符串中的前端空白字符
void skipSpace(const char*& str) {
	while (*str != '\0' && *str == ' ') {
		++str;
	}
}

void XMLNodeExpression::interpret(const char*& str)
{
	/*while (*str != '\0') {
		if (*str == '<')
			nodeExp.interpret(str);
	}*/
}


void XMLPropertyExpression::interpret(const char*& str)
{
	bool matchStart = false;
	while (*str != '\0') {
		if (*str == '<') {
			matchStart = true;
			++str;
			skipSpace(str);
		}
		if (*str == '>') {
			matchStart = false;
			break;
		}
		if (matchStart) {
			if (*str != ' ') {
				nodeName += *str;
				++str;
				continue;
			}
			else {
				if (nodeName.empty()) {
					std::cout << "节点必须要有一个名称" << std::endl;
					return;
				}
				else {
					skipSpace(str);
					equalExp.interpret(str);
					++str;
				}
			}
		}
	}
	std::cout << nodeName ;
}

void XMLEqualsExpression::interpret(const char*& str)
{
	propertyName.clear();
	const char* preSpace = str;
	bool hasEquals = false;
	while (*str != '\0') {
		if (*str == '=') {
			hasEquals = true;
			++str;
			skipSpace(str);
			qmarkExp.interpret(str);
			break;
		}
		this->propertyName += *str;
		++str;
	}
	if (!hasEquals) {
		std::cout << "无效的属性表达式" << std::endl;
	}
	std::cout << propertyName << " : " << qmarkExp.str << std::endl;;
}

void XMLQuotationMarksExpression::interpret(const char*& str)
{
	this->str.clear();
	if (*str != '"') {
		error = true;
		std::cout << "属性值需要双引号作为开头，而不是[" << *str << "]" << std::endl;
		return;
	}

	//空白字符存在三个地方  "空白字符string空白字符string空白字符"
	//用来标记空白字符的区间
	//空白字符并不是遍历后就直接存储的，下面操纵相当于将空白字符放在了一个缓冲区，当条件满足时将空白字符添加进数据集
	//表示尾部的位置由循环中的数据替代
	const char* beginSpace = nullptr;	//表示空白字符的前一个字符    例如 ：  " A 空白字符 B  " 此时beginSpace所表示就是字母A所在位置
	//表示是否是一个完整的双引号数据，一个完整的数据，必须 ""匹配
	bool hasBeenClosed = false;
	//表示当前字符的前一个字符是否是空白字符，该变量用来确定空白字符区域的边界
	bool preCharIsSpace = false;
	beginSpace = str;
	while (*str != '\0') {
		++str;
		if (*str == ' ') {
			preCharIsSpace = true;
		}
		else {
			//当匹配到结束标识，直接推出本次的双引号数据解析过程，可以忽略数据集后紧挨着的空白字符
			if (*str == '"') {
				hasBeenClosed = true;
				break;
			}
			if(preCharIsSpace){
				preCharIsSpace = false;
				//字符集头部的空白字符将被忽略
				if (*beginSpace == '"') {
					this->str += *str;
					continue;
				}
				//将非头非尾的空白字符加入到数据集中,此操作包含了此次循环所包括的非空字符
				for (int i = 1; (beginSpace + i) <= str; ++i) {
					this->str += *(beginSpace + i);
				}
			}
			else {
				this->str += *str;
			}
			//移动空字符头部位置
			beginSpace = str;
		}
	}
	if (!hasBeenClosed) {
		std::cout << "没有配对的双引号" << std::endl;
		error = true;
		return;
	}
}
