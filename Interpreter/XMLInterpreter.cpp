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



//辅助函数：用于验证字符的合法性
//bool vertifiedCharValidation(const char chr) {
//	if()
//}

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
	nodeName.clear();

	bool hasBeenClosed = false;
	bool needNodeName = true;
	bool isClosedNode = false;
	Node node;
	while (*str != '\0') {
		if (*str == '<') {
			++str;
			if (SpaceChar(*str)) {
				skipSpace(str);
			}
			if (*str == '/') {
				isClosedNode = true;
				++str;
			}
			continue;
		}
		if (*str == '>') {
			hasBeenClosed = true;
			++str;
			break;
		}
		if (SpaceChar(*str)) {
			if (nodeName.empty())
				needNodeName = true;
			else
				needNodeName = false;
			skipSpace(str);
			continue;
		}
		if (*str == '=') {
			XMLInvalidString e("无效的字符 =");
			RaiseError(e);
			errorOcurred = true;
			return;
		}
		if (*str == '"') {
			XMLInvalidString e("无效的字符 \"");
			RaiseError(e);
			errorOcurred = true;
			return;
		}
		if (*str == '/') {
			++str;
			if (!SpaceChar(*str) && *str != '>') {
				XMLInvalidString e("无效的字符 /");
				RaiseError(e);
				errorOcurred = true;
				return;
			}
			if (*str == '>') {
				hasBeenClosed = true;
				break;
			}
			skipSpace(str);
			if (*str != '>') {
				XMLInvalidString e("无效的字符 /");
				RaiseError(e);
				errorOcurred = true;
				return;
			}
			hasBeenClosed = true;
			break;
		}
		if (needNodeName) {
			nodeName += *str;
			++str;
		}
		else {
			equalExp.SetNode(node);
			equalExp.interpret(str);
		}

	}
	if (!hasBeenClosed) {
		std::string errorStr = "未找点终结字符 >";
		XMLInvalidNode e(errorStr.c_str());
		RaiseError(e);
		errorOcurred = true;
		return;
	}
	if (isClosedNode)
		return;
	node.NodeName = nodeName;
	xmlControler->AddNode(node);
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
	bool hasName = false;
	bool hasValue = false;
	bool invalide = false;
	while (*str != '\0') {
		//在未进行属性值解析时，可能存在非法字符
		if (*str == '/') {
			/*XMLInvalidSymbol e('/');
			RaiseError(e);*/
			errorOcurred = true;
			break;
		}
		// 字符 = 是表达式合法的证明之一
		if (*str == '=') {
			if (this->propertyName.empty()) {
				break;
			}
			hasName = true;
			hasEquals = true;
			++str;
			continue;
		}
		if (*str == '"') {
			//存在双引号时，需要判断是否已经存在=
			if (!hasEquals) {
				break;
			}
			qmarkExp.interpret(str);
			if (!qmarkExp.errorOcurred)
				hasValue = true;
			break;
		}
		if (SpaceChar(*str)) {
			//空白字符在表达式有以下合法位置    A空白字符=空白字符"B"
			skipSpace(str);
			//所以需要检查空白字符后接字符
			//该处检查并没有使用拷贝字符串指针，是因为，如果是合法的表达式，那么空白字符位置本身就需要被跳过
			//如果是非法表达式,也就是该字符串不是一个有效的属性，那么跳到下一个非空字符或者空字符都可以，外层调用会屏蔽掉这个差异
			if (*str != '=' && *str != '"') {
				break;
			}
			continue;
		}
		this->propertyName += *str;
		++str;
	}
	if (!hasEquals || !hasName || !hasValue) {
		std::string errorStr = this->propertyName + ":不是一个有效的属性表达式";
		XMLInvalidString e(errorStr.c_str());
		RaiseError(e);
		errorOcurred = true;
		return;
	}
	if (node != nullptr) {
		node->propertys.insert(std::make_pair(this->propertyName, qmarkExp.str));
	}
}

void XMLQuotationMarksExpression::interpret(const char*& str)
{
	this->str.clear();
	//字符串必须以 " 为起始字符，就算前边是空字符+"也不行
	if (*str != '"') {
		std::string erorrStr = "属性值必须以\"为起始字符，而不是" + *str;
		XMLInvalidString e(erorrStr.c_str());
		RaiseError(e);
		errorOcurred = true;
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
		if (*str == '<') {
			XMLUnexpectedTerminalSymbol e('<');
			RaiseError(e);
			errorOcurred = true;

			return;
		}
		if (*str == '>') {
			XMLUnexpectedTerminalSymbol e('>');
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
		std::string error = "未找到属性值的终结字符\"";
		XMLInvalidString e(error.c_str());
		RaiseError(e);
		errorOcurred = true;

		return;
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
