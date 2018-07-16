#include <iostream>

#include "SymbolTable.h"

using namespace std;

// If you want every variable operation to be output, set this to true.
// Warning: this will be a lot of output.
const bool VERBOSE = false;

Symbol::Symbol() {
    type = MPLType::INT;
    data.intVal = 0;
}
Symbol::Symbol(const Symbol &other) {
    type = other.type;
    switch (other.type) {
        case MPLType::INT:
            data.intVal = other.data.intVal;
            break;
        case MPLType::STRING:
            data.strVal = new string(*other.data.strVal);
            break;
        case MPLType::BOOL:
            data.boolVal = other.data.boolVal;
            break;
        case MPLType::ARRAY:
            data.arrVal = new vector<Symbol>(*other.data.arrVal);
            break;
    }
}

Symbol::Symbol(int val) {
    type = MPLType::INT;
    setInt(val);
}

Symbol::Symbol(bool val) {
    type = MPLType::INT;
    setBool(val);
}

Symbol::Symbol(std::string val) {
    type = MPLType::INT;
    setString(val);
}

Symbol::Symbol(std::vector<Symbol> val) {
    type = MPLType::INT;
    setVector(val);
}

Symbol::~Symbol() {
    if (type == MPLType::STRING) {
        delete data.strVal;
    } else if (type == MPLType::ARRAY) {
        delete data.arrVal;
    }
}

Symbol& Symbol::operator=(const Symbol &other) {
    if (this == &other) {
        return *this;
    }
    if (type == other.type) {
        switch (type) {
            case MPLType::INT:
                data.intVal = other.data.intVal;
                break;
            case MPLType::BOOL:
                data.boolVal = other.data.boolVal;
                break;
            case MPLType::STRING:
                *data.strVal = *other.data.strVal;
                break;
            case MPLType::ARRAY:
                *data.arrVal = *other.data.arrVal;
        }
    } else {
        cleanup();
        type = other.type;
        switch (type) {
            case MPLType::INT:
                data.intVal = other.data.intVal;
                break;
            case MPLType::BOOL:
                data.boolVal = other.data.boolVal;
                break;
            case MPLType::STRING:
                data.strVal = new string(*other.data.strVal);
                break;
            case MPLType::ARRAY:
                data.arrVal = new vector<Symbol>(*other.data.arrVal);
        }
    }
    return *this;
}
MPLType Symbol::getType() {
    return type;
}
int Symbol::getInt() {
    if (type != MPLType::INT) {
        throw SymbolTableException("Attempting to read int from Symbol of type " + toString(type));
    }
    return data.intVal;
}
std::string& Symbol::getString() {
    if (type != MPLType::STRING) {
        throw SymbolTableException("Attempting to read string from Symbol of type " + toString(type));
    }
    return *data.strVal;
}
bool Symbol::getBool() {
    if (type != MPLType::BOOL) {
        throw SymbolTableException("Attempting to read bool from Symbol of type " + toString(type));
    }
    return data.boolVal;
}
std::vector<Symbol>& Symbol::getVector() {
    if (type != MPLType::ARRAY) {
        throw SymbolTableException("Attempting to read array from Symbol of type " + toString(type));
    }
    return *data.arrVal;
}
void Symbol::setInt(int val) {
    if (type != MPLType::INT) {
        cleanup();
        type = MPLType::INT;
        data.intVal = val;
    } else {
        data.intVal = val;
    }
}
void Symbol::setString(std::string val) {
    if (type != MPLType::STRING) {
        cleanup();
        type = MPLType::STRING;
        data.strVal = new string(val);
    } else {
        *data.strVal = val;
    }
}
void Symbol::setBool(bool val) {
    if (type != MPLType::BOOL) {
        cleanup();
        type = MPLType::BOOL;
        data.boolVal = val;
    } else {
        data.boolVal = val;
    }
}
void Symbol::setVector(std::vector<Symbol> val) {
    if (type != MPLType::ARRAY) {
        cleanup();
        type = MPLType::ARRAY;
        data.arrVal = new vector<Symbol>(val);
    } else {
        *data.arrVal = val;
    }
}
void Symbol::cleanup() {
    if (type == MPLType::STRING) {
        delete data.strVal;
    } else if (type == MPLType::ARRAY) {
        delete data.arrVal;
    }
}

void SymbolTable::pushTable() {
    if (VERBOSE) {
        cerr << "Symbol table push state" << endl;
    }
    scopes.push_back(map<string, Symbol>());
}

void SymbolTable::popTable() {
    if (VERBOSE) {
        cerr << "Symbol table pop state" << endl;
    }
    if (scopes.size() == 0) {
        throw SymbolTableException("Internal error: attempting to pop final table");
    }
    scopes.pop_back();
}

int SymbolTable::getIntVal(std::string name) {
    auto *info = getSymbol(name);
    if (!info) {
        throw SymbolTableException("Variable " + name + " doesn't exist");
    }
    return info->getInt();
}

std::string SymbolTable::getStringVal(std::string name) {
    auto *info = getSymbol(name);
    if (!info) {
        throw SymbolTableException("Variable " + name + " doesn't exist");
    }
    
    return info->getString();
}

bool SymbolTable::getBoolVal(std::string name) {
    auto *info = getSymbol(name);
    if (!info) {
        throw SymbolTableException("Variable " + name + " doesn't exist");
    }
    return info->getBool();
}

std::vector<Symbol>& SymbolTable::getVector(std::string name) {
    auto *info = getSymbol(name);
    if (!info) {
        throw SymbolTableException("Variable " + name + " doesn't exist");
    }
    return info->getVector();
}

void SymbolTable::storeIntVal(std::string name, int val) {
    if (VERBOSE) {
        cerr << "Storing " << val << " in " << name << endl;
    }
    auto *info = getSymbol(name);
    if (info && info->getType() != MPLType::INT) {
        throw SymbolTableException("Variable " + name + " is not of type int");
    }
    if (!info) {
        info = createSymbol(name);
    }
    info->setInt(val);
}

void SymbolTable::storeStringVal(std::string name, std::string val) {
    if (VERBOSE) {
        cerr << "Storing " << val << " in " << name << endl;
    }
    auto *info = getSymbol(name);
    if (info && info->getType() != MPLType::STRING) {
        throw SymbolTableException("Variable " + name + " is not of type string");
    }
    if (!info) {
        info = createSymbol(name);
    }
    info->setString(val);
}

void SymbolTable::storeBoolVal(std::string name, bool val) {
    if (VERBOSE) {
        cerr << "Storing " << val << " in " << name << endl;
    }
    auto *info = getSymbol(name);
    if (info && info->getType() != MPLType::BOOL) {
        throw SymbolTableException("Variable " + name + " is not of type bool");
    }
    if (!info) {
        info = createSymbol(name);
    }
    info->setBool(val);
}

void SymbolTable::storeVector(std::string name, std::vector<Symbol> val) {
    if (VERBOSE) {
        cerr << "Storing vector in " << name << endl;
    }
    auto *info = getSymbol(name);
    if (info && info->getType() != MPLType::ARRAY) {
        throw SymbolTableException("Variable " + name + " is not of type array");
    }
    if (!info) {
        info = createSymbol(name);
    }
    info->setVector(val);
}

bool SymbolTable::doesSymbolExist(std::string name) {
    return getSymbol(name);
}

MPLType SymbolTable::getSymbolType(std::string name) {
    auto *info = getSymbol(name);
    if (!info) {
        throw SymbolTableException("Internal error: Unable to find symbol " + name);
    }
    return info->getType();
}

Symbol *SymbolTable::getSymbol(std::string name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto itSymbol = it->find(name);
        if (itSymbol != it->end()) {
            return &itSymbol->second;
        }
    }
    
    return nullptr;
}

Symbol *SymbolTable::createSymbol(std::string name) {
    auto ans = scopes.back().insert(make_pair(name, Symbol()));
    return &ans.first->second;
}
