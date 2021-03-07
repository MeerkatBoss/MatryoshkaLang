#pragma once
#include <string>
#include <iostream>
#include <fstream>
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

class LogErrorHandler : public ErrorHandler	//TODO: figure out the way to clear file on new run
{
public:
	void reportError(int line, std::string message) override
	{
		__super::reportError(line, message);
		std::ofstream fout("log.txt", std::ios_base::app);
		fout << "Ошибка на строке " << line << ": " << message << std::endl;
		fout.close();
	}
};