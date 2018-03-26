//
//  summoner.c
//  elise
//
//  contains all summoner request function definitions of libelise
#include "summoner.h"
#include "networking.h"
#include "error.h"


#include <stdio.h> //debuggging

// returns ELISE_OK on success, all other values indicate failure
errno_t get_summoner_by_name(char* uri_string, char* buffer, char* summoner_name) {
    char name[24];
    // Lets do a little error checking before manipulating these strings
    if(uri_string == NULL) return NULL_STRING;
    if (strlen(uri_string) == 0) return STRING_EMPTY;
    if(summoner_name == NULL) return NULL_STRING;
    if(strlen(summoner_name) == 0) return STRING_EMPTY;

    // Error handling message is done within the function
    if (remove_spaces(summoner_name, name) != ELISE_OK) return -1;

    // couldn't think of a better name than bridge
    // it's the bridge between the first string and the last string
    // Also this is the maximum bytes possible for snprinf to write
    char bridge[512] = "/summoners/by-name/";
    // concatenate the uri string we created from the uri_builder
    // to the bridge and summoner_name that we entered when we called this function

    // use our newly formatted string as our uri for a GET request
    // but only if bytes were written ;)
    // the check makes sure bytes written is > 0, but should really check to make sure
    // all of the bytes were written in the future.
    if (snprintf(uri_string, sizeof(bridge), "%s%s%s", uri_string, bridge, name) > 0) {
        // returns -1 if riot_get_request fails for any reason
        // as well as showing any error that was handled by the fucntion itself
        // that returned anything but ELISE_OK
        if(riot_get_request(uri_string, buffer, API_KEY) != ELISE_OK) return -1;
        return ELISE_OK;
    }
    fprintf(stderr, "Error: Unable to create bridge ending string in get_summoner_by_name().\n");
    return -1;
}

errno_t get_summoner_by_account_id(char* str, char* buffer, size_t id) {
    char bridge[512] = "/summoners/by-account/";
    return snprintf(buffer, sizeof(bridge), "%s%s%zu",str, bridge, id);
}
errno_t get_summoner_by_summoner_id(char* str, char* buffer, size_t id) {
    char bridge[512] = "/summoners/";
    return snprintf(buffer, sizeof(bridge), "%s%s%zu",str, bridge, id);
}

// Only one api request supports usernames in text rather than by id
// returns ELISE_OK on success, all other values indicate failure
errno_t remove_spaces(char* name, char* buf) {
    if (name == NULL) return NULL_STRING;
    // Maximum summoner name length is 16 characters
    if (strlen(name) > 16) {
        fprintf(stderr, "Error: The summoner name you've entered is longer than the 16 characters supported by Riot.");
        return STRING_TOO_LARGE;
    }
    // start removing spaces from the name
    size_t i, j;
    for (i = 0, j = 0; i < strlen(name); i++, j++) {
        // if the character is a space, then we add 1 to i to make sure
        // we skip over it then continue copying until we reach the next space
        if(name[i] == ' ') i++;
        buf[j] = name[i];
    }buf[j] = '\0'; // always make sure to null terminate your strings ;)
    return ELISE_OK;
}
