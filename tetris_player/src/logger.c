// Copyright 2011 Edwin Buck. Found on Stack Overflow.

#include <stdio.h>
#include <time.h>
#include "include/logger.h"

void log_info(const char *message) {
    time_t now;
    time(&now);
    printf("%s [INFO]: %s\n", ctime(&now), message);
}

void log_error(const char *message) {
    time_t now;
    time(&now);
    fprintf(stderr, "%s [ERROR]: %s\n", ctime(&now), message);
}
