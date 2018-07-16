/*
    @Authors Benjamin Bladow & Cole deSilva
    @Class: CPSC 326-01
    @Assignment: Programming Assignment #2 (Lexer)
    @Due Date: 2/1/2018
*/

#include <sstream>
#include <string>

#include "Lexer.h"

using namespace std;

Lexer::Lexer(std::istream &initIn): in(initIn)
{
    // You shouldn't need to modify this function

    // If we are given an invalid input stream, throw an exception
    if (!in)
    {
        throw LexerException("Invalid input stream passed to Lexer");
    }
}

Lexeme Lexer::next()
{
    eatWhitespace();
    char tmp = peek();
    int startLine = lineNum;
    int startCol = colNum;
    if (tmp == eof())
    {
        return Lexeme(Token::EOS, "", lineNum, colNum);
    }
    else if ((tmp >= 'a' && tmp <= 'z') || (tmp >= 'A' && tmp <= 'Z'))
    {
        // TODO: This will be the beginning of an identifier or of some Token
        // that is represented by a series of letters (such as "while")
        string result;
        result.push_back(tmp);
        read();

        while((peek() >= 'a' && peek() <= 'z') || (peek() >= 'A' && peek() <= 'Z') || (peek() >= '0' && peek() <= '9'))
        {
            tmp = peek();
            read();
            result.push_back(tmp); //come back if have issues
        }

        if(result == "not")
        {
            return Lexeme(Token::NOT, "not", startLine, startCol);
        }
        else if(result == "or")
        {
            return Lexeme(Token::OR, "or", startLine, startCol);
        }
        else if(result == "and")
        {
            return Lexeme(Token::AND, "and", startLine, startCol);
        }
        else if(result == "if")
        {
            return Lexeme(Token::IF, "if", startLine, startCol);
        }
        else if(result == "then")
        {
            return Lexeme(Token::THEN, "then", startLine, startCol);
        }
        else if(result == "elseif")
        {
            return Lexeme(Token::ELSEIF, "elseif", startLine, startCol);
        }
        else if(result == "else")
        {
            return Lexeme(Token::ELSE, "else", startLine, startCol);
        }
        else if(result == "while")
        {
            return Lexeme(Token::WHILE, "while", startLine, startCol);
        }
        else if(result == "do")
        {
            return Lexeme(Token::DO, "do", startLine, startCol);
        }
        else if(result == "end")
        {
            return Lexeme(Token::END, "end", startLine, startCol);
        }
        else if(result == "print")
        {
            return Lexeme(Token::PRINT, "print", startLine, startCol);
        }
        else if(result == "println")
        {
            return Lexeme(Token::PRINTLN, "println", startLine, startCol);
        }
        else if(result == "or")
        {
            return Lexeme(Token::OR, "or", startLine, startCol);
        }
        else if(result == "readint")
        {
            return Lexeme(Token::READINT, "readint", startLine, startCol);
        }
        else if(result == "readstr")
        {
            return Lexeme(Token::READSTR, "readstr", startLine, startCol);
        }
        else if(result == "true")
        {
            return Lexeme(Token::BOOL, "true", startLine, startCol);
        }
        else if(result == "false")
        {
            return Lexeme(Token::BOOL, "false", startLine, startCol);
        }
        else
        {
            return Lexeme(Token::ID, result, startLine, startCol);
        }
    }
    else if(tmp >= '0' && tmp <= '9')
    {
        string result;
        result.push_back(tmp);
        read();

        while((peek() >= '0' && peek() <= '9'))
        {
            tmp = peek();
            read();
            result.push_back(tmp);
        }

        return Lexeme(Token::INT, result, startLine, startCol);
    }
    else if(tmp == '\"')
    {
        string result;
        read();

        while(peek() != '\"')
        {
            if(peek() == '\n')
            {
                ostringstream oss;
                oss << "Expected '\"' Symbol";
                throw LexerException(debugMessage(oss.str()));
            }
            else
            {
                tmp = peek();
                read();
                result.push_back(tmp);
            }
        }

        read();
        return Lexeme(Token::STRING, result, startLine, startCol);
    }
    else if (tmp == '+')
    {
        return makeSingleCharLexeme(Token::PLUS);
    }
    else if (tmp == '-')
    {
        return makeSingleCharLexeme(Token::MINUS);
    }
    else if (tmp == '*')
    {
        return makeSingleCharLexeme(Token::MULTIPLY);
    }
    else if (tmp == '/')
    {
        return makeSingleCharLexeme(Token::DIVIDE);
    }
    else if (tmp == '%')
    {
        return makeSingleCharLexeme(Token::MODULUS);
    }
    else if (tmp == '(')
    {
        return makeSingleCharLexeme(Token::LPAREN);
    }
    else if (tmp == ')')
    {
        return makeSingleCharLexeme(Token::RPAREN);
    }
    else if (tmp == '[')
    {
        return makeSingleCharLexeme(Token::LBRACKET);
    }
    else if (tmp == ']')
    {
        return makeSingleCharLexeme(Token::RBRACKET);
    }
    else if (tmp == ',')
    {
        return makeSingleCharLexeme(Token::COMMA);
    }
    else if (tmp == ';')
    {
        return makeSingleCharLexeme(Token::SEMICOLON);
    }
    else if (tmp == '>')
    {
        read();
        if (peek() == '=')
        {
            read();
            return Lexeme(Token::GREATER_THAN_EQUAL, ">=", startLine, startCol);
        }
        else
        {
            return Lexeme(Token::GREATER_THAN, ">", startLine, startCol);
        }
    }
    else if (tmp == '<')
    {
        read();
        if (peek() == '=')
        {
            read();
            return Lexeme(Token::LESS_THAN_EQUAL, "<=", startLine, startCol);
        }
        else
        {
            return Lexeme(Token::LESS_THAN, "<", startLine, startCol);
        }
    }
    else if (tmp == '=')
    {
        read();
        if (peek() == '=')
        {
            read();
            return Lexeme(Token::EQUAL, "==", startLine, startCol);
        }
        else
        {
            return Lexeme(Token::ASSIGN, "=", startLine, startCol);
        }
    }
    else if (tmp == '!')
    {
        read();
        if (peek() == '=')
        {
            read();
            return Lexeme(Token::NOT_EQUAL, "!=", startLine, startCol);
        }
        else
        {
            ostringstream oss;
            oss << "Unexpected symbol after '!': '" << peek() << "'";
            throw LexerException(debugMessage(oss.str()));
        }
    }
    else
    {
        // You will not need to change the contents of this "else"
        ostringstream oss;
        oss << "Unexpected symbol: '" << peek() << "'";
        throw LexerException(debugMessage(oss.str()));
    }
    // Yes, you might get a warning about control reaching the end of a
    // non-void function.  Make sure you are returning a Lexeme from each
    // block (the code as distributed has one case with just a TODO).
    //
    // If this is an error in your compiler, you can put in a call to
    // abort() which will stop the program running if control reaches here.
}

bool Lexer::atEOF()
{
    // You shouldn't need to modify this function
    return peek() == eof();
}

char Lexer::peek()
{
    // You shouldn't need to modify this function
    return in.peek();
}

char Lexer::read()
{
    // You shouldn't need to modify this function
    if (in.peek() == '\n')
    {
        colNum = 1;
        lineNum += 1;
    } else
    {
        colNum += 1;
    }
    return in.get();
}
void Lexer::eatWhitespace()
{

    while(peek() == ' ' || peek() == '\n' || peek() == '\t' || peek() == '#')
    {
        if(peek() == '#')
        {
            read();
            while(peek() != '\n')
            {
                read();
            }
        }
        else
        {
            read();
        }
    }
}

char Lexer::eof()
{
    // You shouldn't need to modify this function
    return std::istream::traits_type::eof();
}

Lexeme Lexer::makeSingleCharLexeme(Token token) {
    // You shouldn't need to modify this function
    int outLine = lineNum;
    int outCol = colNum;
    string str(1, read());
    return Lexeme(token, str, outLine, outCol);
}

std::string Lexer::debugMessage(std::string msg)
{
    // You shouldn't need to modify this function
    ostringstream oss;
    oss << msg << " " << lineNum << ":" << colNum;
    return oss.str();
}
