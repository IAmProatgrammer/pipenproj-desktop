#include "login.h"
#include "disstack.h"

#include <gtk/gtk.h>

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <security/pam_appl.h>
#include "desktop.h"

/* =========================================================
 * PAM authentication
 * =========================================================
 */

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
        calloc(
            num_msg,
            sizeof(struct pam_response)
        );


    if (responses == NULL)
        return PAM_CONV_ERR;


    for (int i = 0; i < num_msg; i++) {

        if (msg[i]->msg_style ==
            PAM_PROMPT_ECHO_OFF) {

            responses[i].resp =
                strdup(auth->password);


            if (responses[i].resp == NULL) {

                for (int j = 0; j < i; j++)
                    free(responses[j].resp);

                free(responses);

                return PAM_CONV_ERR;
            }

        }

        else if (
            msg[i]->msg_style == PAM_TEXT_INFO ||
            msg[i]->msg_style == PAM_ERROR_MSG
        ) {

            responses[i].resp = NULL;

        }

        else {

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
    if (username == NULL ||
        password == NULL)
        return 0;


    auth_data_t data = {
        .password = password
    };


    struct pam_conv conv = {
        .conv = conversation,
        .appdata_ptr = &data
    };


    pam_handle_t *pamh = NULL;


    int result =
        pam_start(
            "login",
            username,
            &conv,
            &pamh
        );


    if (result != PAM_SUCCESS)
        return 0;


    result =
        pam_authenticate(
            pamh,
            0
        );


    pam_end(
        pamh,
        result
    );

    if (result == PAM_SUCCESS) {
        void desktop_create(
            screen_t *screen
        );
        
    }
    return result == PAM_SUCCESS;
}


/* =========================================================
 * Login data
 * =========================================================
 */

typedef struct {

    screen_t *screen;

    GtkWidget *user_screen;
    GtkWidget *password_screen;

    GtkWidget *password_entry;

    char *username;

} LoginData;


/* =========================================================
 * Password screen callbacks
 * =========================================================
 */

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
            GTK_EDITABLE(
                login->password_entry
            )
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
         * Later:
         *
         * switchscreen(
         *     login->screen,
         *     "desktop"
         * );
         */

    }

    else {

        printf(
            "Incorrect password for %s\n",
            login->username
        );


        gtk_editable_set_text(
            GTK_EDITABLE(
                login->password_entry
            ),
            ""
        );
    }
}


/* =========================================================
 * Password screen
 * =========================================================
 */

static GtkWidget *create_password_screen(
    LoginData *login
)
{
    GtkWidget *overlay;
    GtkWidget *backgroundimg;

    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *password;
    GtkWidget *login_button;
    GtkWidget *back_button;


    /*
     * =====================================================
     * Overlay
     * =====================================================
     */

    overlay = gtk_overlay_new();


    /*
     * =====================================================
     * Background image
     * =====================================================
     */

    backgroundimg =
        gtk_picture_new_for_filename(
            "assets/images/passwordscr.png"
        );

    gtk_widget_set_hexpand(
        backgroundimg,
        TRUE
    );

    gtk_widget_set_vexpand(
        backgroundimg,
        TRUE
    );

    gtk_picture_set_content_fit(
        GTK_PICTURE(backgroundimg),
        GTK_CONTENT_FIT_COVER
    );


    /*
     * Put the image at the bottom.
     */

    gtk_overlay_set_child(
        GTK_OVERLAY(overlay),
        backgroundimg
    );


    /*
     * =====================================================
     * Password UI
     * =====================================================
     */

    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        10
    );

    /*
     * Center the login box over the image.
     */

    gtk_widget_set_halign(
        box,
        GTK_ALIGN_CENTER
    );

    gtk_widget_set_valign(
        box,
        GTK_ALIGN_CENTER
    );


    /*
     * CSS class for the whole password box.
     */

    gtk_widget_add_css_class(
        box,
        "password-box"
    );


    /*
     * =====================================================
     * Title
     * =====================================================
     */




    /*
     * =====================================================
     * Password entry
     * =====================================================
     */

    password =
        gtk_password_entry_new();

    gtk_widget_set_size_request(
        password,
        250,
        -1
    );

    gtk_widget_add_css_class(
        password,
        "password-entry"
    );

    gtk_box_append(
        GTK_BOX(box),
        password
    );


    /*
     * Save the entry so the callbacks can access it.
     */

    login->password_entry = password;


    /*
     * =====================================================
     * Login button
     * =====================================================
     */

    login_button =
        gtk_button_new_with_label(
            "Login"
        );

    gtk_widget_add_css_class(
        login_button,
        "password-login-button"
    );

    gtk_box_append(
        GTK_BOX(box),
        login_button
    );


    /*
     * =====================================================
     * Back button
     * =====================================================
     */

    back_button =
        gtk_button_new_with_label(
            "Back"
        );

    gtk_widget_add_css_class(
        back_button,
        "password-back-button"
    );

    gtk_box_append(
        GTK_BOX(box),
        back_button
    );


    /*
     * =====================================================
     * Signals
     * =====================================================
     */

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
     * Show password peek icon.
     */

    gtk_password_entry_set_show_peek_icon(
        GTK_PASSWORD_ENTRY(password),
        TRUE
    );


    /*
     * =====================================================
     * Put the UI over the background.
     * =====================================================
     */

    gtk_overlay_add_overlay(
        GTK_OVERLAY(overlay),
        box
    );


    return overlay;
}

/* =========================================================
 * User selection callbacks
 * =========================================================
 */

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
        gtk_list_box_row_get_child(
            row
        );


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
        GTK_EDITABLE(
            login->password_entry
        ),
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
        gtk_list_box_row_get_child(
            row
        );


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
 * =========================================================
 */

static GtkWidget *create_user_screen(
    LoginData *login
)
{
    GtkWidget *overlay;
    GtkWidget *background;
    GtkWidget *box;
    GtkWidget *title;
    GtkWidget *list;


    /*
     * Overlay lets the login UI sit
     * on top of the background image.
     */
    overlay =
        gtk_overlay_new();


    /*
     * Background image.
     */
    background =
        gtk_picture_new_for_filename(
            "assets/images/loginscr.png"
        );


    gtk_widget_set_hexpand(
        background,
        TRUE
    );

    gtk_widget_set_vexpand(
        background,
        TRUE
    );


    gtk_picture_set_content_fit(
        GTK_PICTURE(background),
        GTK_CONTENT_FIT_COVER
    );


    gtk_overlay_set_child(
        GTK_OVERLAY(overlay),
        background
    );


    /*
     * Login UI.
     */
    box =
        gtk_box_new(
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




    /*
     * User list.
     */
    list =
        gtk_list_box_new();


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


    /*
     * Give the list a CSS class.
     *
     * users.css can target:
     *
     * .user-list
     */
    gtk_widget_add_css_class(
        list,
        "user-list"
    );


    gtk_box_append(
        GTK_BOX(box),
        list
    );


    /*
     * Put UI on top of background.
     */
    gtk_overlay_add_overlay(
        GTK_OVERLAY(overlay),
        box
    );


    /*
     * Get users from /etc/passwd.
     */
    struct passwd *user;


    setpwent();


    while ((user = getpwent()) != NULL) {

        if (user->pw_uid < 1000)
            continue;


        if (user->pw_dir == NULL ||
            user->pw_dir[0] == '\0')
            continue;


        GtkWidget *row;
        GtkWidget *label;


        row =
            gtk_list_box_row_new();


        label =
            gtk_label_new(
                user->pw_name
            );


        gtk_widget_set_halign(
            label,
            GTK_ALIGN_START
        );


        /*
         * CSS class for individual user rows.
         */
        gtk_widget_add_css_class(
            row,
            "user-row"
        );


        gtk_widget_add_css_class(
            label,
            "user-label"
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
     * Selection changed.
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
     * Select first user.
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


    return overlay;
}


/* =========================================================
 * Main login creation
 * =========================================================
 */

void login_create(
    screen_t *screen
)
{
    if (screen == NULL)
        return;


    LoginData *login =
        calloc(
            1,
            sizeof(LoginData)
        );


    if (login == NULL)
        return;


    login->screen =
        screen;


    /*
     * Create user screen.
     */
    login->user_screen =
        create_user_screen(
            login
        );


    /*
     * Create password screen.
     */
    login->password_screen =
        create_password_screen(
            login
        );


    /*
     * Add both to the same GtkStack.
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
     * Start on users.
     *
     * This also loads:
     *
     * assets/css/users.css
     */
    switchscreen(
        screen,
        "users"
    );
}