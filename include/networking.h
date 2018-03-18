#ifndef _NETWORKING_H_
#define _NETWORKING_H_
#include <stdint.h>
#include <string.h>

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
    char* hostname;
    Api api;
    size_t version;
    char* api_end;
};

// user must set it in their program
extern const char* API_KEY;

//int get_request(struct Uri *uri, const char* API_KEY);

// uri builder takes a Uri struct and API key then returns the complete URI string
char* uri_builder(struct Uri *uri, char* result);


char* parse_region(Region region);
char* parse_api(Api api);
// adding this just in case not all apis correspond to the same version uri
// e.g /lol/summoner/v3/ /lol/match/v4
char* parse_version(size_t version);

#endif
