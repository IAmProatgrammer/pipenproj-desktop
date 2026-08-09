#include "disstack.h"
#include <stdio.h>

GtkWidget *createstack(void)
{
    GtkWidget *stack;

    stack = gtk_stack_new();

    gtk_stack_set_transition_type(
        GTK_STACK(stack),
        GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT
    );

    gtk_stack_set_transition_duration(
        GTK_STACK(stack),
        300
    );

    return stack;
}

void addscreen(
    screen_t *screen,
    GtkWidget *widget,
    const char *name
)
{
    if (screen == NULL) {
        printf("ERROR: screen NULL\n");
        return;
    }

    if (screen->stack == NULL) {
        printf("ERROR: stack NULL\n");
        return;
    }

    if (!GTK_IS_STACK(screen->stack)) {
        printf("ERROR: stack is not a GtkStack\n");
        return;
    }

    if (widget == NULL) {
        printf("ERROR: widget NULL\n");
        return;
    }

    if (name == NULL) {
        printf("ERROR: name NULL\n");
        return;
    }

    printf("Adding screen '%s'\n", name);

    gtk_stack_add_named(
        GTK_STACK(screen->stack),
        widget,
        name
    );
}

void switchscreen(
    screen_t *screen,
    const char *name
)
{
    if (screen == NULL) {
        printf("ERROR: screen NULL\n");
        return;
    }

    if (screen->stack == NULL) {
        printf("ERROR: stack NULL\n");
        return;
    }

    if (!GTK_IS_STACK(screen->stack)) {
        printf("ERROR: stack is not a GtkStack\n");
        return;
    }

    if (name == NULL) {
        printf("ERROR: name NULL\n");
        return;
    }

    printf("Switching to '%s'\n", name);

    gtk_stack_set_visible_child_name(
        GTK_STACK(screen->stack),
        name
    );
}