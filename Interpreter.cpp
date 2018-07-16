/*
 @Author David Schroeder
 @Editors Benjamin Bladow & Cole deSilva
 @Class: CPSC 326-01
 @Assignment: Programming Assignment #7 (Interpreter)
 @Due Date: 3/29/2018
 */
#include <iostream>
#include <sstream>
#include <limits>

#include "Interpreter.h"
#include "ASTNodes.h"

using namespace std;

void Interpreter::visit(ASTSimpleBoolExpression& simpleBoolExpr)
{
    simpleBoolExpr.expression->accept(*this);
    // Note: after visiting the above expression, (assuming it's boolean)
    // it will set currentBool to be whatever boolean value it represents
    if (currentType != MPLType::BOOL)
    {
        throw InterpreterException("Expected boolean expression");
    }
    if (simpleBoolExpr.negated)
    {
        currentBool = !currentBool;
    }
    currentType = MPLType::BOOL;
}

void Interpreter::visit(ASTComplexBoolExpression& complexBoolExpr) {
    complexBoolExpr.first->accept(*this);
    auto lhsType = currentType;
    auto lhsInt = currentInt;
    auto lhsString = currentString;
    auto lhsBool = currentBool;
    
    complexBoolExpr.second->accept(*this);
    bool myResult;
    
    //checking to make sure types are equivalent
    if(lhsType != currentType)
    {
        throw InterpreterException("Expected types to be equivalent");
    }
    else
    {
        if(lhsType == MPLType::BOOL)
        {
            //checking to make sure the correct test is being performed for the type bool
            if(complexBoolExpr.relation != Token::EQUAL && complexBoolExpr.relation != Token::NOT_EQUAL)
            {
                throw InterpreterException("Expected equality or inequality check");
            }
            else
            {
                //performing the specified test based on the operator
                if(complexBoolExpr.relation != Token::EQUAL)
                {
                    myResult = (lhsBool == currentBool);
                }
                else if(complexBoolExpr.relation != Token::NOT_EQUAL)
                {
                    myResult = (lhsBool != currentBool);
                }
            }
        }
        else if(lhsType == MPLType::INT)
        {
            //checking to make sure the correct test is being performed for the type int
            if(complexBoolExpr.relation != Token::EQUAL && complexBoolExpr.relation != Token::NOT_EQUAL && complexBoolExpr.relation != Token::LESS_THAN && complexBoolExpr.relation != Token::GREATER_THAN && complexBoolExpr.relation != Token::LESS_THAN_EQUAL && complexBoolExpr.relation != Token::GREATER_THAN_EQUAL)
            {
                throw InterpreterException("Expected type of equality check");
            }
            else
            {
                //performing the specified test based on the operator
                if(complexBoolExpr.relation == Token::EQUAL)
                {
                    myResult = (lhsInt == currentInt);
                }
                else if(complexBoolExpr.relation == Token::NOT_EQUAL)
                {
                    myResult = (lhsInt != currentInt);
                }
                else if(complexBoolExpr.relation == Token::LESS_THAN)
                {
                    myResult = (lhsInt < currentInt);
                }
                else if(complexBoolExpr.relation == Token::GREATER_THAN)
                {
                    myResult = (lhsInt > currentInt);
                }
                else if(complexBoolExpr.relation == Token::LESS_THAN_EQUAL)
                {
                    myResult = (lhsInt <= currentInt);
                }
                else if(complexBoolExpr.relation == Token::GREATER_THAN_EQUAL)
                {
                    myResult = (lhsInt >= currentInt);
                }
            }
        }
        else if(lhsType == MPLType::STRING)
        {
            //checking to make sure the correct test is being performed for the type string
            if(complexBoolExpr.relation != Token::EQUAL && complexBoolExpr.relation != Token::NOT_EQUAL && complexBoolExpr.relation != Token::LESS_THAN && complexBoolExpr.relation != Token::GREATER_THAN && complexBoolExpr.relation != Token::LESS_THAN_EQUAL && complexBoolExpr.relation != Token::GREATER_THAN_EQUAL)
            {
                throw InterpreterException("Expected type of equality check");
            }
            else
            {
                //performing the specified test based on the operator
                if(complexBoolExpr.relation == Token::EQUAL)
                {
                    myResult = (lhsString == currentString);
                }
                else if(complexBoolExpr.relation == Token::NOT_EQUAL)
                {
                    myResult = (lhsString != currentString);
                }
                else if(complexBoolExpr.relation == Token::LESS_THAN)
                {
                    myResult = (lhsString < currentString);
                }
                else if(complexBoolExpr.relation == Token::GREATER_THAN)
                {
                    myResult = (lhsString > currentString);
                }
                else if(complexBoolExpr.relation == Token::LESS_THAN_EQUAL)
                {
                    myResult = (lhsString <= currentString);
                }
                else if(complexBoolExpr.relation == Token::GREATER_THAN_EQUAL)
                {
                    myResult = (lhsString >= currentString);
                }
            }
        }
    }
    
    //checks to see if the expression has a conjunction and performs necessary work (Done by Schroeder)
    if (complexBoolExpr.hasConjunction)
    {
        complexBoolExpr.remainder->accept(*this);
        if (complexBoolExpr.conjunction == Token::AND)
        {
            currentBool = myResult && currentBool;
        }
        else
        {
            currentBool = myResult || currentBool;
        }
    }
    else
    {
        currentBool = myResult;
    }
    if (complexBoolExpr.negated)
    {
        currentBool = !currentBool;
    }
    currentType = MPLType::BOOL;
}

void Interpreter::visit(ASTStatementList& statementList)
{
    table.pushTable();
    for (auto &statement : statementList.statements)
    {
        statement->accept(*this);
    }
    table.popTable();
}

void Interpreter::visit(ASTBasicIf& basicIf)
{
    // I strongly suggest not using this function and instead having
    // visit(ASTIfStatement) explicitly look inside the various baseIfs.
    // This is because once you enter a certain if statementList, you will
    // not want to enter any other statementLists.
    abort();
}

void Interpreter::visit(ASTIfStatement& ifStatement) //please helppppppppppppppppppppppppppppp
{
    //dealing with base if
    ifStatement.baseIf.expression -> accept(*this);
    
    if(currentType != MPLType::BOOL)
    {
        throw InterpreterException("Expected boolean expression");
    }
    
    //if true then run statement list
    if(currentBool)
    {
        ifStatement.baseIf.statementList -> accept(*this);
        return;
    }
    
    //if there are else-ifs then run visits for each of those and check to see if they are true to run the statment list
    for(int i = 0; i < ifStatement.elseifs.size(); i++)
    {
        ifStatement.elseifs[i].expression -> accept(*this);
        
        if(currentType != MPLType::BOOL)
        {
            throw InterpreterException("Expected boolean expression");
        }
        
        if(currentBool)
        {
            ifStatement.elseifs[i].statementList -> accept(*this);
            return;
        }
    }
    
    //run visit for else (if there is one)
    if(ifStatement.elseList)
    {
        ifStatement.elseList -> accept(*this);
    }
}

void Interpreter::visit(ASTWhileStatement& whileStatement)
{
    whileStatement.condition->accept(*this);
    while (currentBool)
    {
        whileStatement.statements->accept(*this);
        //updates currentBool
        whileStatement.condition->accept(*this);
    }
}

void Interpreter::visit(ASTPrintStatement& printStatement)
{
    printStatement.expression->accept(*this);
    
    //if it's a println
    if(printStatement.isPrintln)
    {
        //checks which type to print out
        if (currentType == MPLType::INT)
        {
            cout << currentInt << endl;
        }
        else if (currentType == MPLType::STRING)
        {
            cout << currentString << endl;
        }
        else if (currentType == MPLType::BOOL)
        {
            cout << currentBool << endl;
        }
    }
    else //if it's just print
    {
        //checks which type to print out
        if (currentType == MPLType::INT)
        {
            cout << currentInt;
        }
        else if (currentType == MPLType::STRING)
        {
            cout << currentString;
        }
        else if (currentType == MPLType::BOOL)
        {
            cout << currentBool;
        }
    }
}

void Interpreter::visit(ASTAssignmentStatement& assignmentStatement)
{
    //only care about right hand side of assignment because thats where type is
    assignmentStatement.rhs -> accept(*this);
    
    //assigning a type to the associated variable in the table
    if(currentType == MPLType::INT)
    {
        table.storeIntVal(assignmentStatement.identifier -> name, currentInt); //sets to currentInt value
    }
    else if(currentType == MPLType::STRING)
    {
        table.storeStringVal(assignmentStatement.identifier -> name, currentString); //sets to current string value
    }
    else if(currentType == MPLType::BOOL)
    {
        table.storeBoolVal(assignmentStatement.identifier -> name, currentBool); // sets to current boolean value
    }
    else //not sure if this is needed but just in case
    {
        throw InterpreterException("Expected int, string, or bool");
    }
}

void Interpreter::visit(ASTIdentifier& identifier)
{
    if (table.doesSymbolExist(identifier.name))
    {
        currentType = table.getSymbolType(identifier.name);
        //sets the current value to the right value associated with the identifier
        if(currentType == MPLType::INT)
        {
            currentInt = table.getIntVal(identifier.name);
        }
        else if(currentType == MPLType::STRING)
        {
            currentString = table.getStringVal(identifier.name);
        }
        else if(currentType == MPLType::BOOL)
        {
            currentBool = table.getBoolVal(identifier.name);
        }
    }
    else
    {
        throw InterpreterException("Identifier " + identifier.name + " used before defined");
    }
    if (identifier.indexExpression && currentType != MPLType::ARRAY)
    {
        throw InterpreterException("Identifier " + identifier.name + " given an index when not an array");
    }
}

void Interpreter::visit(ASTLiteral& literal)
{
    currentType = literal.type;
    if (currentType == MPLType::INT)
    {
        // istringstream allows us to create something like "cin" but instead
        // of reading from the keyboard, it reads from the string that we give
        // it
        istringstream iss(literal.value);
        iss >> currentInt;
        // If there was an error reading the integer out of the istringstream,
        // it will evaluate to false when used in a boolean context.
        if (!iss)
        {
            throw InterpreterException("Bad integer literal: " + literal.value);
        }
    }
    else if (currentType == MPLType::BOOL)
    {
        // also could be written as
        // currentBool = literal.value == "true", but that looks confusing
        if (literal.value == "true")
        {
            currentBool = true;
        }
        else
        {
            currentBool = false;
        }
    }
    else
    {
        currentString = literal.value;
    }
}

void Interpreter::visit(ASTListLiteral& listLiteral)
{
    // TODO? (only needed for extra credit)
}

void Interpreter::visit(ASTReadExpression& readExpression)
{
    cout << readExpression.message;
    if (readExpression.isReadInt)
    {
        cin >> currentInt;
        currentType = MPLType::INT;
    }
    else
    {
        // getline will read everything up until a newline character, allowing
        // us to read in a string containing spaces
        getline(cin, currentString);
        currentType = MPLType::STRING;
    }
    // If there was an input problem (say, a user entering 'test' to readint),
    // cin will be false when used in a boolean context.  This way, if the
    // user enters something silly, we can throw an error.
    if (!cin)
    {
        throw InterpreterException("Invalid user input");
    }
    // If we read in a number, ignore everything else on the current line
    if (readExpression.isReadInt)
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Interpreter::visit(ASTComplexExpression& complexExpression)
{
    complexExpression.firstOperand -> accept(*this);
    //provides temp variables for the firstOperand of the expression
    auto firstType = currentType;
    auto firstInt = currentInt;
    auto firstString = currentString;
    
    complexExpression.rest -> accept(*this);
    //provides temp variables for the secondOperand fo the expression
    auto lastInt = currentInt;
    auto lastString = currentString;
    
    //checks to make sure types are equivalent
    if(firstType != currentType)
    {
        throw InterpreterException("Expected expressions to be of equal type");
    }
    else
    {
        //checks to make sure the specified operation is allowed for strings
        if(firstType == MPLType::STRING)
        {
            if(complexExpression.operation != Token::PLUS)
            {
                throw InterpreterException("Expected '+' operator for strings");
            }
            else //performs operation
            {
                    currentString = firstString + lastString;
            }
        }
        //checks to make sure the specified operation is allowed for ints
        else if(firstType == MPLType::INT)
        {
            if(complexExpression.operation != Token::PLUS && complexExpression.operation != Token::MINUS && complexExpression.operation != Token::MULTIPLY && complexExpression.operation !=Token::DIVIDE && complexExpression.operation != Token::MODULUS)
            {
                throw InterpreterException("Expected math operator for integers");
            }
            else //performs specified operation
            {
                if(complexExpression.operation == Token::PLUS)
                {
                    currentInt = firstInt + lastInt;
                }
                else if(complexExpression.operation == Token::MINUS)
                {
                    currentInt = firstInt - lastInt;
                }
                else if(complexExpression.operation == Token::MULTIPLY)
                {
                    currentInt = firstInt * lastInt;
                }
                else if(complexExpression.operation == Token::DIVIDE)
                {
                    currentInt = firstInt / lastInt;
                }
                else if(complexExpression.operation == Token::MODULUS)
                {
                    currentInt = firstInt % lastInt;
                }
            }
        }
        //throws error if attempt to use a math operator on boolean
        else if(firstType == MPLType::BOOL)
        {
            throw InterpreterException("Boolean not allowed to be operand with math operators");
        }
    }
}
