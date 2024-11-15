#include "XMLInterpreter.h"
#include <iostream>

bool SpaceChar(const char ch) {
	return ch == ' ' || ch == '\n' || ch == '	';
}
//�������������������ַ����е�ǰ�˿հ��ַ�
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
			//�ڵ������ʼ
		case '<':
			if (matchStart) {
				std::cout << "�Ƿ��ַ� <" << std::endl;
				errorOcurred = true;
				return;
			}
			++str;
			CheckNodeType(str);
			matchStart = true;
			return;
			//�ڵ��������
		case '>':
			if (!matchStart) {
				std::cout << "�Ƿ��ַ� >" << std::endl;
				errorOcurred = true;
				return;
			}
			++str;
			hasBeenClosed = true;
			matchStart = false;
			return;
			//�ڵ����Ԥ����
		case '?':
		case '!':
		case '/':
			//���кϷ�����֤
			++str;
			if (SpaceChar(*str)) {
				skipSpace(str);
			}
			if (*str != '>') {
				errorOcurred = true;
				std::cout << "�Ƿ��ַ�/" << std::endl;
				return;
			}
			//����һ���Ասڵ�
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
		//ϵͳ���ͽڵ�
	case '?':
		++str;
		break;
		//�رսڵ�
	case '/':
		++str;
		break;
		//ע�ͽڵ�
	case '!':
		if (*(str+1) != '-' && *(str+2)!='-') {
			XMLError e("��Ч����������");
			RaiseError(e);
			return;
		}
		str += 2;
		break;
	//��Ч����
	case '\0':
	{
		XMLError e("������������");
		RaiseError(e);
	}
	break;
	//��ͨ�ڵ�
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
		// �ַ� = �Ǳ��ʽ�Ϸ���֤��֮һ
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
		//�Ƿ��ַ�
		if (!VertifyCharValidation(*str)) {
			XMLUnexpectedSymbol e(startAddres, str + 1);
			RaiseError(e);
			errorOcurred = true;
			break;
		}
		//�հ��ַ�����
		if (SpaceChar(*str)) {
			//�հ��ַ��ڱ��ʽ�����ºϷ�λ��    A�հ��ַ�=
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
	
	//�ַ��������� " Ϊ��ʼ�ַ�������ǰ���ǿ��ַ�+"Ҳ����
	if (*str != '"') {
		std::string erorrStr = "����ֵ������ \" Ϊ��ʼ�ַ���������" + *str;
		XMLError e(erorrStr.c_str());
		RaiseError(e);
		errorOcurred = true;
		return;
	}
	const char* startAddres = str;
	//�հ��ַ����������ط�  "�հ��ַ�string�հ��ַ�string�հ��ַ�"
	//������ǿհ��ַ�������
	//�հ��ַ������Ǳ������ֱ�Ӵ洢�ģ���������൱�ڽ��հ��ַ�������һ��������������������ʱ���հ��ַ���ӽ����ݼ�
	//��ʾβ����λ����ѭ���е��������
	const char* beginSpace = nullptr;	//��ʾ�հ��ַ���ǰһ���ַ�    ���� ��  " A �հ��ַ� B  " ��ʱbeginSpace����ʾ������ĸA����λ��
	//��ʾ�Ƿ���һ��������˫�������ݣ�һ�����������ݣ����� ""ƥ��
	bool hasBeenClosed = false;
	//��ʾ��ǰ�ַ���ǰһ���ַ��Ƿ��ǿհ��ַ����ñ�������ȷ���հ��ַ�����ı߽�
	bool preCharIsSpace = false;
	beginSpace = str;
	while (*str != '\0') {
		++str;
		if (SpaceChar(*str)) {
			preCharIsSpace = true;
			continue;
		}
		//��ƥ�䵽������ʶ��ֱ���Ƴ����ε�˫�������ݽ������̣����Ժ������ݼ�������ŵĿհ��ַ�
		if (*str == '"') {
			hasBeenClosed = true;
			++str;
			break;
		}
		//�Ƿ��ַ�
		if (!VertifyCharValidation(*str)) {
			XMLUnexpectedSymbol e(startAddres, str + 1);
			RaiseError(e);
			errorOcurred = true;
			return;
		}
		//����һ���ַ��ǿհ��ַ�ʱ
		if (preCharIsSpace) {
			preCharIsSpace = false;
			//�ַ���ͷ���Ŀհ��ַ���������
			if (*beginSpace == '"') {
				this->str += *str;
				continue;
			}
			//����ͷ��β�Ŀհ��ַ����뵽���ݼ���,�˲��������˴˴�ѭ���������ķǿ��ַ�
			for (int i = 1; (beginSpace + i) <= str; ++i) {
				this->str += *(beginSpace + i);
			}
		}
		else {
			this->str += *str;
		}
		//�ƶ����ַ�ͷ��λ��
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