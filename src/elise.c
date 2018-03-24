// This file will be deleted at some point
// It's only purpose is to test the library functions.


#include <stdio.h>
#include <curl/curl.h>
#include "networking.h"
#include "summoner.h"
#include "error.h"

int main(int argc, char* argv[]) {
  // setup a parser for this URI
  API_KEY = "RGAPI-d233bdf4-ade2-43e6-9ee5-f162a818e89f";

  // setup only necessary members that the build needs as input
  struct Uri uri;
  uri.region = NA;
  uri.api = SUMMONER;
  // uri.version is optional, defaults to v3
  uri.version = 3;

  // Create a buffer for uri_builder to return string data to then
  char str[512];

  // Pass uri struct as reference for less memory usage

  // uri builder needs to have another buffer for error messages
  // errno_t uri_builder(&uri, str, &err)
  if(uri_builder(&uri, str) != ELISE_OK) {\
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
