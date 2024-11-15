#pragma once
#include<string>
enum class ErrorType {
	XMLError
};

enum class ErrorSubType {
	XMLInvalidString,
	XMLUnexpectedTerminalSymbol,
	XMLUnkownString
};

class AbstractError {
public:
	inline unsigned int GetErrorCode() const noexcept { return errorCode; }
	virtual void ErrorMessage() const = 0;
protected:
	unsigned int errorCode;
	std::string message;
};

