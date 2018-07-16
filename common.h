#ifndef COMMON_H
#define COMMON_H

#include <string>

// Shared enum to use to represent the possible types in MyPL
enum class MPLType {
    BOOL, STRING, INT, ARRAY
};

std::string toString(MPLType t);

#endif /* COMMON_H */
