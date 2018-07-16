#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <string>
#include <iostream>
#include <utility>
#include "Visitor.h"

// Class to print out an abstract syntax tree.  Implements the Visitor pattern
// to traverse the AST.
class ASTPrinter: public Visitor {
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
    
    // Increase the indentation level
    void indent();
    
    // Decrease the indentation level
    void dedent();
    
    // Return a string to be used to indent the output
    std::string spacing();
    
    // Base case for the implementation of the println function.  Outputs
    // endl to end the line being output.
    void printlnImpl() {
        std::cout << std::endl;
    }
    
    // Recursive case for the implementation of the println function.  Uses
    // variadic templates to allow anything to be passed in to be output as
    // long as it has an appropriate operator<<.
    template<class T, class ...Rest>
    void printlnImpl(T&& item, Rest&& ...rest) {
        std::cout << std::forward<T>(item);
        printlnImpl(std::forward<Rest>(rest)...);
    }
    
    // Utility function to print out a series of items on a line out output.
    // Output is indented as specified by the current indentation level.  You
    // can pass as many arguments to this function as you want, as long as they
    // are able to be output via cout.
    template<class ...Rest>
    void println(Rest&& ...rest) {
        std::cout << spacing();
        printlnImpl(std::forward<Rest>(rest)...);
    }
    
    // How many indentation steps to indent the output
    int spaceCount = 0;
};

#endif /* ASTPRINTER_H */
