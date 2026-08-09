#include "login.h"
#include "disstack.h"

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <security/pam_appl.h>
#include <string.h>


/* =========================================================
 * PAM authentication
 * ========================================================= */

typedef struct {
    const char *password;
} auth_data_t;


static int conversation(
    int num_msg,
    const struct pam_message **msg,
    struct pam_response **resp,
    void *data
)
{
    auth_data_t *auth = data;

    struct pam_response *responses =
        calloc(num_msg, sizeof(struct pam_response));

    if (responses == NULL)
        return PAM_CONV_ERR;

    for (int i = 0; i < num_msg; i++) {

        if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF) {

            responses[i].resp =
                strdup(auth->password);

            if (responses[i].resp == NULL) {
                free(responses);
                return PAM_CONV_ERR;
            }

        } else if (msg[i]->msg_style == PAM_TEXT_INFO ||
                   msg[i]->msg_style == PAM_ERROR_MSG) {

            /*
             * PAM may send informational/error messages.
             * We don't need to respond to them.
             */

            responses[i].resp = NULL;

        } else {

            for (int j = 0; j < i; j++)
                free(responses[j].resp);

            free(responses);

            return PAM_CONV_ERR;
        }
    }

    *resp = responses;

    return PAM_SUCCESS;
}


int authenticate_user(
    const char *username,
    const char *password
)
{
    if (username == NULL || password == NULL)
        return 0;

    auth_data_t data = {
        .password = password
    };

    struct pam_conv conv = {
        .conv = conversation,
        .appdata_ptr = &data
    };

    pam_handle_t *pamh = NULL;

    int result = pam_start(
        "login",
        username,
        &conv,
        &pamh
    );

    if (result != PAM_SUCCESS)
        return 0;

    result = pam_authenticate(
        pamh,
        0
    );

    pam_end(
        pamh,
        result
    );

    return result == PAM_SUCCESS;
}


/* =========================================================
 * Login data
 * ========================================================= */

typedef struct {

    /*
     * This pointer is safe because main.c keeps the
     * screen_t alive for the lifetime of the application.
     */
    screen_t *screen;

    GtkWidget *user_screen;
    GtkWidget *password_screen;

    GtkWidget *password_entry;

    char *username;

} LoginData;


/* =========================================================
 * Password screen callbacks
 * ========================================================= */

static void password_back(
    GtkButton *button,
    gpointer data
)
{
    (void)button;

    LoginData *login = data;

    if (login == NULL)
        return;

    switchscreen(
        login->screen,
        "users"
    );
}


static void password_login(
    GtkButton *button,
    gpointer data
)
{
    (void)button;

    LoginData *login = data;

    if (login == NULL)
        return;

    if (login->username == NULL)
        return;

    const char *password =
        gtk_editable_get_text(
            GTK_EDITABLE(login->password_entry)
        );

    if (authenticate_user(
        login->username,
        password
    )) {

        printf(
            "Login successful: %s\n",
            login->username
        );

        /*
         * Later you can switch to your actual
         * desktop/home screen here.
         */

    } else {

        printf(
            "Incorrect password for %s\n",
            login->username
        );

        gtk_editable_set_text(
            GTK_EDITABLE(login->password_entry),
            ""
        );
    }
}


/* =========================================================
 * Password screen
 * ========================================================= */

static GtkWidget *create_password_screen(
    LoginData *login
)
{
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *password;
    GtkWidget *login_button;
    GtkWidget *back_button;

    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        10
    );

    gtk_widget_set_margin_top(
        box,
        30
    );

    gtk_widget_set_margin_bottom(
        box,
        30
    );

    gtk_widget_set_margin_start(
        box,
        30
    );

    gtk_widget_set_margin_end(
        box,
        30
    );


    title = gtk_label_new(
        "Enter Password"
    );

    gtk_box_append(
        GTK_BOX(box),
        title
    );


    password = gtk_password_entry_new();

    gtk_widget_set_hexpand(
        password,
        TRUE
    );

    gtk_box_append(
        GTK_BOX(box),
        password
    );

    login->password_entry = password;


    login_button =
        gtk_button_new_with_label(
            "Login"
        );

    gtk_box_append(
        GTK_BOX(box),
        login_button
    );


    back_button =
        gtk_button_new_with_label(
            "Back"
        );

    gtk_box_append(
        GTK_BOX(box),
        back_button
    );


    g_signal_connect(
        login_button,
        "clicked",
        G_CALLBACK(password_login),
        login
    );

    g_signal_connect(
        back_button,
        "clicked",
        G_CALLBACK(password_back),
        login
    );


    /*
     * Pressing Enter in the password entry
     * can activate the login button.
     */
    gtk_password_entry_set_show_peek_icon(
        GTK_PASSWORD_ENTRY(password),
        TRUE
    );


    return box;
}


/* =========================================================
 * User selection callbacks
 * ========================================================= */

static void user_activated(
    GtkListBox *list,
    GtkListBoxRow *row,
    gpointer data
)
{
    (void)list;

    LoginData *login = data;

    if (login == NULL)
        return;

    if (row == NULL)
        return;


    GtkWidget *label =
        gtk_list_box_row_get_child(row);

    if (label == NULL)
        return;


    const char *username =
        gtk_label_get_text(
            GTK_LABEL(label)
        );


    free(login->username);

    login->username =
        strdup(username);

    if (login->username == NULL)
        return;


    printf(
        "clicked: %s\n",
        login->username
    );


    gtk_editable_set_text(
        GTK_EDITABLE(login->password_entry),
        ""
    );


    switchscreen(
        login->screen,
        "password"
    );
}


static void user_selected(
    GtkListBox *list,
    GtkListBoxRow *row,
    gpointer data
)
{
    (void)list;
    (void)data;

    if (row == NULL)
        return;


    GtkWidget *label =
        gtk_list_box_row_get_child(row);

    if (label == NULL)
        return;


    const char *username =
        gtk_label_get_text(
            GTK_LABEL(label)
        );


    printf(
        "Selected user: %s\n",
        username
    );
}


/* =========================================================
 * User selection screen
 * ========================================================= */

static GtkWidget *create_user_screen(
    LoginData *login
)
{
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *list;


    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        10
    );

    gtk_widget_set_margin_top(
        box,
        30
    );

    gtk_widget_set_margin_bottom(
        box,
        30
    );

    gtk_widget_set_margin_start(
        box,
        30
    );

    gtk_widget_set_margin_end(
        box,
        30
    );


    title = gtk_label_new(
        "Select User"
    );

    gtk_box_append(
        GTK_BOX(box),
        title
    );


    list = gtk_list_box_new();

    gtk_list_box_set_selection_mode(
        GTK_LIST_BOX(list),
        GTK_SELECTION_SINGLE
    );

    gtk_list_box_set_activate_on_single_click(
        GTK_LIST_BOX(list),
        TRUE
    );

    gtk_widget_set_vexpand(
        list,
        TRUE
    );

    gtk_box_append(
        GTK_BOX(box),
        list
    );


    /*
     * Get users from /etc/passwd.
     */

    struct passwd *user;

    setpwent();

    while ((user = getpwent()) != NULL) {

        /*
         * Only show normal users.
         */

        if (user->pw_uid < 1000)
            continue;

        if (user->pw_dir == NULL ||
            user->pw_dir[0] == '\0')
            continue;


        GtkWidget *row;
        GtkWidget *label;


        row = gtk_list_box_row_new();


        label = gtk_label_new(
            user->pw_name
        );


        gtk_widget_set_halign(
            label,
            GTK_ALIGN_START
        );


        gtk_list_box_row_set_child(
            GTK_LIST_BOX_ROW(row),
            label
        );


        gtk_list_box_append(
            GTK_LIST_BOX(list),
            row
        );
    }

    endpwent();


    /*
     * User selection changed.
     */

    g_signal_connect(
        list,
        "row-selected",
        G_CALLBACK(user_selected),
        NULL
    );


    /*
     * User activated.
     */

    g_signal_connect(
        list,
        "row-activated",
        G_CALLBACK(user_activated),
        login
    );


    /*
     * Select the first user.
     */

    GtkListBoxRow *first_row =
        gtk_list_box_get_row_at_index(
            GTK_LIST_BOX(list),
            0
        );


    if (first_row != NULL) {

        gtk_list_box_select_row(
            GTK_LIST_BOX(list),
            first_row
        );

        gtk_widget_grab_focus(
            GTK_WIDGET(first_row)
        );
    }


    return box;
}


/* =========================================================
 * Main login creation
 * ========================================================= */

void login_create(
    screen_t *screen
)
{
    if (screen == NULL)
        return;

    LoginData *login =
        calloc(1, sizeof(LoginData));

    if (login == NULL)
        return;


    /*
     * Keep the screen manager.
     */

    login->screen = screen;


    /*
     * Create user screen.
     */

    login->user_screen =
        create_user_screen(login);


    /*
     * Create password screen.
     */

    login->password_screen =
        create_password_screen(login);


    /*
     * Add both screens to the SAME GtkStack
     * owned by screen->stack.
     */

    addscreen(
        screen,
        login->user_screen,
        "users"
    );


    addscreen(
        screen,
        login->password_screen,
        "password"
    );


    /*
     * Start on the user screen.
     */

    switchscreen(
        screen,
        "users"
    );
}