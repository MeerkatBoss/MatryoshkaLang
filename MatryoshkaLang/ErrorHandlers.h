#pragma once
#include <string>
#include <iostream>
class IErrorHandler
{
public:
	bool errorFound = false;
	virtual void reportError(int line, std::string message)
	{
		errorFound = true;
	};
};

class ConsoleErrorHandler : IErrorHandler
{
public:
	void reportError(int line, std::string message) override
	{
		__super::reportError(line, message);
		std::cout << "Ошибка на строке " << line << ": " << message << std::endl;
	}
};