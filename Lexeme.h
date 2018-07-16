#ifndef LEXEME_H
#define LEXEME_H

// You shouldn't need to modify this file for HW2

#include <string>
#include <ostream>
#include <sstream>

#include "Token.h"

/* Represents a lexeme in the code */
struct Lexeme {
    /* Explicitly allow default constructor (creating a Lexeme without providing
     any information */
    Lexeme() = default;
    
    /* Allow constructing a Lexeme by passing in a token, text, line, and
     column.  Initializes the member variables appropriately */
    Lexeme(Token initToken, std::string initText, int initLine, int initCol):
    token(initToken), text(initText), line(initLine), col(initCol) {}
    
    // What token?
    Token token;
    
    // What text corresponds in the source code?
    std::string text;
    
    // What line did the token start on?
    int line = -1;
    
    // What character did the token start on?
    int col = -1;
};

/* Allows Lexemes to be output to cout or other output streams using the
 << syntax */
std::ostream& operator<<(std::ostream& out, const Lexeme& lex);

/* Given a lexeme, return a string representation of its contents */
std::string toString(const Lexeme &lex);


#endif /* LEXEME_H */
