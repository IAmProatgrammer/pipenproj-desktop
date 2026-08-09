#ifndef DISSTACK_H
#define DISSTACK_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *stack;
} screen_t;

GtkWidget *createstack(void);

void addscreen(
    screen_t *screen,
    GtkWidget *widget,
    const char *name
);

void switchscreen(
    screen_t *screen,
    const char *name
);

#endif