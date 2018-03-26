// This file will be deleted at some point
// It's only purpose is to test the library functions.

#include <stdio.h>
#include <curl/curl.h>
#include "networking.h"
#include "summoner.h"
#include "error.h"

int main(int argc, char* argv[]) {

    API_KEY = "";
    // setup only necessary members that the build needs as input
    struct Uri uri;
    // BUG: passing a value from api struct, works on region as a value
    // since they both correspond to integer values.
    uri.region = NA;
    uri.api = SUMMONER;
    // uri.version is optional, defaults to v3
    uri.version = 3;

    // Create a buffer for uri_builder to return string data to then
    char str[512];

    // Pass uri struct as reference for less memory usage
    if(uri_builder(&uri, str) != ELISE_OK) {
        printf("How unlucky, we failed to construct your request uri\n");
        return -1;
    }
    // create buffer to hold the summoner data in json
    char summoner_data[1024];

    if (get_summoner_by_name(str, summoner_data, "C9 Sneaky") != ELISE_OK) {
        printf("Your own error message.\n");
        return -1;
    }
    printf("%s\n", summoner_data);
    return 0;
}
