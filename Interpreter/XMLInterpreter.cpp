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



//����������������֤�ַ��ĺϷ���
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
		std::string errorStr = "�ڵ��ַ����������ַ� < ��Ϊ��ʼ";
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
			XMLInvalidString e("��Ч���ַ� =");
			RaiseError(e);
			errorOcurred = true;
			return;
		}
		if (*str == '"') {
			XMLInvalidString e("��Ч���ַ� \"");
			RaiseError(e);
			errorOcurred = true;
			return;
		}
		if (*str == '/') {
			++str;
			if (!SpaceChar(*str) && *str != '>') {
				XMLInvalidString e("��Ч���ַ� /");
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
				XMLInvalidString e("��Ч���ַ� /");
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
		std::string errorStr = "δ�ҵ��ս��ַ� >";
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
		std::string errorStr = "���Ա��ʽӦ���Էǿջ������ַ���Ϊ��ʼ";
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
		//��δ��������ֵ����ʱ�����ܴ��ڷǷ��ַ�
		if (*str == '/') {
			/*XMLInvalidSymbol e('/');
			RaiseError(e);*/
			errorOcurred = true;
			break;
		}
		// �ַ� = �Ǳ��ʽ�Ϸ���֤��֮һ
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
			//����˫����ʱ����Ҫ�ж��Ƿ��Ѿ�����=
			if (!hasEquals) {
				break;
			}
			qmarkExp.interpret(str);
			if (!qmarkExp.errorOcurred)
				hasValue = true;
			break;
		}
		if (SpaceChar(*str)) {
			//�հ��ַ��ڱ��ʽ�����ºϷ�λ��    A�հ��ַ�=�հ��ַ�"B"
			skipSpace(str);
			//������Ҫ���հ��ַ�����ַ�
			//�ô���鲢û��ʹ�ÿ����ַ���ָ�룬����Ϊ������ǺϷ��ı��ʽ����ô�հ��ַ�λ�ñ������Ҫ������
			//����ǷǷ����ʽ,Ҳ���Ǹ��ַ�������һ����Ч�����ԣ���ô������һ���ǿ��ַ����߿��ַ������ԣ������û����ε��������
			if (*str != '=' && *str != '"') {
				break;
			}
			continue;
		}
		this->propertyName += *str;
		++str;
	}
	if (!hasEquals || !hasName || !hasValue) {
		std::string errorStr = this->propertyName + ":����һ����Ч�����Ա��ʽ";
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
	//�ַ��������� " Ϊ��ʼ�ַ�������ǰ���ǿ��ַ�+"Ҳ����
	if (*str != '"') {
		std::string erorrStr = "����ֵ������\"Ϊ��ʼ�ַ���������" + *str;
		XMLInvalidString e(erorrStr.c_str());
		RaiseError(e);
		errorOcurred = true;
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
		std::string error = "δ�ҵ�����ֵ���ս��ַ�\"";
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
