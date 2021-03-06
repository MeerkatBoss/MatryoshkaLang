#pragma once
#include "Token.h"
#include "TokenType.h"
#include "ErrorHandlers.h"
#include <vector>
#include <string>
#include <map>
#include <variant>
class Lexer
{
public:
	Lexer(std::string source, IErrorHandler errorHandler)
	{
		this->source = source;
		this->errorHandler = errorHandler;
	}
	std::vector<Token> getTokens()
	{
		while (!isAtEnd())
		{
			start = current;
			getToken();
		}
		tokens.push_back(Token(TokenType::FILE_END, "", NULL, line));
		return tokens;
	}
private:
	IErrorHandler errorHandler;
	std::string source;
	std::vector<Token> tokens;
	int start = 0;
	int current = 0;
	int line = 1;
	bool isAtEnd()
	{
		return current >= source.length();
	}
	void getToken()
	{
		char c = advance();
		switch (c)
		{
		case '!': addToken(TokenType::EXLAMATION); break;
		case '№': addToken(TokenType::NUM_OF); break;
		case ';': addToken(TokenType::SEMICOLON);
		case '%': addToken(TokenType::MODULO); break;
		case ':': addToken(TokenType::COLON); break;
		case '?': addToken(TokenType::QUESTION); break;
		case '*': addToken(TokenType::STAR); break;
		case '-': addToken(TokenType::MINUS); break;
		case '+': addToken(TokenType::PLUS); break;
		case '=': addToken(TokenType::ASSIGN); break;
		case '(': addToken(TokenType::LEFT_PAREN); break;
		case ')': addToken(TokenType::RIGHT_PAREN); break;
		default:
			errorHandler.reportError(line, "Неизвестный символ.");
			break;
		}
	}
	char advance()
	{
		return source[current++];
	}
	void addToken(TokenType type)
	{
		addToken(type, NULL);
	}
	void addToken(TokenType type, std::variant<int, double, std::string> stored)
	{
		std::string text = source.substr(start, current);
		tokens.push_back(Token(type, text, stored, line));
	}
};

