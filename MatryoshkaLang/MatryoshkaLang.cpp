#include <iostream>
#include <fstream>
#include <string>
#include <clocale>
#include <vector>
#include "Lexer.h"
#include "ErrorHandlers.h"
#include "Token.h"
#include "TokenType.h"

int main()
{
	setlocale(LC_ALL, "rus");
	std::ifstream fin("input.txt");
	std::string str;
	std::string text = "";
	while (getline(fin, str))
		text += str + '\n';
	ErrorHandler* errorHandler = new ConsoleErrorHandler();
	Lexer lexer(text, errorHandler);
	std::vector<Token> tokens = lexer.getTokens();	//TODO: make a function to print tokens
	if (errorHandler->hadError())
		exit(65);
	return 0;
}