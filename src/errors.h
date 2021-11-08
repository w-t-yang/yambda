#ifndef YAMBDA_ERRORS
#define YAMBDA_ERRORS

#define ERR_ELEMENT_INT_X_UNKNOWN "Unknow element type %d."

#define ERR_ENV_NOT_INITED "Environment is not initialized."

#define ERR_FUNCTION_COND_EXPECTS_ATOM_AT_X "Expect the %dth element to be an atom for \"cond\"."
#define ERR_FUNCTION_DIV_WITH_DIVISOR_ZERO "Divisor cannot be 0."
#define ERR_FUNCTION_INT_X_UNKNOWN "Unknow function type %d."
#define ERR_FUNCTION_INVALID "Invalid function."
#define ERR_FUNCTION_X_EXPECTS_Y "Function \"%s\" expects %s."
#define ERR_FUNCTION_X_NOT_IMPLEMENTED "`Function \"%s\" not implemented."
#define ERR_FUNCTION_X_WITH_INVALID_TYPE_Y "Function \"%s\" with invalid type \"%s\"."
#define ERR_FUNCTION_X_WITH_NULL_LIST "Cannot call function \"%s\" with NULL list."

#define ERR_IN_FORMAT_NO_SLASH_T "%s DO NOT use \\t in yambda script."
#define ERR_IN_INDENTATION_INVALID "%s Invalid indentation."
#define ERR_IN_INTEGER_X_INVALID "%s Invalid integer \"%s\""
#define ERR_IN_PAREN_INVALID "%s Invalid parenthesis."
#define ERR_IN_STRING_END_WITHOUT_WHITESPACE "%s End of string should be followed by whitespace."
#define ERR_IN_STRING_EOF_REACHED "%s Invalid string. End of file reached."

#define ERR_LAMBDA_INVALID "Invalid lambda function."
#define ERR_LAMBDA_TOO_MANY_ARGS "Lambda function expects no more than %d arguments."

#define ERR_SYMBOL_X_INVALID "Invalid symbol name \"%s\"."
#define ERR_SYMBOL_X_IS_RESERVED "Cannot use reserved symbol \"%s\"."
#define ERR_SYMBOL_X_NOT_FOUND "Symbol \"%s\" not found."

#define ERR_UNEXPECTED_FOR_X "Unexpected error in function \"%s\"."

#define ERR_WITH_NO_MESSAGE "Error with no message."


#endif
