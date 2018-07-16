#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Visitor.h"
#include "common.h"
#include "SymbolTable.h"
#include "MyPLException.h"

class InterpreterException: public MyPLException {
public:
    InterpreterException(std::string msg=""): MyPLException(msg) {}
};

class Interpreter: public Visitor {
public:
    void visit(ASTSimpleBoolExpression& simpleBoolExpr) override;
    
    void visit(ASTComplexBoolExpression& complexBoolExpr) override;
    
    void visit(ASTStatementList& statementList) override;
    
    void visit(ASTBasicIf& basicIf) override;
    
    void visit(ASTIfStatement& ifStatement) override;
    
    void visit(ASTWhileStatement& whileStatement) override;
    
    void visit(ASTPrintStatement& printStatement) override;
    
    void visit(ASTAssignmentStatement& assignmentStatement) override;
    
    void visit(ASTIdentifier& identifier) override;
    
    void visit(ASTLiteral& literal) override;
    
    void visit(ASTListLiteral& listLiteral) override;
    
    void visit(ASTReadExpression& readExpression) override;
    
    void visit(ASTComplexExpression& complexExpression) override;
    
private:
    // Type of current expression (if any)
    MPLType currentType;
    
    // If int is current type, this contains the current expression value
    int currentInt;
    
    // If string is the current type, this contains the current expression
    // value
    std::string currentString;
    
    // If bool is the current type, this contains the current expression value
    bool currentBool;
    
    // Symbol table used to store and access variable values
    SymbolTable table;
};


#endif /* INTERPRETER_H */
