// This file will be deleted at some point
// It's only purpose is to test the library functions.


#include <stdio.h>
#include <curl/curl.h>
#include "networking.h"
#include "summoner.h"
#include "error.h"

int main(int argc, char* argv[]) {
  // setup a parser for this URI
  API_KEY = "RGAPI-b72ce6ec-b6ea-49d8-bc23-0f787df0d14c";

  // setup only necessary members that the build needs as input
  struct Uri uri;
  // TODO: passing a value from api struct, works on region as a value
  // since they both correspond to integer values.
  uri.region = NA;
  uri.api = SUMMONER;
  // uri.version is optional, defaults to v3
  uri.version = 3;

  // Create a buffer for uri_builder to return string data to then
  char str[512];

  // Pass uri struct as reference for less memory usage

  if(uri_builder(&uri, str) != ELISE_OK) {
      // printf("%s\n", err);
      // TODO: error handling
      return -1;
  }
  // create buffer to hold the summoner data in json
  char buff[1024];

  if (get_summoner_by_name(str, buff, "C9 Sneaky") != ELISE_OK) {
      // error
      return -1;
  }
  printf("%s\n", buff);
  //printf("%s : %d\n", buff, bytes);

  return 0;
}
