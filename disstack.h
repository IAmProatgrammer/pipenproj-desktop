#ifndef DISSTACK_H
#define DISSTACK_H

#include <gtk/gtk.h>

typedef struct screen {
    GtkWidget *stack;
} screen_t;


/* Create the screen stack */
GtkWidget *createstack(void);


/* Add a screen to the stack */
void addscreen(
    screen_t *screen,
    GtkWidget *widget,
    const char *name
);


/* Switch to a screen */
void switchscreen(
    screen_t *screen,
    const char *name
);

#endif