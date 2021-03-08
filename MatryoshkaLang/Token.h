#pragma once
#include "TokenType.h"
#include <string>
#include <any>
class Token
{
public:
	TokenType type;
	std::string lexeme;
	std::any stored;
	int line;
	Token(TokenType type, std::string lexeme, std::any stored, int line)
	{
		this->type = type;
		this->lexeme = lexeme;
		this->stored = stored;
		this->line = line;
	}
};