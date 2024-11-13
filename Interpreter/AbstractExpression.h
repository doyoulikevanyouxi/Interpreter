#pragma once
#include<functional>
class Error {
public:
	Error() = default;
	inline uint16_t GetErrorCode() noexcept { return errorCode; }
	virtual void ErrorMessage() = 0;
private:
	uint16_t errorCode;
};


class AbstractExpression
{
protected:
	virtual void interpret(const char *& str) = 0;
};

class XMLAbstractExpression : public AbstractExpression {
public:
	void SetErrorHandleCallBack(std::function& callback) {}
protected:
	virtual void OnErrorOccured() = 0;
public:
	Error& error;
};