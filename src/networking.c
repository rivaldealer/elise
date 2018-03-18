#include <stdlib.h>
#include <stdio.h>
#include "networking.h"

char* uri_builder(struct Uri *uri, char* result) {
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
    size_t bytes_written = snprintf(result, sizeof(address), "%s%s%s%s",region, hostname, api, version);

    //printf("%s : %lu : %lu\n", result, sizeof(result), strlen(result));
    if (bytes_written != 0) {
        return result;
    }
    return "Error: Unable to write bytes to result";
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
        case 7:  return "na";
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
