#ifndef LOGIN_H
#define LOGIN_H

#include "disstack.h"

void login_create(screen_t *screen);

int authenticate_user(
    const char *username,
    const char *password
);

#endif