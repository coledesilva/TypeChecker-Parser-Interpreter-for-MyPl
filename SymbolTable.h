#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <map>
#include <string>

#include "MyPLException.h"
#include "common.h"

class SymbolTableException: public MyPLException {
public:
    SymbolTableException(std::string msg=""): MyPLException(msg) {}
};

// Represents a type and a value in MyPL.  Has support for int, bool, string,
// and vector/array.  Vectors contain Symbols, which can themselves be vectors,
// allowing for nested arrays.  Symbols can be viewed as being a discriminated
// union, as long as you limit yourself to ONLY using the public member
// functions.  It is perfectly fine to take a Symbol of one type and change
// the type that is stored inside of it.  If you attempt to get the wrong
// type out of Symbol, a SymbolTableException will be thrown.
class Symbol {
public:
    // Create a new symbol with a valid state (currently, int 0)
    Symbol();
    
    // Copy constructor
    Symbol(const Symbol &other);
    
    // Create a new int Symbol with the specified value
    Symbol(int val);
    
    // Create a new bool Symbol with the specified value
    Symbol(bool val);
    
    // Create a new string Symbol with the specified value.  This will make
    // a copy of the passed-in value, so changes made to the string that was
    // passed to this function will NOT change what is in the symbol.
    Symbol(std::string val);
    
    // Create a new vector Symbol with the specified value.  This will make
    // a copy of the passed-in value, so changes made to the string that was
    // passed to this function will NOT change what is in the symbol.
    Symbol(std::vector<Symbol> val);
    
    // Destructor
    ~Symbol();
    
    // Assignment operator
    Symbol& operator=(const Symbol &other);
    
    // Returns current type in the symbol
    MPLType getType();
    
    // Returns the int value stored in this symbol.  If an int isn't stored,
    // this will throw an exception.
    int getInt();
    
    // Returns the string value stored in this symbol.  If a string isn't
    // stored, this will throw an exception.  This returns a reference to
    // the string value so that you can use this to change the contents
    // of the string
    std::string& getString();
    
    // Returns the bool value stored in this symbol.  If a bool isn't stored,
    // this will throw an exception.
    bool getBool();
    
    // Returns the vector value stored in this symbol.  If a vector isn't
    // stored, this will throw an exception.  This returns a reference to the
    // vector so that you are able to change the vector stored in this
    // symbol.
    std::vector<Symbol>& getVector();
    
    // Stores an integer and changes the type of this Symbol to be int
    void setInt(int val);
    
    // Stores a string and changes the type of this Symbol to be string
    void setString(std::string val);
    
    // Stores a boolean and changes the type of the Symbol to be boolean
    void setBool(bool val);
    
    // Stores a vector and changes the type of the Symbol to be vector
    void setVector(std::vector<Symbol> val);
private:
    
    // Properly clean up the current type in the union
    void cleanup();
    
    // Current type
    MPLType type;
    
    // Current data being stored.  The string and vector are stored as pointers
    // because union members must be trivially constructable.
    union {
        int intVal;
        std::string *strVal;
        bool boolVal;
        std::vector<Symbol> *arrVal;
    } data;
};

class SymbolTable {
public:
    // Enter a new nested scope
    void pushTable();
    
    // Exit the current scope
    void popTable();
    
    // Returns the integer associated with the given name.  If that name
    // doesn't exist, or if the variable is not an integer, throws an
    // exception.
    int getIntVal(std::string name);
    
    // Returns the string associated with the given name.  If that name
    // doesn't exist, or if the variable is not a string, throws an
    // exception.
    std::string getStringVal(std::string name);
    
    // Returns the boolean associated with the given name.  If that name
    // doesn't exist, or if the variable is not a boolean, throws an
    // exception.
    bool getBoolVal(std::string name);
    
    // Returns the vector associated with the given name.  If that name
    // doesn't exist, or if the variable is not a vector, throws an
    // exception.
    std::vector<Symbol>& getVector(std::string name);
    
    // Stores an integer value to be associated with the given name.  If
    // name refers to an existing integer variable, the value is changed.
    // If name doesn't exist, it is created and associated with the specified
    // value.  If it is a variable of a different type, an exception is thrown.
    void storeIntVal(std::string name, int val);
    
    // Stores a string value to be associated with the given name.  If
    // name refers to an existing string variable, the value is changed.
    // If name doesn't exist, it is created and associated with the specified
    // value.  If it is a variable of a different type, an exception is thrown.
    void storeStringVal(std::string name, std::string val);
    
    // Stores a boolean value to be associated with the given name.  If
    // name refers to an existing boolean variable, the value is changed.
    // If name doesn't exist, it is created and associated with the specified
    // value.  If it is a variable of a different type, an exception is thrown.
    void storeBoolVal(std::string name, bool val);
    
    // Stores a vector value to be associated with the given name.  If
    // name refers to an existing vector variable, the value is changed.
    // If name doesn't exist, it is created and associated with the specified
    // value.  If it is a variable of a different type, an exception is thrown.
    //
    // Note that this does NOT do typechecking to ensure that an array of int
    // isn't changed into an array of string, or array of array of int.
    void storeVector(std::string name, std::vector<Symbol> val);
    
    // Returns true if the given name is defined
    bool doesSymbolExist(std::string name);
    
    // Returns the type associated with the given name.  If the name is not
    // defined, throws an exception.
    MPLType getSymbolType(std::string name);
private:
    // Internal helper function that will return a pointer to the Symbol
    // with the given name.  If no such Symbol exists, returns nullptr.
    Symbol *getSymbol(std::string name);
    
    // Creates a new Symbol in the current scope with the given name.  Note
    // that this function does NOT check to see if a symbol exists with that
    // name, so this function can be (mis-)used to shadow a variable in an
    // outer scope.
    Symbol *createSymbol(std::string name);
    
    // Vector of maps of strings to symbol.  The vector contains elements
    // ranging from the outermost scope to the innermost.  Inside the map,
    // variable names (std::string) are used to look up the corresponding
    // Symbols.
    std::vector<std::map<std::string, Symbol>> scopes;
};

#endif /* SYMBOLTABLE_H */
