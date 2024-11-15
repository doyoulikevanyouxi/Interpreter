#include "XMLInterpreter.h"
#include <iostream>

bool SpaceChar(const char ch) {
	return ch == ' ' || ch == '\n' || ch == '	';
}
//辅助函数：用于跳过字符串中的前端空白字符
void skipSpace(const char*& str) {
	while (*str != '\0') {
		if (!SpaceChar(*str))
			break;
		++str;
	}
}

bool IsNum(const char& chr) {
	if (chr >= '0' && chr < '9')
		return true;
	return false;
}

bool IsLetter(const char& chr) {
	if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'))
		return true;
	return false;
}

void XMLExpression::RaiseError(const AbstractError& error)
{
	if (xmlControler == nullptr)
		return;
	xmlControler->OnXMLErrorOccured(error);
}

void XMLPropertyExpression::interpret(const char*& str)
{
	errorOcurred = false;
	matchStart = false;
	hasBeenClosed = false;
	if (SpaceChar(*str)) {
		skipSpace(str);
	}
	if (*str != '<') {
		XMLInvalidNode e;
		RaiseError(e);
		return;
	}
	
	Node node;
	bool toSaveNodeName = true;
	while (*str != '\0') {
		if (hasBeenClosed)
			break;
		if (SpaceChar(*str)) {
			if (toSaveNodeName && !node.NodeName.empty())
				toSaveNodeName = false;
			skipSpace(str);
		}
		if (!IsLetter(*str) && !IsNum(*str)) {
			HandledSpecialChar(str);
			if (errorOcurred)
				break;
			continue;
		}
		if (toSaveNodeName) {
			node.NodeName += *str;
			++str;
			continue;
		}
		equalExp.SetNode(node);
		equalExp.interpret(str);
	}
	if (errorOcurred) {
		XMLInvalidNode e;
		RaiseError(e);
		return;
	}
	xmlControler->AddNode(node);
}

void XMLPropertyExpression::HandledSpecialChar(const char*& str)
{
		switch (*str)
		{
			//节点解析开始
		case '<':
			if (matchStart) {
				std::cout << "非法字符 <" << std::endl;
				errorOcurred = true;
				return;
			}
			++str;
			CheckNodeType(str);
			matchStart = true;
			return;
			//节点解析结束
		case '>':
			if (!matchStart) {
				std::cout << "非法字符 >" << std::endl;
				errorOcurred = true;
				return;
			}
			++str;
			hasBeenClosed = true;
			matchStart = false;
			return;
			//节点解析预结束
		case '?':
		case '!':
		case '/':
			//进行合法性验证
			++str;
			if (SpaceChar(*str)) {
				skipSpace(str);
			}
			if (*str != '>') {
				errorOcurred = true;
				std::cout << "非法字符/" << std::endl;
				return;
			}
			//这是一个自闭节点
			break;
		default:
			errorOcurred = true;
			break;
		}
}


void XMLPropertyExpression::CheckNodeType(const char*& str)
{
	if (SpaceChar(*str)) {
		skipSpace(str);
	}
	switch (*str)
	{
		//系统类型节点
	case '?':
		++str;
		break;
		//关闭节点
	case '/':
		++str;
		break;
		//注释节点
	case '!':
		if (*(str+1) != '-' && *(str+2)!='-') {
			XMLError e("无效的数据类型");
			RaiseError(e);
			return;
		}
		str += 2;
		break;
	//无效数据
	case '\0':
	{
		XMLError e("不完整的数据");
		RaiseError(e);
	}
	break;
	//普通节点
	default:
		break;
	}
}

void XMLEqualsExpression::interpret(const char*& str)
{
	errorOcurred = false;
	if (SpaceChar(*str)) {
		skipSpace(str);
	}

	std::string propertyName;
	const char* startAddres = str;
	bool hasEquals = false;
	bool hasName = false;
	bool hasValue = false;
	bool invalide = false;
	while (*str != '\0') {
		// 字符 = 是表达式合法的证明之一
		if (*str == '=') {
			if (propertyName.empty()) {
				break;
			}
			hasName = true;
			hasEquals = true;
			++str;
			qmarkExp.interpret(str);
			if (!qmarkExp.errorOcurred) {
				hasValue = true;
			}
			break;
		}
		//非法字符
		if (!VertifyCharValidation(*str)) {
			XMLUnexpectedSymbol e(startAddres, str + 1);
			RaiseError(e);
			errorOcurred = true;
			break;
		}
		//空白字符处理
		if (SpaceChar(*str)) {
			//空白字符在表达式有以下合法位置    A空白字符=
			skipSpace(str);
			if (*str != '=' ) {

				break;
			}
			continue;
		}
		propertyName += *str;
		++str;
	}
	if (!hasEquals || !hasName || !hasValue) {

		XMLInvalidString e(startAddres,str);
		RaiseError(e);
		errorOcurred = true;
		return;
	}
	if (node != nullptr) {
		node->propertys.insert(std::make_pair(propertyName, qmarkExp.str));
	}
}

inline bool XMLEqualsExpression::VertifyCharValidation(const char& chr)
{
	return  IsLetter(chr) || IsNum(chr) || chr == '-' || chr ==':';
}

void XMLQuotationMarksExpression::interpret(const char*& str)
{
	errorOcurred = false;
	this->str.clear();
	if (SpaceChar(*str)) {
		skipSpace(str);
	}
	
	//字符串必须以 " 为起始字符，就算前边是空字符+"也不行
	if (*str != '"') {
		std::string erorrStr = "属性值必须以 \" 为起始字符，而不是" + *str;
		XMLError e(erorrStr.c_str());
		RaiseError(e);
		errorOcurred = true;
		return;
	}
	const char* startAddres = str;
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
		if (SpaceChar(*str)) {
			preCharIsSpace = true;
			continue;
		}
		//当匹配到结束标识，直接推出本次的双引号数据解析过程，可以忽略数据集后紧挨着的空白字符
		if (*str == '"') {
			hasBeenClosed = true;
			++str;
			break;
		}
		//非法字符
		if (!VertifyCharValidation(*str)) {
			XMLUnexpectedSymbol e(startAddres, str + 1);
			RaiseError(e);
			errorOcurred = true;
			return;
		}
		//当上一个字符是空白字符时
		if (preCharIsSpace) {
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
	if (!hasBeenClosed) {
		XMLUnexpectedTerminalSymbol e(startAddres, str, '"', false);
		RaiseError(e);
		errorOcurred = true;
		return;
	}
}

inline bool XMLQuotationMarksExpression::VertifyCharValidation(const char& chr)
{
	switch (chr)
	{
	case '>':
	{
		return false;
	}
	case '<':
	{
		return false;
	}
	default:
		return true;
	}

}

void XMLInterpreter::OnXMLErrorOccured(const AbstractError& e)
{
	e.ErrorMessage();
	noError = false;
}

void XMLInterpreter::start(const char* data)
{
	const char* x = data;
	while (*x != '\0') {
		if (*x == '<') {
			pexp.interpret(x);
			if (pexp.errorOcurred)
				break;
		}
		else {
			++x;
		}
	}
	for (auto& node : nodes) {
		std::cout << node.NodeName << "{" << std::endl;
		for (auto& property : node.propertys) {
			std::cout << "	" << property.first << " = " << property.second << std::endl;
		}
		std::cout << "}" << std::endl;
	}
}