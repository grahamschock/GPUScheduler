#ifndef REQUEST_GUARD
#define REQUEST_GUARD
struct request {
  int prio;
  int matrix[512][512];
};

struct request* generate_request();
#endif
