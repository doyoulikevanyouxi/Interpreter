#include "XMLInterpreter.h"
#include <iostream>

//辅助函数：用于跳过字符串中的前端空白字符
void skipSpace(const char*& str) {
	while (*str != '\0' && *str == ' ') {
		++str;
	}
}


void XMLExpression::RaiseError(const AbstractError& error)
{
	if (xmlControler == nullptr)
		return;
	xmlControler->OnXMLErrorOccured(error);
}


void XMLPropertyExpression::interpret(const char*& str)
{
	if (*str != '<') {
		std::string errorStr = "节点字符串必须以字符 < 作为起始";
		XMLInvalidNode e(errorStr.c_str());
		RaiseError(e);
		return;
	}
	bool hasBeenClosed = false;
	bool needNodeName = true;
	while (*str != '\0') {
		if (*str == '<') {
			++str;
			continue;
		}
		if (*str == '>') {
			hasBeenClosed = true;
			break;
		}
		if (*str == ' ') {
			if (nodeName.empty())
				needNodeName = true;
			else
				needNodeName = false;
			skipSpace(str);
			continue;
		}
		if (*str == '/') { 
			if (*(str + 1) == ' ') {
				++str;
				skipSpace(str);
				if (*str != '>') {
					XMLInvalidString e("无效的字符 /");
					RaiseError(e);
					return;
				}
			}
			else if (*(str + 1) != '>') {
				XMLInvalidString e("无效的字符 /");
				RaiseError(e);
				return;
			}
			else {
				hasBeenClosed = true;
				break;
			}
		
		}
		else {
			if (needNodeName) {
				nodeName += *str;
			}
			else {
				equalExp.interpret(str);
			}
			++str;
		}
	}
	if (!hasBeenClosed) {
		std::string errorStr = "未找点终结字符 >";
		XMLInvalidNode e(errorStr.c_str());
		RaiseError(e);
		return;
	}
	std::cout <<"name:" << nodeName;
}


void XMLEqualsExpression::interpret(const char*& str)
{
	if (*str == ' ') {
		std::string errorStr = "属性表达式应该以非空或特殊字符作为起始";
		XMLInvalidString e(errorStr.c_str());
		RaiseError(e);
		return;
	}

	propertyName.clear();
	const char* preSpace = str;
	bool hasEquals = false;
	while (*str != '\0') {
		if (*str == '=') {
			if (this->propertyName.empty()) {
				std::string errorStr = "未指定属性名称";
				XMLInvalidString e(errorStr.c_str());
				return;
			}
			hasEquals = true;
			++str;
			skipSpace(str);
			qmarkExp.interpret(str);
			break;
		}
		if (*str == ' ') {
			skipSpace(str);
			if (*str != '=') {
				std::string errorStr = "未找到符号 =";
				XMLInvalidString e(errorStr.c_str());
				RaiseError(e);
				return;
			}
			continue;
		}
		this->propertyName += *str;
		++str;
	}
	if (!hasEquals) {
		std::string errorStr = this->propertyName + ":不是一个有效的属性表达式";
		XMLInvalidString e(errorStr.c_str());
		RaiseError(e);
		return;
		//std::cout << "无效的属性表达式" << std::endl;
	}
	//std::cout << propertyName << " : " << qmarkExp.str << std::endl;;
}

void XMLQuotationMarksExpression::interpret(const char*& str)
{
	this->str.clear();
	//字符串必须以 " 为起始字符，就算前边是空字符+"也不行
	if (*str != '"') {
		std::string erorrStr = "属性值必须以\"为起始字符，而不是" + *str;
		XMLInvalidString e(erorrStr.c_str());
		RaiseError(e);
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
			if (*str == '<') {
				XMLUnexpectedTerminalSymbol e('<');
				RaiseError(e);
				return;
			}
			if (*str == '>') {
				XMLUnexpectedTerminalSymbol e('>');
				RaiseError(e);
				return;
			}
			//当上一个字符是空白字符时
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
		std::string error = "未找到属性值的终结字符\"";
		XMLInvalidString e(error.c_str());
		RaiseError(e);
		return;
	}
	std::cout << this->str << std::endl;
}

void XMLInterpreter::OnXMLErrorOccured(const AbstractError& e)
{
	e.ErrorMessage();
}

