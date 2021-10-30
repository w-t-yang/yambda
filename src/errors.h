#ifndef YAMBDA_ERRORS
#define YAMBDA_ERRORS

#define ERR_ENV_NOT_INITED "Environment is not initialized."
#define ERR_WITH_NO_MESSAGE "Error with no message."
#define ERR_FORMAT_NO_SLASH_T "DO NOT use \\t in yambda script."

#define ERR_FUNCTION_INVALID "Invalid function."
#define ERR_FUNCTION_X_WITH_NULL_LIST "Cannot call function \"%s\" with NULL list."
#define ERR_FUNCTION_X_EXPECTS_Y "Function \"%s\" expects %s."
#define ERR_FUNCTION_X_NOT_IMPLEMENTED "`Function \"%s\" not implemented."
#define ERR_FUNCTION_INT_X_UNKNOWN "Unknow function type %d."
#define ERR_ELEMENT_INT_X_UNKNOWN "Unknow element type %d."
#define ERR_FUNCTION_DIV_WITH_DIVISOR_ZERO "Divisor cannot be 0."
#define ERR_FUNCTION_COND_EXPECTS_ATOM_AT_X "Expect the %dth element to be an atom for \"cond\"."
#define ERR_FUNCTION_X_WITH_INVALID_TYPE_Y "Function \"%s\" with invalid type \"%s\"."
#define ERR_INDENTATION_INVALID_AROUND_X "Invalid indentation at \"%s\"."

#define ERR_INTEGER_X_INVALID "Invalid integer \"%s\""

#define ERR_PAREN_INVALID "Invalid parenthesis."

#define ERR_STRING_END_WITHOUT_WHITESPACE "End of string should be followed by whitespace."
#define ERR_STRING_EOF_REACHED "Invalid string. End of file reached."

#define ERR_SYMBOL_X_INVALID "Invalid symbol name \"%s\"."
#define ERR_SYMBOL_X_IS_RESERVED "Cannot use reserved symbol \"%s\"."
#define ERR_SYMBOL_X_NOT_FOUND "Symbol \"%s\" not found."

#define ERR_UNEXPECTED_FOR_X "Unexpected error in function \"%s\"."


#endif
