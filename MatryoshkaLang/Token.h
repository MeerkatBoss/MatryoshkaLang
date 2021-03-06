#pragma once
#include "TokenType.h"
#include <string>
#include <variant>
class Token
{
public:
	TokenType type;
	std::string lexeme;
	std::variant<int, double, std::string> stored;
	int line;
	Token(TokenType type, std::string lexeme, std::variant<int, double, std::string> stored, int line)
	{
		this->type = type;
		this->lexeme = lexeme;
		this->stored = stored;
		this->line = line;
	}
};