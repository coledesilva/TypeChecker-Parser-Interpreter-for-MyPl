#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "MyPLException.h"
#include "Lexer.h"
#include "ASTNodes.h"

class ParserException: public MyPLException {
public:
    ParserException(std::string msg = ""): MyPLException(msg) {}
};

class Parser {
public:
    Parser(Lexer &lex);
    
    std::shared_ptr<ASTNode> getAST();
private:
    void advance();
    
    void eat(Token token, std::string message);
    
    [[noreturn]] void error(std::string message);
    
    std::shared_ptr<ASTStatementList> stmts(std::shared_ptr<ASTStatementList> lst = nullptr);
    std::shared_ptr<ASTStatement> stmt();
    
    std::shared_ptr<ASTPrintStatement> output();
    std::shared_ptr<ASTReadExpression> input();
    std::shared_ptr<ASTAssignmentStatement> assign();
    std::shared_ptr<ASTExpression> listindex();
    
    std::shared_ptr<ASTExpression> expr();
    void exprt(std::shared_ptr<ASTComplexExpression> expression);
    std::shared_ptr<ASTExpression> value();
    std::shared_ptr<ASTListLiteral> exprlist();
    void exprtail(std::shared_ptr<ASTListLiteral> list);
    
    std::shared_ptr<ASTIfStatement> cond();
    void condt(std::shared_ptr<ASTIfStatement> statement);
    std::shared_ptr<ASTBoolExpression> bexpr();
    void bexprt(std::shared_ptr<ASTComplexBoolExpression> expression);
    void bconnect(std::shared_ptr<ASTComplexBoolExpression> expression);
    
    std::shared_ptr<ASTWhileStatement> loop();
    
    Lexer &lexer;
    
    Lexeme currentLexeme;
    
    std::shared_ptr<ASTNode> syntaxTree;
};

#endif /* PARSER_H */
