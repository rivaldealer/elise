//
//  networking.c
//  elise
//
//  contains all networking function definitions of libelise

#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include "networking.h"


// returns ELISE_OK on success all other values indicate failure
errno_t uri_builder(struct Uri *uri, char* buffer) {
    // (void *) casts here to ignore compiler warnings
    // the result will be NULL if it's not set by the user so this is still
    // a proper and necessary check
    if ((void*)uri->region == NULL) { // user forgot to specify region
        fprintf(stderr, "Error: Region is either not supported or isn't specified in URI struct.\n");
        return NULL_REGION;
    }
    if ((void*)uri->api == NULL) {
        fprintf(stderr, "Error: API is either not supported or isn't specified in URI struct.\n");
        return NULL_API;
    }
    // no checks needed for version in uri, because we set it even if the user
    // doesn't specify it.

    char* region = parse_region(uri->region);
    if (region == NULL) {
        // probably should change this so that the error handling makes more sense
        // look at comments above parse_region()
        return NULL_REGION;
    }
    // hostname is the same for every region as of this commit
    const char* hostname = ".api.riotgames.com";
    char* api = parse_api(uri->api);
    if (api == NULL) return NULL_API;
    char* version = parse_version(uri->version);

    // note that snprinf takes the size of address and not the size of the buffer
    // passed to this function
    // this way we know that it will never be more than 512 characters
    // if it needs to be, we change it here.
    char address[512] = "https://";

    errno = 0;
    // Eventually check to make sure all bytes from each string where written
    // Right now it just assumes if it wrote more than 0 then it wrote all of the bytes
    if (snprintf(buffer, sizeof(address), "%s%s%s%s%s", address, region, hostname, api, version) > 0) {
        return ELISE_OK;
    }
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return -1;
}

// instead of returning a string, return an errno_t instead and pass a buffer
// by reference to modify it and return it while still being able to produce
// decent error checking
char* parse_region(Region region) {
    // switch through the region enum and return the regional endpoint
    switch (region) {
        case 0:  return "br1";
        case 1:  return "eun1";
        case 2:  return "euw1";
        case 3:  return "jp1";
        case 4:  return "kr";
        case 5:  return "la1";
        case 6:  return "la2";
        case 7:  return "na1";
        case 8:  return "oc1";
        case 9:  return "tr1";
        case 10: return "ru";
        case 11: return "pbe1";
        default: fprintf(stderr, "Error: Region specified is not supported.\n");
        return NULL;
    }
}

char* parse_api(Api api) {
    switch (api) {
        case 0: return "/lol/champion-mastery/";
        case 1: return "/lol/platform/";
        case 2: return "/lol/league/";
        case 3: return "/lol/static-data/";
        case 4: return "/lol/status/";
        case 5: return "/lol/match/";
        case 6: return "/lol/specator/";
        case 7: return "/lol/summoner/";
        case 8: return "/lol/tournament-stub/";
        case 9: return "/lol/tournament/";
        default: fprintf(stderr, "Error: API specified is not supported.\n");
        return NULL;
    }
}

char* parse_version(size_t version){
    // only supporting one revision higher in case I don't update this
    // immediately.
    switch (version) {
        case 3: return "v3";
        case 4: return "v4";
        default: return "v3";
    }
}

// returns ELISE_OK on success
// returns CURL_ERROR if curl checks fail.
// returns NULL_X or STRING_X if it runs into user input issues
errno_t riot_get_request(char* uri_string, char* buffer, const char* api_key) {
    if (uri_string == NULL) return NULL_STRING;
    // API_KEY erorr checking
    if (API_KEY == NULL) {
        fprintf(stderr, "Error: API_KEY not set.\n");
        return NULL_STRING;
    }if (strlen(API_KEY) > 42) {
        // current api key is RGAPI-[+36 characters].
        // This is for dev keys, I'm not sure about production level keys.
        fprintf(stderr, "Error: API Key is longer than 42 characters.\n");
        return STRING_TOO_LARGE;
    }if (strlen(API_KEY) < 42) {
        fprintf(stderr, "Error: API Key is shorter than 42 characters.\n");
        return STRING_TOO_SMALL;
    }

    // literal + 1;
    const char query[10] = "?api_key=";
    char end[512];

    int check = snprintf(end, sizeof(end), "%s%s%s", uri_string, query, api_key);
    if (check != strlen(end)) {
        fprintf(stderr, "Error: Not all bytes were able to be written to string end.\n");
        return -1;
    }

    // start curl magics
    // probably should fold all of this into a function for the next x_get_request() functions.
    CURL *curl;
    CURLcode res;
    char result[512];

    curl = curl_easy_init();
    if(curl) {
        // eventually write errors out to the stderr, for consistency
        // see CURLcode res below
        if(curl_easy_setopt(curl, CURLOPT_URL, end) != CURLE_OK) {
            curl_easy_cleanup(curl);
            return CURL_ERROR;
        }
        if(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L) != CURLE_OK) {
            curl_easy_cleanup(curl);
            return CURL_ERROR;
        }
        // specify a callback function to be called when the response is received
        if(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback) != CURLE_OK) {
            // always try to free resources on failure
            curl_easy_cleanup(curl);
            return CURL_ERROR;
        }
        // perform the get request and store the result in res
        res = curl_easy_perform(curl);
        // proper curl error checking
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // attempt to write our json response out to our buffer
        // ideally we want to return from here
        if(snprintf(buffer, sizeof(json), "%s", json) > 0) {
            curl_easy_cleanup(curl);
            return ELISE_OK;
        }
        // if our write fails, we clean up and return -1;
        curl_easy_cleanup(curl);
        return CURL_ERROR;
    }
    // if for some reason curl is inaccesable we will land here.
    fprintf(stderr, "Error: Undefined curl behavior, please submit an issue on the libelise github.\n");
    return CURL_ERROR;
    // end curl magics
}

// curl's integrated callback for passing response data to a function
// returns size of the response data on success, any other value indicates failure
static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    if(ptr == NULL) {
        fprintf(stderr, "Error: Curl response data is null in write_callback().\n");
        // curl has an error message if we return from this function without the bytes written
        return 0;
    }
    // null or invalid
    //printf("hello\n");
    size_t real_size = size * nmemb;
    //char data[512];
    // we add one to real_size, otherwise we lose the json ending bracket
    if (snprintf(json, real_size+1, "%s", ptr) != 0) {
        // checks need to be done here ^
        // printf("%lu\n", real_size);
        // printf("%s\n", json);
        // this function must return real_size or curl will think the function
        // failed to write any data
        return real_size;
    } return CALLBACK_ERROR;
}
