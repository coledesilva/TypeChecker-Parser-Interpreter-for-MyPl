#ifndef MYPLEXCEPTION_H
#define MYPLEXCEPTION_H

// You shouldn't need to modify this file for HW2

#include <exception>
#include <string>

// Base class to use for all exceptions in the MyPL implementation.
class MyPLException: public std::logic_error {
public:
    MyPLException(std::string msg = ""): logic_error(msg) {}
};

#endif /* MYPLEXCEPTION_H */
