//
//  summoner.c
//  elise
//
#include "summoner.h"
#include "networking.h"
#include "error.h"


#include <stdio.h> //debuggging

// returns 0 on success, -1 on failure
errno_t get_summoner_by_name(char* uri_string, char* buffer, char* summoner_name) {
    errno = 0;
    char name[24];
    // Lets do a little error checking before manipulating these strings
    if(uri_string == NULL) return NULL_STRING;
    if (strlen(uri_string) == 0) return EMPTY_STRING;
    if(name == NULL) return NULL_STRING;
    if(strlen(name) == 0) return EMPTY_STRING;

    remove_spaces(summoner_name, name);
    //printf("%s\n", name);

    // couldn't think of a better name than bridge
    // it's the bridge between the first string and the last string
    // Also this is the maximum bytes possible for snprinf to write
    char bridge[512] = "/summoners/by-name/";
    // concatenate the uri string we created from the uri_builder
    // to the bridge and name that we entered when we called this function

    // use our newly formatted string as our uri for a GET request
    // but only if bytes were written ;)
    if (snprintf(uri_string, sizeof(bridge), "%s%s%s", uri_string, bridge, name) != 0) {
        //printf("%s\n", uri_string);
        // checks need to be done here ^
        if(get_request(uri_string, buffer, API_KEY) != 0){
            // checks need to be done here ^
            // error handling
            return errno;
        }
        return ELISE_OK;
    } return -1; //error

}

errno_t get_summoner_by_account_id(char* str, char* buffer, size_t id) {
    char bridge[512] = "/summoners/by-account/";
    return snprintf(buffer, sizeof(bridge), "%s%s%zu",str, bridge, id);
}
errno_t get_summoner_by_summoner_id(char* str, char* buffer, size_t id) {
    char bridge[512] = "/summoners/";
    return snprintf(buffer, sizeof(bridge), "%s%s%zu",str, bridge, id);
}

// Luckily only one api request supports usernames in text rather than by id
errno_t remove_spaces(char* name, char* buf) {
    if (name == NULL) return NULL_STRING;
    // Eventually double check the maxmimum character length of a summoner name
    if (strlen(name) > 24) {
        fprintf(stderr, "Error: The summoner name you've entered is longer than 24 characters.");
        return STRING_TOO_LARGE;
    }
    size_t i, j;
    for (i = 0, j = 0; i < strlen(name); i++, j++) {
        // if the character is a space, then we add 1 to i to make sure
        // we skip over it then continue copying until we reach the next space
        if(name[i] == ' ') i++;
        buf[j] = name[i];
    }buf[j] = '\0'; // always make sure to null terminate your strings ;)
    return ELISE_OK;
}
