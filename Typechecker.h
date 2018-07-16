#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "Visitor.h"
#include "SymbolTable.h"
#include "MyPLException.h"

class TypecheckerException: public MyPLException {
public:
    TypecheckerException(std::string msg=""): MyPLException(msg) {}
};

class Typechecker: public Visitor {
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
    // Table to store current scopes
    SymbolTable table;
    
    // Type of the current expression
    MPLType currentType;
};

#endif /* TYPECHECKER_H */
