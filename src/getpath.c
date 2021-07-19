#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal_stuff.h"

#ifndef USING_EMSCRIPTEN
#include "whereami.h"
#endif


char *__get_path__(const char *child) {
#ifdef USING_EMSCRIPTEN
  char loc[] = "build/";
  char *path = (char *) malloc(strlen(loc) + strlen(child));
  strcpy(path, loc);
#else
  int length = wai_getExecutablePath(NULL, 0, NULL);
  char *ex_path = (char *) malloc((size_t)(length + 1));
  int dirname_length;
  wai_getExecutablePath(ex_path, length, &dirname_length);
  ex_path[length] = '\0';


  int t = 0;
  for (int i = 0; i < length; ++i) {
    if (ex_path[i] == '\\') {
      t = i;
      ex_path[i] = '/';
    }
  }

  char *path = (char *) malloc((size_t)(t+1) + strlen(child));
  strncpy(path, ex_path, (size_t)(t+1));
  free(ex_path);
  path[t+1] = '\0';
#endif
  strcat(path, child);
  return path;
}
