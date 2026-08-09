#include <gtk/gtk.h>

#include "disstack.h"
#include "login.h"


static void activate(
    GtkApplication *app,
    gpointer user_data
)
{
    (void)user_data;

    GtkWidget *window;
    GtkWidget *stack;

    /*
     * IMPORTANT:
     *
     * This is static so it remains alive after
     * activate() returns.
     *
     * The login callbacks keep a pointer to this
     * screen_t.
     */
    static screen_t screen;


    /*
     * Create window.
     */

    window =
        gtk_application_window_new(app);


    gtk_window_set_title(
        GTK_WINDOW(window),
        "Linux Desktop"
    );


    gtk_window_set_default_size(
        GTK_WINDOW(window),
        800,
        480
    );


    /*
     * Create the GtkStack.
     */

    stack = createstack();


    if (stack == NULL)
        return;


    /*
     * Give the screen manager ownership of
     * the GtkStack pointer.
     */

    screen.stack = stack;


    /*
     * Create login screens and add them
     * to this stack.
     */

    login_create(
        &screen
    );


    /*
     * Put the stack into the window.
     */

    gtk_window_set_child(
        GTK_WINDOW(window),
        stack
    );


    gtk_window_present(
        GTK_WINDOW(window)
    );
}


int main(
    int argc,
    char **argv
)
{
    GtkApplication *app;
    int status;


    app = gtk_application_new(
        "com.example.linuxdesktopui",
        G_APPLICATION_DEFAULT_FLAGS
    );


    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        NULL
    );


    status =
        g_application_run(
            G_APPLICATION(app),
            argc,
            argv
        );


    g_object_unref(app);


    return status;
} 