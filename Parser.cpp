#include "Parser.h"

#include <iostream>
#include <sstream>

using namespace std;

const bool USE_CONTEXTLOG = false;

class ContextLog
{
public:
    ContextLog(string p, const Lexeme &lex):
    part(p),
    l(lex.line),
    c(lex.col)
    {
        if (USE_CONTEXTLOG)
            cout << "Entering " << part << "(" << l << ":" << c << ")" << endl;
    }
    ~ContextLog()
    {
        if (USE_CONTEXTLOG)
            cout << "Exiting " << part << "(" << l << ":" << c << ")" << endl;
    }
private:
    string part;
    int l;
    int c;
};

Parser::Parser(Lexer &lex): lexer(lex)
{
    advance();
    syntaxTree = stmts();
}

shared_ptr<ASTNode> Parser::getAST()
{
    return syntaxTree;
}

void Parser::advance()
{
    currentLexeme = lexer.next();
}

void Parser::eat(Token token, std::string message)
{
    if (currentLexeme.token == token)
    {
        advance();
    }
    else
    {
        error(message);
    }
}

void Parser::error(std::string message)
{
    cout << "Error found" << endl;
    ostringstream oss;
    oss << message << ", found " << currentLexeme << " instead";
    throw ParserException(oss.str());
}

bool isTokenAValue(Token token)
{
    switch (token)
    {
        case Token::ID:
        case Token::STRING:
        case Token::INT:
        case Token::BOOL:
        case Token::READINT:
        case Token::READSTR:
        case Token::LBRACKET:
            return true;
            break;
        default:
            return false;
    }
}

std::shared_ptr<ASTStatementList> Parser::stmts(std::shared_ptr<ASTStatementList> lst)
{
    ContextLog clog("stmts", currentLexeme);
    if (!lst) lst = make_shared<ASTStatementList>();
    switch (currentLexeme.token)
    {
        case Token::PRINT:
        case Token::PRINTLN:
        case Token::ID:
        case Token::IF:
        case Token::WHILE:
            lst->statements.push_back(stmt());
            stmts(lst);
            break;
        default:
            // May be empty
            break;
    }
    return lst;
}
std::shared_ptr<ASTStatement> Parser::stmt()
{
    ContextLog clog("stmt", currentLexeme);
    switch (currentLexeme.token)
    {
        case Token::PRINT:
        case Token::PRINTLN:
            return output();
            break;
        case Token::ID:
            return assign();
            break;
        case Token::IF:
            return cond();
            break;
        case Token::WHILE:
            return loop();
            break;
        default:
            error("Expected statement");
    }
}

std::shared_ptr<ASTPrintStatement> Parser::output()
{
    ContextLog clog("output", currentLexeme);
    auto ans = std::make_shared<ASTPrintStatement>();
    if (currentLexeme.token == Token::PRINT)
    {
        eat(Token::PRINT, "Expected print");
        ans->isPrintln = false;
    }
    else
    {
        eat(Token::PRINTLN, "Expected print or println");
        ans->isPrintln = true;
    }
    eat(Token::LPAREN, "Expected '('");
    ans->expression = expr();
    eat(Token::RPAREN, "Expected ')'");
    eat(Token::SEMICOLON, "Expected ';'");
    return ans;
}

std::shared_ptr<ASTReadExpression> Parser::input()
{
    ContextLog clog("input", currentLexeme);
    auto ans = std::make_shared<ASTReadExpression>();

     //checks if its a readint or readstr.
    if(currentLexeme.token == Token::READINT)
    {
        eat(Token::READINT, "Expected readint");
        ans -> isReadInt = true;
    }
    else
    {
        eat(Token::READSTR, "Expected readstr");
        ans -> isReadInt = false;
    }

    eat(Token::LPAREN, "Expected '('");
    ans -> message = currentLexeme.text;
    eat(Token::STRING, "Expected string");
    eat(Token::RPAREN, "Expected ')'");

    return ans;
}

std::shared_ptr<ASTAssignmentStatement> Parser::assign() //come back to this
{
    ContextLog clog("assign", currentLexeme);
    auto ans = std::make_shared<ASTAssignmentStatement>();

    ans -> identifier = make_shared<ASTIdentifier>();
    ans -> identifier -> name = currentLexeme.text;
    eat(Token::ID, "Expected ID");
    ans -> identifier -> indexExpression = listindex();
    eat(Token::ASSIGN, "Expected '='");
    ans -> rhs = expr();
    eat(Token::SEMICOLON, "Expected ';'");

    return ans;
}

std::shared_ptr<ASTExpression> Parser::listindex()
{
    ContextLog clog("listindex", currentLexeme);
    if (currentLexeme.token == Token::LBRACKET)
    {
        advance();
        auto ans = expr();
        eat(Token::RBRACKET, "Expected ']'");
        return ans;
    }

    return nullptr;
}

std::shared_ptr<ASTExpression> Parser::expr()
{
    ContextLog clog("expr", currentLexeme);
    if(isTokenAValue(currentLexeme.token))
    {
        auto ans = make_shared<ASTComplexExpression>();
        ans -> firstOperand = value();
        exprt(ans);
        if(ans -> rest)
        {
            return ans;
        }
        else
        {
            return ans -> firstOperand;
        }
    }
    else
    {
        error("Expected a value");
    }
}

void Parser::exprt(std::shared_ptr<ASTComplexExpression> expression)
{
    ContextLog clog("exprt", currentLexeme);
    switch(currentLexeme.token)
    {
        case Token::PLUS:
            expression -> operation = currentLexeme.token;
            eat(Token::PLUS, "Expected '+'");
            expression -> rest = expr();
            break;
        case Token::MINUS:
            expression -> operation = currentLexeme.token;
            eat(Token::MINUS, "Expected '-'");
            expression -> rest = expr();
            break;
        case Token::DIVIDE:
            expression -> operation = currentLexeme.token;
            eat(Token::DIVIDE, "Expected '/'");
            expression -> rest = expr();
            break;
        case Token::MULTIPLY:
            expression -> operation = currentLexeme.token;
            eat(Token::MULTIPLY, "Expected '*'");
            expression -> rest = expr();
            break;
        case Token::MODULUS:
            expression -> operation = currentLexeme.token;
            eat(Token::MODULUS, "Expected '%'");
            expression -> rest = expr();
            break;
        default:
            expression -> rest = nullptr;
            break;
    }

}

std::shared_ptr<ASTExpression> Parser::value()
{
    ContextLog clog("value", currentLexeme);
    switch (currentLexeme.token)
    {
        case Token::ID:
        {
            auto ans = make_shared<ASTIdentifier>();
            ans -> name = currentLexeme.text;
            eat(Token::ID, "Expected ID");
            ans -> indexExpression = listindex();
            return ans;
            break;
        }
        case Token::STRING:
        {
            auto ans = make_shared<ASTLiteral>();
            ans->type = MPLType::STRING;
            ans->value = currentLexeme.text;
            advance();
            return ans;
            break;
        }
        case Token::INT:
        {
            auto ans = make_shared<ASTLiteral>();
            ans -> type = MPLType::INT;
            ans -> value = currentLexeme.text;
            advance();
            return ans;
            break;
        }
        case Token::BOOL:
        {
            auto ans = make_shared<ASTLiteral>();
            ans -> type = MPLType::BOOL;
            ans -> value = currentLexeme.text;
            advance();
            return ans;
            break;
        }
        case Token::READINT:
        case Token::READSTR:
        {
            return input();
            break;
        }
        case Token::LBRACKET:
        {

            eat(Token::LBRACKET, "Expected '['");
            auto ans = exprlist();
            eat(Token::RBRACKET, "Expected ']'");
            return ans;
            break;
        }
        default:
            error("Expected a value");
    }
}

std::shared_ptr<ASTListLiteral> Parser::exprlist()
{
    ContextLog clog("exprlist", currentLexeme);
    auto ans = std::make_shared<ASTListLiteral>();

    if(isTokenAValue(currentLexeme.token))
    {
        ans -> expressions.push_back(expr());
        exprtail(ans);
        return ans;
    }
    else
    {
        return nullptr;
    }
}
void Parser::exprtail(std::shared_ptr<ASTListLiteral> list)
{
    ContextLog clog("exprtail", currentLexeme);
    switch(currentLexeme.token)
    {
        case Token::COMMA:
            eat(Token::COMMA, "Expected ','");
            list -> expressions.push_back(expr());// come here
            exprtail(list);
            break;
        default:
            break;
    }

}

std::shared_ptr<ASTIfStatement> Parser::cond()
{
    ContextLog clog("cond", currentLexeme);
    auto ans = std::make_shared<ASTIfStatement>();

    eat(Token::IF, "Expected if");
    ans -> baseIf.expression = bexpr();

    eat(Token::THEN, "Expected then");

    ans -> baseIf.statementList = stmts();

    condt(ans);

    eat(Token::END, "Expected end");

    return ans;
}

void Parser::condt(std::shared_ptr<ASTIfStatement> statement)
{
    ContextLog clog("condt", currentLexeme);
    switch(currentLexeme.token)
    {
        case Token::ELSEIF:
            eat(Token::ELSEIF, "Expected elseif");
            statement -> elseifs.push_back(ASTBasicIf());
            statement -> elseifs.back().expression = bexpr();

            eat(Token::THEN, "Expected then");
            statement -> elseifs.back().statementList = stmts();
            condt(statement);
            break;
        case Token::ELSE:
            eat(Token::ELSE, "Expected else");
            statement -> elseList = stmts();
            break;
        default:
            break;
    }
}

std::shared_ptr<ASTBoolExpression> Parser::bexpr() {
    ContextLog clog("bexpr", currentLexeme);
    bool negated = false;
    if (currentLexeme.token == Token::NOT) {
        advance();
        negated = true;
    }
    auto lhs = expr();
    switch (currentLexeme.token) {
        case Token::EQUAL:
        case Token::LESS_THAN:
        case Token::GREATER_THAN:
        case Token::LESS_THAN_EQUAL:
        case Token::GREATER_THAN_EQUAL:
        case Token::NOT_EQUAL: {
            auto ans = make_shared<ASTComplexBoolExpression>();
            ans->negated = negated;
            ans->first = lhs;
            bexprt(ans);
            return ans;
            break;
        }
        default: {
            auto ans = make_shared<ASTSimpleBoolExpression>();
            ans->negated = negated;
            ans->expression = lhs;
            return ans;
            break;
        }
    }
}

void Parser::bexprt(std::shared_ptr<ASTComplexBoolExpression> expression)
{
    ContextLog clog("bexprt", currentLexeme);
    switch (currentLexeme.token)
    {
        case Token::EQUAL:
        case Token::LESS_THAN:
        case Token::GREATER_THAN:
        case Token::LESS_THAN_EQUAL:
        case Token::GREATER_THAN_EQUAL:
        case Token::NOT_EQUAL:
            expression->relation = currentLexeme.token;
            break;
        default:
            error("Internal error: expected relational operator");
    }
    advance();
    expression->second = expr();
    bconnect(expression);
}

void Parser::bconnect(std::shared_ptr<ASTComplexBoolExpression> expression)
{
    ContextLog clog("bconnect", currentLexeme);
    switch(currentLexeme.token)
    {
        case Token::AND:
        case Token::OR:
            expression -> hasConjunction = true;
            expression -> conjunction = currentLexeme.token;
            advance();
            expression -> remainder = bexpr();
            break;
        default:
            break;
    }
}

std::shared_ptr<ASTWhileStatement> Parser::loop()
{
    ContextLog clog("loop", currentLexeme);
    auto ans = std::make_shared<ASTWhileStatement>();

    eat(Token::WHILE, "Expected while");
    ans -> condition = bexpr();

    eat(Token::DO, "Expected do");
    ans -> statements = stmts();

    eat(Token::END, "Expected end");
    return ans;
}
