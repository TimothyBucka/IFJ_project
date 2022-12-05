#include "semantics.h"

data_type kw_to_data_type(keyword kw) {
    switch (kw) {
    case KW_VOID:
        return NULL_TYPE;
    case KW_INT:
        return INT;
    case KW_INT_NULL:
        return INT_NULL;
    case KW_FLOAT:
        return FLOAT;
    case KW_FLOAT_NULL:
        return FLOAT_NULL;
    case KW_STRING:
        return STRING;
    case KW_STRING_NULL:
        return STRING_NULL;
    default:
        return UNDEFINED;
    }
}
