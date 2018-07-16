#ifndef VISITOR_H
#define VISITOR_H

// Forward class declarations to avoid circular includes

class ASTSimpleBoolExpression;
class ASTComplexBoolExpression;

class ASTStatementList;
class ASTBasicIf;
class ASTIfStatement;
class ASTWhileStatement;
class ASTPrintStatement;
class ASTAssignmentStatement;

class ASTIdentifier;
class ASTLiteral;
class ASTListLiteral;
class ASTReadExpression;
class ASTComplexExpression;

// The visitor class implements the visitor pattern for the ASTNodes.
class Visitor {
public:
    // Virtual destructor because abstract class
    virtual ~Visitor() {}
    
    virtual void visit(ASTSimpleBoolExpression& simpleBoolExpr) = 0;
    
    virtual void visit(ASTComplexBoolExpression& complexBoolExpr) = 0;
    
    virtual void visit(ASTStatementList& statementList) = 0;
    
    virtual void visit(ASTBasicIf& basicIf) = 0;
    
    virtual void visit(ASTIfStatement& ifStatement) = 0;
    
    virtual void visit(ASTWhileStatement& whileStatement) = 0;
    
    virtual void visit(ASTPrintStatement& printStatement) = 0;
    
    virtual void visit(ASTAssignmentStatement& assignmentStatement) = 0;
    
    virtual void visit(ASTIdentifier& identifier) = 0;
    
    virtual void visit(ASTLiteral& literal) = 0;
    
    virtual void visit(ASTListLiteral& listLiteral) = 0;
    
    virtual void visit(ASTReadExpression& readExpression) = 0;
    
    virtual void visit(ASTComplexExpression& complexExpression) = 0;
};

#endif /* VISITOR_H */
