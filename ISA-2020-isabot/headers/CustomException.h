/**
 * @file CustomException.h
 * @author David Sladký
 * @brief Declaration of CustomException class. Class to be thrown.
 */
#ifndef XSLADK07_CUSTOMEXCEPTION_H
#define XSLADK07_CUSTOMEXCEPTION_H

#include <string>

/**
 * Enum of error codes
 */
enum ErrorCodes
{
    TEMP = 1,
    OPTION_USED_MULTIPLE_TIMES,
    OPTION_REQUIRES_A_ARGUMENT,
    FATAL_ERROR,
    OPTION_UNKNOW,
    OPTION_MISSING,
    MISSING_TOKEN
};

/**
 * Class to be thrown when error occures. 
 */
class CustomException
{
    public:
        /**
         * A constructor.
         * Initializes error message and code of the class.
         */
        CustomException(const std::string message, const ErrorCodes code);
        /**
         * @return Error code.
         */
        ErrorCodes Code();
        /**
         * @return Error Message.
         */
        std::string Message();

    private:
        ErrorCodes errorCode;
        std::string errorMessage;
};

#endif //XSLADK07_CUSTOMEXCEPTION_H