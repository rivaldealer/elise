//
//  error.h
//  elise
//
// contains all relevant error handling
#ifndef _ERROR_H_
#define _ERROR_H_
#include <errno.h>

#define ELISE_OK 0


enum {
    INVALID_STRING = 1,
    NULL_STRING,
    NULL_REGION,
    NULL_API,
    STRING_TOO_LARGE,
    EMPTY_STRING,
    CALLBACK_ERROR, // specific to the write_callback function
}ErrorCodes;

#endif
// do some error handling
