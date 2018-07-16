#ifndef TOKENS_H
#define TOKENS_H

// You shouldn't need to modify this file for HW2

#include <ostream>
#include <string>

/* Define a scoped enumeration with all tokens from "tokens-inc.h" as elements
 */
enum class Token {
#define x(ident) ident,
#include "tokens-inc.h"
#undef x
};

/* Provide an output operator allowing Tokens to be outputted to std::cout
 or to any other ostream using << */
std::ostream& operator<<(std::ostream &out, Token token);

std::string toString(Token token);

#endif /* TOKENS_H */
