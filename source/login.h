#ifndef LOGIN_H
#define LOGIN_H

#include "disstack.h"

/*
 * Create all login screens and add them
 * to the application's GtkStack.
 */
void login_create(
    screen_t *screen
);

/*
 * Authenticate a Linux user using PAM.
 */
int authenticate_user(
    const char *username,
    const char *password
);

#endif