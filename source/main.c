#include <gtk/gtk.h>

#include "disstack.h"
#include "login.h"

static void activate(
    GtkApplication *app,
    gpointer user_data
)
{
    GtkWidget *window;
    GtkWidget *stack;

    screen_t screen;

    window = gtk_application_window_new(app);

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
     * Create the actual GtkStack.
     */
    stack = createstack();

    /*
     * Store that GtkStack in the screen manager.
     */
    screen.stack = stack;

    /*
     * Create the login screens.
     */
    login_create(&screen);

    /*
     * Put the GtkStack into the window.
     */
    gtk_window_set_child(
        GTK_WINDOW(window),
        stack
    );

    gtk_window_present(
        GTK_WINDOW(window)
    );
}

int main(int argc, char **argv)
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

    status = g_application_run(
        G_APPLICATION(app),
        argc,
        argv
    );

    g_object_unref(app);

    return status;
}