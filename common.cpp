#include "common.h"

std::string toString(MPLType t) {
    if (t == MPLType::INT) {
        return "INT";
    } else if (t == MPLType::STRING) {
        return "STRING";
    } else if (t == MPLType::BOOL){
        return "BOOL";
    } else {
        return "ARRAY";
    }
}
