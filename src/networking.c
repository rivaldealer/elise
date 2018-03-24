//
//  networking.c
//  elise
//

// TODO: complete the URI -- uri_end
// create handlers for each Api enum

#include <stdlib.h>
#include <stdio.h>
#include "networking.h"
#include <curl/curl.h>

errno_t uri_builder(struct Uri *uri, char* buffer) {
    // checks need to be done here ^ to be sure they're not null or invalid
    // setup uri struct

    // more advanced erorr handling is to be done in the future
    char* region = parse_region(uri->region);
    // hostname is the same for every region as of this commit
    char* hostname = ".api.riotgames.com";
    char* api = parse_api(uri->api);
    char* version = parse_version(uri->version);

    char address[512] = "https://";
    // I'm sure there's a better way to do this, but this will do for now.
    // Also, oddly enough strcat_s is an optional feature for compilers using
    // the C11 standard.

    // note that it takes the size of address and not the size of the buffer
    // passed to this function
    // this way we know that it will never be more than 512 characters
    // if it needs to be, we change it here.


    //printf("%s : %lu : %lu\n", buffer, sizeof(buffer), strlen(buffer));
    // snprintf returns size_t, negative numbers aren't possible
    if (snprintf(buffer, sizeof(address), "%s%s%s%s%s", address, region, hostname, api, version) != 0) {
        return ELISE_OK;
    }
    // error
    return fprintf(stderr, "Error: %s\n", strerror(errno));
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
        // todo: return: REGION_UNSUPPORTED;
        // terrible, remove this v
        default: return "Error: Region not supported.\n";
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
        // also terrible
        default: return "Error: Requested API is not supported.\n";
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


// document the fuck out of this function.
// it's messy as fuck.

// currently returns 0 on success, -1 on failure
// TODO: proper error handling

// be more accurate with the names
// e.g. riot_get_request(), ddragon_get_request(), cdragon_get_request(), etc...
errno_t get_request(char* uri_string, char* buffer, const char* api_key) {
    // checks need to be done here ^

    // do some curl magics here
    // add api key to string

    // literal + 1;
    char query[10] = "?api_key=";
    char end[512];

    // TODO: error checking on the return value of this call
    snprintf(end, sizeof(end), "%s%s%s", uri_string, query, api_key);
    //printf("%s\n", end);
    CURL *curl;
    CURLcode res;
    char result[512];

    curl = curl_easy_init();
    if(curl) {
        //printf("%s\n", end);
      if(curl_easy_setopt(curl, CURLOPT_URL, end) != CURLE_OK) {
          // make new exit code for curl fails
          return -1;
      }
      /* example.com is redirected, so we tell libcurl to follow redirection */
      if(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L) != CURLE_OK) printf("Failed.\n");
      if(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback) != CURLE_OK) {
          // always try to free resources on failure
          // TODO: cleanup the other unchecked errors
          curl_easy_cleanup(curl);


          // write a new function to replace spaces with %20 so that curl will
          // properly get the request json
          // no need to overthink it.
          // also delete all thos printf("failed\n"); calls geez.
          return -1;
      }

      /* Perform the request, res will get the return code */
      res = curl_easy_perform(curl);
      /* Check for errors */
      if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    if(snprintf(buffer, sizeof(json), "%s", json) > 0) {
        //printf("wrote some shit \n");
        curl_easy_cleanup(curl);
        return 0;
    }
      /* always cleanup */
      curl_easy_cleanup(curl);
    }
    // attempt to write out our json to the buffer


    return -1; // error
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // checks need to be done here ^ on every argument to make sure they're not
    // null or invalid
    //printf("hello\n");
    size_t real_size = size * nmemb;
    //char data[512];
    printf("%s\n", ptr);
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
