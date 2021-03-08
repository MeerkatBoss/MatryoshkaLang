#pragma once
#include "Token.h"
#include "TokenType.h"
#include "ErrorHandlers.h"
#include <vector>
#include <string>
#include <map>
#include <any>
class Lexer
{
public:
	Lexer(std::string source, ErrorHandler* errorHandler)
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
	std::map<std::string, TokenType> keywords =	//list of all keywords
	{
		{"нач", TokenType::BEGIN},
		{"кон", TokenType::END}
	};	//TODO: add all existing keywords to map
	ErrorHandler* errorHandler;
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
			//insignificant symbols
		case ' ':
		case '\r':
		case '\t':
			break;
			//new line
		case '\n':
			line++;
			break;
			//single symbol tokens
		case '!': addToken(TokenType::EXLAMATION); break;
		case '№': addToken(TokenType::NUM_OF); break;
		case ';': addToken(TokenType::SEMICOLON); break;
		case '%': addToken(TokenType::MODULO); break;
		case ':': addToken(TokenType::COLON); break;
		case '?': addToken(TokenType::QUESTION); break;
		case '*': addToken(TokenType::STAR); break;
		case '-': addToken(TokenType::MINUS); break;
		case '+': addToken(TokenType::PLUS); break;
		case '=': addToken(TokenType::ASSIGN); break;
		case '(': addToken(TokenType::LEFT_PAREN); break;
		case ')': addToken(TokenType::RIGHT_PAREN); break;
		case '.': addToken(TokenType::POINT); break;
		case ',': addToken(TokenType::COMMA); break;
			//SLASH and commentary have same beginning
		case '/':
			if (advanceIf('/'))	//commentaries last until end of line
			{
				while (checkNext() != '\n' && !isAtEnd()) advance();	//new line not consumed, it's needed to update line counter
			}
			else
			{
				addToken(TokenType::SLASH);
			}
			break;
		case '\"':
			addStringValue();
			break;
		default:
			if (isNumeric(c))	//number
			{
				addNumericValue();
			}
			else if (isAlphabetic(c))	//identifier or keyword
			{
				addIdentifier();
			}
			else
			{
				errorHandler->reportError(line, "Неизвестный символ.");
			}
			break;
		}
	}
	char advance()	//Returns next character and moves pointer forward
	{
		return source[current++];
	}
	bool advanceIf(char cmp)	//Calls advance() if next character matches argument. If it does returns true, otherwise false
	{
		if (checkNext() != cmp) return false;
		advance();
		return true;
	}
	char checkNext()	//Returns next character without moving pointer forward
	{
		if (isAtEnd()) return '\0';
		return source[current];
	}
	char checkAfterNext()	//Returns character after next without moving pointer forward
	{
		if (current + 1 >= source.length()) return '\0';
		return source[current + 1];
	}
	void addToken(TokenType type)	//Adds token without value
	{
		addToken(type, NULL);
	}
	void addToken(TokenType type, std::any stored)	//Adds token with value
	{
		std::string text = source.substr(start, current - start);
		tokens.push_back(Token(type, text, stored, line));
	}
	bool isNumeric(char c)
	{
		return '0' <= c && c <= '9';
	}
	bool isAlphabetic(char c)
	{
		return 'А' <= c && c <= 'Я'
			|| 'а' <= c && c <= 'я'
			|| c == '_';
	}
	void addStringValue()
	{
		while (checkNext() != '\"' && !isAtEnd() && checkNext() != '\n') advance();
		if (isAtEnd())
		{
			errorHandler->reportError(line, "Незакрытая '\"' в конце файла.");
			return;
		}

		if (checkNext() == '\n')	//not single-line strings aren't allowed
		{
			errorHandler->reportError(line, "Незакрытая '\"' в конце строки.");
			return;
		}

		advance();
		std::string txt = source.substr(start + 1, current - start - 2);
		addToken(TokenType::STRING, txt);
	}
	void addNumericValue()
	{
		bool hadDecimalPoint = false;
		while (isNumeric(checkNext())) advance();
		if (checkNext() == '.' && isNumeric(checkAfterNext()))
		{
			hadDecimalPoint = true;
			advance();
			while (isNumeric(checkNext())) advance();
		}
		else if (checkNext() == '.')
		{
			advance();
			errorHandler->reportError(line, "Непредвиденная '.' в числе без дробной части.");
			return;
		}
		std::string num = source.substr(start, current - start);

		if (hadDecimalPoint)
		{
			auto ptr = num.find_first_of('.');	//because of Russian locale '.' needs to be replaced with ','
			num.replace(ptr, ptr, ",");
			double d = std::stod(num);
			addToken(TokenType::DOUBLE, d);
		}
		else
		{
			int i = std::stoi(num);
			addToken(TokenType::INT, i);
		}
	}
	void addIdentifier()
	{
		while (isAlphabetic(checkNext()) || isNumeric(checkNext())) advance();
		std::string word = source.substr(start, current - start);
		auto typePtr = keywords.find(word);
		TokenType type;
		if (typePtr == keywords.end())	//check on being keyword
			type = TokenType::IDENTIFIER;
		else
			type = typePtr->second;
		addToken(type);
	}
};