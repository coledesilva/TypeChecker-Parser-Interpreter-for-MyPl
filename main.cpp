#include <iostream>
#include <string>

#include "Token.h"
#include "Lexeme.h"
#include "Lexer.h"
#include "Parser.h"
#include "ASTPrinter.h"
#include "Typechecker.h"
#include "Interpreter.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string code = R"code(
    running = true;
    x = 1;
    while running do
        if x <= 100 then
            println(x);
        x = x + 1;
        else
            running = false;
        end
    end
    )code";
    try {
        istringstream ins(code);
        Lexer lexer(ins);
        Parser parser(lexer);
        ASTPrinter printer;
        parser.getAST()->accept(printer);
        Typechecker checker;
        parser.getAST()->accept(checker);
        Interpreter interpreter;
        parser.getAST()->accept(interpreter);

    } catch (MyPLException &e) {
        cout << "Error encountered: " << e.what() << endl;
    }
    return 0;
}
