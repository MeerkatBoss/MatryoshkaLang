#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Logger.h"
class ErrorHandler
{
public:
	bool hadError()
	{
		return errorFound;
	}
	virtual void reportError(int line, std::string message)
	{
		errorFound = true;
	};
private:
	bool errorFound = false;
};

class ConsoleErrorHandler : public ErrorHandler
{
public:
	void reportError(int line, std::string message) override
	{
		__super::reportError(line, message);
		std::cout << "Ошибка на строке " << line << ": " << message << std::endl;
	}
};

class LogErrorHandler : public ErrorHandler
{
public:
	LogErrorHandler(Logger* logger)
	{
		this->logger = logger;
	}
	void reportError(int line, std::string message) override
	{
		__super::reportError(line, message);
		logger->log("\tОшибка на строке " + std::to_string(line) + ": " + message);
	}
private:
	Logger* logger;
};