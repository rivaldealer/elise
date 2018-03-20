// This file will be deleted at some point
// It's only purpose is to test the library functions.


#include <stdio.h>
#include <curl/curl.h>
#include "networking.h"
#include "summoner.h"

int main() {
  // setup a parser for this URI
  API_KEY = "RGAPI-b5bdb4af-d037-4639-889e-d7998f116bea";

  // setup only necessary members that the build needs as input
  struct Uri uri;
  uri.region = NA;
  uri.api = SUMMONER;
  // uri.version is optional, defaults to v3
  uri.version = 3;

  // Create a buffer for uri_builder to return string data to then
  char str[512];
  // Pass uri struct as reference for less memory usage
  if(uri_builder(&uri, str) <=0) {
      // TODO: error handling
      return -1;
  }
  // create buffer to hold the summoner data in json
  char buff[1024];
  int result = get_summoner_by_name(str, buff, "Deftsu");
  if (result != 0) {
      // error
      return -1;
  }
  printf("%s\n", buff);
  //printf("%s : %d\n", buff, bytes);

  return 0;
}
