#include "XMLInterpreter.h"
#include <iostream>

//�������������������ַ����е�ǰ�˿հ��ַ�
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
		std::string errorStr = "�ڵ��ַ����������ַ� < ��Ϊ��ʼ";
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
					XMLInvalidString e("��Ч���ַ� /");
					RaiseError(e);
					return;
				}
			}
			else if (*(str + 1) != '>') {
				XMLInvalidString e("��Ч���ַ� /");
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
		std::string errorStr = "δ�ҵ��ս��ַ� >";
		XMLInvalidNode e(errorStr.c_str());
		RaiseError(e);
		return;
	}
	std::cout <<"name:" << nodeName;
}


void XMLEqualsExpression::interpret(const char*& str)
{
	if (*str == ' ') {
		std::string errorStr = "���Ա��ʽӦ���Էǿջ������ַ���Ϊ��ʼ";
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
				std::string errorStr = "δָ����������";
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
				std::string errorStr = "δ�ҵ����� =";
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
		std::string errorStr = this->propertyName + ":����һ����Ч�����Ա��ʽ";
		XMLInvalidString e(errorStr.c_str());
		RaiseError(e);
		return;
		//std::cout << "��Ч�����Ա��ʽ" << std::endl;
	}
	//std::cout << propertyName << " : " << qmarkExp.str << std::endl;;
}

void XMLQuotationMarksExpression::interpret(const char*& str)
{
	this->str.clear();
	//�ַ��������� " Ϊ��ʼ�ַ�������ǰ���ǿ��ַ�+"Ҳ����
	if (*str != '"') {
		std::string erorrStr = "����ֵ������\"Ϊ��ʼ�ַ���������" + *str;
		XMLInvalidString e(erorrStr.c_str());
		RaiseError(e);
		return;
	}

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
		if (*str == ' ') {
			preCharIsSpace = true;
		}
		else {
			//��ƥ�䵽������ʶ��ֱ���Ƴ����ε�˫�������ݽ������̣����Ժ������ݼ�������ŵĿհ��ַ�
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
			//����һ���ַ��ǿհ��ַ�ʱ
			if(preCharIsSpace){
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
	}
	if (!hasBeenClosed) {
		std::string error = "δ�ҵ�����ֵ���ս��ַ�\"";
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

