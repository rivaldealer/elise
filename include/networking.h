//
//  networking.h
//  elise
//

#ifndef _NETWORKING_H_
#define _NETWORKING_H_
#include <stdint.h>
#include <string.h>
#include "error.h"

// current regional endpoints:
//      https://developer.riotgames.com/regional-endpoints.html
//
typedef enum {
    BR,
    EUNE,
    EUW,
    JP,
    KR,
    LAN,
    LAS,
    NA,
    OCE,
    TR,
    RU,
    PBE
}Region;

typedef enum {
    CHAMPION_MASTERY,
    CHAMPION,
    LEAGUE,
    STATIC_DATA,
    STATUS,
    MATCH,
    SPECTATOR,
    SUMMONER,
    // Remember to implement regional proxies for TOURNAMENT
    // https://developer.riotgames.com/regional-endpoints.html
    //
    TOURNAMENT_STUB,
    TOURNAMENT
}Api;

// https://region.hostname/lol/api/version/api_end
struct Uri {
    Region region;
    Api api;
    size_t version;
    char* api_end;
};

// user must set it in their program
const char* API_KEY;
static char json[512];

//int get_request(struct Uri *uri, const char* API_KEY);

// uri builder takes a Uri struct and API key then returns the complete URI string
errno_t uri_builder(struct Uri *uri, char* buffer);
// complete uri is to be the handler for all api endings
errno_t riot_get_request(char* uri_string, char* buffer, const char* api_key);

char* parse_region(Region region);
char* parse_api(Api api);
// adding this just in case not all apis correspond to the same version uri
// e.g /lol/summoner/v3/ /lol/match/v4
char* parse_version(size_t version);

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

#endif
