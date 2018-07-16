#include "ASTPrinter.h"
#include "ASTNodes.h"

#include <iostream>

using namespace std;

const int INDENTATION_STEP = 4;

void ASTPrinter::visit(ASTSimpleBoolExpression& simpleBoolExpr) {
    println("SimpleBoolExpression:");
    indent();
    if (simpleBoolExpr.negated) {
        println("not");
        simpleBoolExpr.expression->accept(*this);
    }
    dedent();
}

void ASTPrinter::visit(ASTComplexBoolExpression& complexBoolExpr) {
    println("ComplexBoolExpression:");
    indent();
    if (complexBoolExpr.negated) {
        println("not");
    }
    complexBoolExpr.first->accept(*this);
    println(complexBoolExpr.relation);
    complexBoolExpr.second->accept(*this);
    if (complexBoolExpr.hasConjunction) {
        println(complexBoolExpr.conjunction);
        complexBoolExpr.remainder->accept(*this);
    }
    dedent();
}

void ASTPrinter::visit(ASTStatementList& statementList) {
    println("StatementList");
    indent();
    for (auto& statement: statementList.statements) {
        statement->accept(*this);
    }
    dedent();
}

void ASTPrinter::visit(ASTBasicIf &basicIf) {
    println("BasicIf");
    indent();
    basicIf.expression->accept(*this);
    basicIf.statementList->accept(*this);
    dedent();
}

void ASTPrinter::visit(ASTIfStatement& ifStatement) {
    println("IfStatement");
    indent();
    println("If:");
    indent();
    ifStatement.baseIf.accept(*this);
    dedent();
    for (auto &elseif: ifStatement.elseifs) {
        println("Elseif:");
        indent();
        elseif.accept(*this);
        dedent();
    }
    if (ifStatement.elseList) {
        println("Else:");
        ifStatement.elseList->accept(*this);
    }
    dedent();
}

void ASTPrinter::visit(ASTWhileStatement& whileStatement) {
    println("While:");
    indent();
    whileStatement.condition->accept(*this);
    whileStatement.statements->accept(*this);
    dedent();
}

void ASTPrinter::visit(ASTPrintStatement& printStatement) {
    if (printStatement.isPrintln) {
        println("Println");
    } else {
        println("Print");
    }
    indent();
    printStatement.expression->accept(*this);
    dedent();
}

void ASTPrinter::visit(ASTAssignmentStatement& assignmentStatement) {
    println("Assignment:");
    indent();
    println("Set:");
    assignmentStatement.identifier->accept(*this);
    println("Equal to:");
    assignmentStatement.rhs->accept(*this);
    dedent();
}

void ASTPrinter::visit(ASTIdentifier& identifier) {
    println("Identifier: ", identifier.name);
    if (identifier.indexExpression) {
        println("Index:");
        indent();
        identifier.indexExpression->accept(*this);
        dedent();
    }
}

void ASTPrinter::visit(ASTLiteral& literal) {
    switch(literal.type) {
        case MPLType::BOOL:
            println("Boolean literal: ", literal.value);
            break;
        case MPLType::INT:
            println("Integer literal: ", literal.value);
            break;
        case MPLType::STRING:
            println("String literal: \"", literal.value, "\"");
            break;
    }
}

void ASTPrinter::visit(ASTListLiteral& listLiteral) {
    println("List literal:");
    indent();
    for (int i=0; i<listLiteral.expressions.size(); i++) {
        println("Element ", i, ":");
        indent();
        listLiteral.expressions[i]->accept(*this);
        dedent();
    }
    dedent();
}

void ASTPrinter::visit(ASTReadExpression& readExpression) {
    if (readExpression.isReadInt) {
        println("Read integer with message: ", readExpression.message);
    } else {
        println("Read string with message: ", readExpression.message);
    }
}

void ASTPrinter::visit(ASTComplexExpression& complexExpression) {
    println("Complex expression:");
    indent();
    complexExpression.firstOperand->accept(*this);
    println("Operation: ", complexExpression.operation);
    complexExpression.rest->accept(*this);
    dedent();
}



void ASTPrinter::indent() {
    spaceCount += INDENTATION_STEP;
}

void ASTPrinter::dedent() {
    spaceCount -= INDENTATION_STEP;
}

std::string ASTPrinter::spacing() {
    return string(spaceCount, ' ');
}





