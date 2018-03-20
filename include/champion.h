//
//  champion.h
//  elise
// contains methods for both champion-mastery and champion API
//

#ifndef _CHAMPION_H_
#define _CHAMPION_H_

#include "networking.h"
#include "error.h"
#include <stdbool.h>

// Dual usage in function arguments and data storage for json results
extern struct{
    size_t id;
    char* string;
    bool active;
    bool bot_enabled;
    bool bot_mme_enabled;
    bool free_enabled;
    bool ranked_enabled;
    char* icon_uri;
}Champion;

// champion-mastery-v3

// Get all champion mastery entries sorted by number of champion points
// in descending order
int get_champion_masteries(struct Uri *uri, size_t summoner_id);
// Get a champion mastery by player ID and champion ID.
int get_champion_mastery(struct Uri *uri, size_t summoner_id, size_t champion_id);
// Get a player's total champion mastery score, which is the sum of individual
// champion mastery levels.
int get_champion_mastery_score(struct Uri *uri, size_t summoner_id);

// champion-v3

// Get all champions that exist in the game
int get_all_champions(struct Uri *uri);
// Get free to play chammpions -- this function is based off of the one above
int get_free_to_play_champions();
// Get champion by id
int get_champion(size_t champion_id);

#endif
