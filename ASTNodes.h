#ifndef ASTNODES_H
#define ASTNODES_H

#include <memory>
#include <vector>

#include "Token.h"
#include "Visitor.h"

#include "common.h"


// Base class for all ASTNodes
class ASTNode {
public:
    virtual ~ASTNode(){} /* virtual destructor because abstract class */

    virtual void accept(Visitor& visitor) = 0;
};

// Represents an element of the AST that is a statement
class ASTStatement: public ASTNode {
};

// Represents an element of the AST that is an expression
class ASTExpression: public ASTNode {
};

// Represents an element of the AST that is a boolean expression
class ASTBoolExpression: public ASTNode {
};

// Simple boolean expression.  May or may not contain "not" followed by a
// single expression.  Does not contain a relation operator or a conjunction
// ("and" or "or")
class ASTSimpleBoolExpression: public ASTBoolExpression {
public:
    bool negated = false;
    std::shared_ptr<ASTExpression> expression;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Complex boolean expression.  May or may not contain "not" followed by an
// expression, a relational operator, and may or may not contain "and" or "or
// followed by another boolean expression
class ASTComplexBoolExpression: public ASTBoolExpression {
public:
    bool negated = false;
    std::shared_ptr<ASTExpression> first;
    Token relation;
    std::shared_ptr<ASTExpression> second;
    bool hasConjunction = false;
    Token conjunction;
    std::shared_ptr<ASTBoolExpression> remainder;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Represents an identifier, possibly including an array indexing expression
class ASTIdentifier: public ASTExpression {
public:
    std::string name;
    std::shared_ptr<ASTExpression> indexExpression;
    // ^ nullptr if not an indexed array
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Represents a literal and the type of the literal.
class ASTLiteral: public ASTExpression {
public:
    std::string value;
    MPLType type;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Represents a list literal.
class ASTListLiteral: public ASTExpression {
public:
    std::vector<std::shared_ptr<ASTExpression>> expressions;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Represents a read expression (either readint or readstr)
class ASTReadExpression: public ASTExpression {
public:
    std::string message;
    bool isReadInt = false;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Represents a complex expression, consisting of two expressions joined by
// a math operator (+, -, *, /, %)
class ASTComplexExpression: public ASTExpression {
public:
    std::shared_ptr<ASTExpression> firstOperand;
    Token operation;
    std::shared_ptr<ASTExpression> rest;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// List of statements.  Statements are stored in order in the vector.
class ASTStatementList: public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> statements;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Component to be used in if statements.  This represents a condition along
// with a list of statements to be run if the condition is met.
struct ASTBasicIf {
    std::shared_ptr<ASTBoolExpression> expression;
    std::shared_ptr<ASTStatementList> statementList;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// An arbitrarily complex if statement.  The test in 'baseIf' comes first,
// followed by the contents of the 'elseifs' array in order.  At the end, if
// 'elseList' is not null, it contains the statements present in the "else"
// block.
class ASTIfStatement: public ASTStatement {
public:
    ASTBasicIf baseIf;
    std::vector<ASTBasicIf> elseifs;
    std::shared_ptr<ASTStatementList> elseList;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// A while statement.  The condition for the loop is represented by 'condition'
// and 'statements' contains the statements to be run
class ASTWhileStatement: public ASTStatement {
public:
    std::shared_ptr<ASTBoolExpression> condition;
    std::shared_ptr<ASTStatementList> statements;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Print statement (either print or println).
class ASTPrintStatement: public ASTStatement {
public:
    std::shared_ptr<ASTExpression> expression;
    bool isPrintln = false;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

// Assignment statement.  The identifier is being set equal to what is on
// the right hand side (rhs).
class ASTAssignmentStatement: public ASTStatement {
public:
    std::shared_ptr<ASTIdentifier> identifier;
    std::shared_ptr<ASTExpression> rhs;
    void accept(Visitor& visitor) { visitor.visit(*this); }
};

#endif /* ASTNODES_H */
