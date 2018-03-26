//
//  summoner.h
//  elise
//
#include <stdlib.h> // size_t
#include <stdarg.h>
#include "error.h"
#include "networking.h"

extern struct {
    size_t account_id;
    char* name;
    size_t id;
}Summoner;

// typedef enum {
//     SUMMONER_BY_NAME,
//     SUMMONER_BY_ACCOUNT_ID,
//     SUMMONER_BY_SUMMONER_UD
// }Caller;

// TODO: Make uri a type so that you can't just pass any char* to it
errno_t get_summoner_by_name(char* str, char* buffer, char* name);
errno_t get_summoner_by_account_id(char* str, char* buffer, size_t id);
errno_t get_summoner_by_summoner_id(char* str, char* buffer, size_t id);
errno_t remove_spaces(char* name, char* bu);
