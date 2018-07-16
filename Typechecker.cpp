/*
 @Author David Schroeder
 @Editors Benjamin Bladow & Cole deSilva
 @Class: CPSC 326-01
 @Assignment: Programming Assignment #6 (TypeChecker)
 @Due Date: 3/13/2018
 */

#include "Typechecker.h"
#include "ASTNodes.h"

using namespace std;

void Typechecker::visit(ASTSimpleBoolExpression& simpleBoolExpr)
{
    //runs correct version of visit...not able to return anything though...use currentType field.
    simpleBoolExpr.expression->accept(*this);
    if (currentType != MPLType::BOOL)
    {
        throw TypecheckerException("Expected boolean expression");
    }
    currentType = MPLType::BOOL;
}

void Typechecker::visit(ASTComplexBoolExpression& complexBoolExpr)
{
    complexBoolExpr.first -> accept(*this);
    //checks what currentType is in the first expression and what operators it can be used with
    if(currentType == MPLType::BOOL)
    {
        if(complexBoolExpr.relation != Token::EQUAL && complexBoolExpr.relation != Token::NOT_EQUAL)
        {
            throw TypecheckerException("Expected equality or inequality check");
        }
    }
    else if(currentType == MPLType::INT || currentType == MPLType::STRING)
    {
        if(complexBoolExpr.relation != Token::EQUAL && complexBoolExpr.relation != Token::NOT_EQUAL && complexBoolExpr.relation != Token::LESS_THAN && complexBoolExpr.relation != Token::GREATER_THAN && complexBoolExpr.relation != Token::LESS_THAN_EQUAL && complexBoolExpr.relation != Token::GREATER_THAN_EQUAL)
        {
            throw TypecheckerException("Expected type of equality check");
        }
    }
    
    //checking if type of first expression is equal to type of second expression
    MPLType temp1 = currentType;

    complexBoolExpr.second -> accept(*this);
    MPLType temp2 = currentType;
    
    if(temp1 != temp2)
    {
        throw TypecheckerException("Expected expressions to be of equal type");
    }
    
    //if there is a conjunction then run accept for the next boolean expression
    if(complexBoolExpr.hasConjunction)
    {
        complexBoolExpr.remainder -> accept(*this);
    }
    
    currentType = MPLType::BOOL;
}

void Typechecker::visit(ASTStatementList& statementList)
{
    table.pushTable();
    for (auto &statement  : statementList.statements)
    {
        statement->accept(*this);
    }
    table.popTable();
}

void Typechecker::visit(ASTBasicIf& basicIf)
{
    basicIf.expression -> accept(*this);
    //makes sure expression in if-then is boolean
    if(currentType != MPLType::BOOL)
    {
        throw TypecheckerException("Expected boolean expression");
    }
    
    //run visit for what's inside the if
    basicIf.statementList -> accept(*this);
}

void Typechecker::visit(ASTIfStatement& ifStatement)
{
    ifStatement.baseIf.accept(*this);
    
    //if there are else-ifs then run visits for each of those
    for(int i = 0; i < ifStatement.elseifs.size(); i++)
    {
        ifStatement.elseifs[i].accept(*this);
    }
    
    //run visit for else (if there is one)
    if(ifStatement.elseList)
    {
        ifStatement.elseList -> accept(*this);
    }
}

void Typechecker::visit(ASTWhileStatement& whileStatement)
{
    whileStatement.condition -> accept(*this);
    //makes sure expression in while is boolean
    if(currentType != MPLType::BOOL)
    {
        throw TypecheckerException("Expected boolean expression");
    }

    //runs visit for what's inside the while
    whileStatement.statements -> accept(*this);
}

void Typechecker::visit(ASTPrintStatement& printStatement)
{
    printStatement.expression->accept(*this);
}

void Typechecker::visit(ASTAssignmentStatement& assignmentStatement)
{
    //only care about right hand side of assignment because thats where type is
    assignmentStatement.rhs -> accept(*this);
    
    //assigning a type to the associated variable in the table
    if(currentType == MPLType::INT)
    {
        table.storeIntVal(assignmentStatement.identifier -> name, 0); //sets to 0
    }
    else if(currentType == MPLType::STRING)
    {
        table.storeStringVal(assignmentStatement.identifier -> name, ""); //sets to empty string
    }
    else if(currentType == MPLType::BOOL)
    {
        table.storeBoolVal(assignmentStatement.identifier -> name, false); // sets to false
    }
    else //not sure if this is needed but just in case
    {
        throw TypecheckerException("Expected int, string, or bool");
    }
}

void Typechecker::visit(ASTIdentifier& identifier)
{
    if (table.doesSymbolExist(identifier.name))
    {
        currentType = table.getSymbolType(identifier.name);
    }
    else
    {
        throw TypecheckerException("Identifier " + identifier.name + " used before defined");
    }
    if (identifier.indexExpression && currentType != MPLType::ARRAY)
    {
        throw TypecheckerException("Identifier " + identifier.name + " given an index when not an array");
    }
}

void Typechecker::visit(ASTLiteral& literal)
{
    currentType = literal.type;
}

void Typechecker::visit(ASTListLiteral& listLiteral)
{
    //if there is a list of literals then run visits for each of those literals
    for(int i = 0; i < listLiteral.expressions.size(); i++)
    {
        listLiteral.expressions[i] -> accept(*this);
    }
}

void Typechecker::visit(ASTReadExpression& readExpression)
{
    if (readExpression.isReadInt)
    {
        currentType = MPLType::INT;
    }
    else
    {
        currentType = MPLType::STRING;
    }
}

void Typechecker::visit(ASTComplexExpression& complexExpression)
{
    complexExpression.firstOperand -> accept(*this);
    //checks first operand and its type to see what operators are compatible with it
    if(currentType == MPLType::STRING)
    {
        if(complexExpression.operation != Token::PLUS)
        {
            throw TypecheckerException("Expected '+' operator for strings");
        }
    }
    else if(currentType == MPLType::INT)
    {
        if(complexExpression.operation != Token::PLUS && complexExpression.operation != Token::MINUS && complexExpression.operation != Token::MULTIPLY && complexExpression.operation != Token::DIVIDE && complexExpression.operation != Token::MODULUS)
        {
            throw TypecheckerException("Expected math operator for integers");
        }
    }
    else if(currentType == MPLType::BOOL)
    {
        throw TypecheckerException("Boolean not allowed to be operand with math operators");
    }
    
    //checking if type of first expression is equal to type of second expression
    MPLType temp1 = currentType;
    
    complexExpression.rest -> accept(*this);
    if(temp1 != currentType)
    {
        throw TypecheckerException("Expected expressions to be of equal type");
    }
}




