#include "XMLInterpreter.h"
#include <iostream>

//�������������������ַ����е�ǰ�˿հ��ַ�
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
					std::cout << "�ڵ����Ҫ��һ������" << std::endl;
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
		std::cout << "��Ч�����Ա��ʽ" << std::endl;
	}
	std::cout << propertyName << " : " << qmarkExp.str << std::endl;;
}

void XMLQuotationMarksExpression::interpret(const char*& str)
{
	this->str.clear();
	if (*str != '"') {
		error = true;
		std::cout << "����ֵ��Ҫ˫������Ϊ��ͷ��������[" << *str << "]" << std::endl;
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
		std::cout << "û����Ե�˫����" << std::endl;
		error = true;
		return;
	}
}
