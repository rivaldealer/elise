//
//  summoner.c
//  elise
//
#include "summoner.h"
#include "networking.h"


#include <stdio.h> //debuggging

// returns 0 on success, -1 on failure
int get_summoner_by_name(char* uri_string, char* buffer, char* name) {
    // couldn't think of a better name than bridge
    // it's the bridge between the first string and the last string
    // Also this is the maximum bytes possible for snprinf to write
    char bridge[512] = "/summoners/by-name/";
    // concatenate the uri string we created from the uri_builder
    // to the bridge and name that we entered when we called this function

    // use our newly formatted string as our uri for a GET request
    // but only if bytes were written ;)
    if (snprintf(uri_string, sizeof(bridge), "%s%s%s", uri_string, bridge, name) != 0) {
        if(get_request(uri_string, buffer, API_KEY) != 0){
            // error handling
            return -1;
        }
        //printf("%d\n", response);
        return 0;
    } return -1; //error

}

int get_summoner_by_account_id(char* str, char* buffer, size_t id) {
    char bridge[512] = "/summoners/by-account/";
    return snprintf(buffer, sizeof(bridge), "%s%s%zu",str, bridge, id);
}
int get_summoner_by_summoner_id(char* str, char* buffer, size_t id) {
    char bridge[512] = "/summoners/";
    return snprintf(buffer, sizeof(bridge), "%s%s%zu",str, bridge, id);
}
