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

int uri_builder(struct Uri *uri, char* buffer) {
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
    size_t bytes_written = snprintf(buffer, sizeof(address), "%s%s%s%s%s", address, region, hostname, api, version);

    //printf("%s : %lu : %lu\n", buffer, sizeof(buffer), strlen(buffer));
    if (bytes_written != 0) {
        return bytes_written;
    }
    // error
    return fprintf(stderr, "Error: %s\n", strerror(errno));
}

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
int get_request(char* uri_string, char* buffer, const char* api_key) {
    // do some curl magics here
    // add api key to string

    // literal + 1;
    char query[10] = "?api_key=";
    char end[512];

    snprintf(end, sizeof(end), "%s%s%s", uri_string, query, api_key);
    CURL *curl;
    CURLcode res;
    char result[512];

    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, end);
      /* example.com is redirected, so we tell libcurl to follow redirection */
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
      if(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback) != CURLE_OK) {
          return -1;
      }
      /* Perform the request, res will get the return code */
      res = curl_easy_perform(curl);
      /* Check for errors */
      if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

      /* always cleanup */
      curl_easy_cleanup(curl);
    }
    // attempt to write out our json to the buffer
    if(snprintf(buffer, sizeof(json), "%s", json) != 0) {
        return 0;
    }
    return -1; // error
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t real_size = size * nmemb;
    //char data[512];
    if (snprintf(json, real_size, "%s", ptr) != 0) {
        // printf("%lu\n", real_size);
        // printf("%s\n", json);
        // this function must return real_size or curl will think the function
        // failed to write any data
        return real_size;
    } return -1;
}
