#ifndef DISSTACK_H
#define DISSTACK_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *stack;
} screen_t;

/*
 * Creates the global GtkStack.
 */
GtkWidget *createstack(void);

/*
 * Adds a screen to the global stack.
 */
void addscreen(
    screen_t *screen,
    GtkWidget *widget,
    const char *name
);

/*
 * Switches screens and loads that screen's CSS.
 */
void switchscreen(
    screen_t *screen,
    const char *name
);

#endif