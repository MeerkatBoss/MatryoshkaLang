#include <iostream>
#include <fstream>
#include <string>
#include <clocale>
#include <vector>
#include "Lexer.h"
#include "ErrorHandlers.h"
#include "Token.h"
#include "TokenType.h"
#include "Logger.h"

int main()
{
	setlocale(LC_ALL, "rus");
	std::ifstream fin("input.txt");
	std::string str;
	std::string text = "";
	while (getline(fin, str))
		text += str + '\n';
	Logger* logger = new Logger("log.txt");
	ErrorHandler* errorHandler = new LogErrorHandler(logger);
	Lexer lexer(text, errorHandler);
	std::vector<Token> tokens = lexer.getTokens();	//TODO: make a function to print tokens
	if (errorHandler->hadError())
	{
		logger->log("При чтении найдены ошибки, процесс остановлен.");
		exit(65);
	}
		
	logger->log("Чтение завершено успешно.");
	return 0;
}