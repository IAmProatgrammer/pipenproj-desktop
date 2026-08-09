#include "disstack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * The GtkStack used by the whole application.
 */
static GtkWidget *global_stack = NULL;

/*
 * The CSS provider currently installed.
 */
static GtkCssProvider *current_css = NULL;


/*
 * Remove the currently loaded CSS.
 */
static void unload_css(void)
{
    if (current_css == NULL)
        return;

    GdkDisplay *display =
        gdk_display_get_default();

    if (display != NULL) {

        gtk_style_context_remove_provider_for_display(
            display,
            GTK_STYLE_PROVIDER(current_css)
        );
    }

    g_object_unref(current_css);

    current_css = NULL;
}


/*
 * Load the CSS belonging to a screen.
 *
 * Example:
 *
 * switchscreen(screen, "users");
 *
 * loads:
 *
 * assets/css/users.css
 */
static void load_screen_css(
    const char *screen
)
{
    if (screen == NULL)
        return;

    GdkDisplay *display =
        gdk_display_get_default();

    if (display == NULL)
        return;


    /*
     * Remove the previous screen's CSS.
     */
    unload_css();


    /*
     * Create the new CSS provider.
     */
    current_css =
        gtk_css_provider_new();


    char path[512];

    snprintf(
        path,
        sizeof(path),
        "assets/css/%s.css",
        screen
    );


    /*
     * Load the CSS file.
     */
    gtk_css_provider_load_from_path(
        current_css,
        path
    );


    /*
     * Install the new CSS.
     */
    gtk_style_context_add_provider_for_display(
        display,
        GTK_STYLE_PROVIDER(current_css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );


    printf(
        "Loaded CSS: %s\n",
        path
    );
}


/*
 * Create the application stack.
 */
GtkWidget *createstack(void)
{
    global_stack =
        gtk_stack_new();


    gtk_stack_set_transition_type(
        GTK_STACK(global_stack),
        GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT
    );


    gtk_stack_set_transition_duration(
        GTK_STACK(global_stack),
        300
    );


    return global_stack;
}


/*
 * Add a screen to the global stack.
 */
void addscreen(
    screen_t *screen,
    GtkWidget *widget,
    const char *name
)
{
    (void)screen;

    if (global_stack == NULL) {

        printf(
            "ERROR: global stack NULL\n"
        );

        return;
    }


    if (widget == NULL)
        return;


    if (name == NULL)
        return;


    gtk_stack_add_named(
        GTK_STACK(global_stack),
        widget,
        name
    );


    printf(
        "Added screen: %s\n",
        name
    );
}


/*
 * Switch screen AND switch CSS.
 */
void switchscreen(
    screen_t *screen,
    const char *name
)
{
    (void)screen;

    if (global_stack == NULL) {

        printf(
            "ERROR: global stack NULL\n"
        );

        return;
    }


    if (name == NULL)
        return;


    printf(
        "Switching to '%s'\n",
        name
    );


    /*
     * Change the visible GTK screen.
     */
    gtk_stack_set_visible_child_name(
        GTK_STACK(global_stack),
        name
    );


    /*
     * Load the CSS belonging to it.
     */
    load_screen_css(name);
}