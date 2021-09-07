#include <stdio.h>
#include <stdlib.h>
#include "request.h"

struct request* generate_request() {
  struct request* req = calloc(1, sizeof(struct request));
  int r = 0;
  for (int i = 0; i < 512; i++) {
    for (int j = 0; j < 512; j++) {
      r = rand() % 25;
      req->matrix[i][j] = r;
    }
  }

  req->prio = 0;
  return req;
}
