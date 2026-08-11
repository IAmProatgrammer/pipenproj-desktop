#include "desktop.h"
#include "disstack.h"

#include <gtk/gtk.h>
#include <stdio.h>

static void programs_clicked(
GtkButton *button,
gpointer data
)
{
(void)button;
(void)data;

 
printf("Programs / Payloads clicked\n");
 

}

static void terminal_clicked(
GtkButton *button,
gpointer data
)
{
(void)button;
(void)data;

 
printf("Terminal clicked\n");
 

}

static void settings_clicked(
GtkButton *button,
gpointer data
)
{
(void)button;
(void)data;

 
printf("Settings clicked\n");
 

}

static void extra_clicked(
GtkButton *button,
gpointer data
)
{
(void)button;
(void)data;


printf("Extra clicked\n");


}

void desktop_create(
screen_t *screen
)
{
if (screen == NULL)
return;


GtkWidget *box =
    gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        10
    );

gtk_widget_set_halign(
    box,
    GTK_ALIGN_CENTER
);

gtk_widget_set_valign(
    box,
    GTK_ALIGN_CENTER
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

GtkWidget *programs =
    gtk_button_new_with_label(
        "Programs / Payloads"
    );

gtk_widget_set_size_request(
    programs,
    250,
    50
);

gtk_box_append(
    GTK_BOX(box),
    programs
);

GtkWidget *terminal =
    gtk_button_new_with_label(
        "Terminal"
    );

gtk_widget_set_size_request(
    terminal,
    250,
    50
);

gtk_box_append(
    GTK_BOX(box),
    terminal
);

GtkWidget *settings =
    gtk_button_new_with_label(
        "Settings"
    );

gtk_widget_set_size_request(
    settings,
    250,
    50
);

gtk_box_append(
    GTK_BOX(box),
    settings
);

GtkWidget *extra =
    gtk_button_new_with_label(
        "Extra"
    );

gtk_widget_set_size_request(
    extra,
    250,
    50
);

gtk_box_append(
    GTK_BOX(box),
    extra
);

g_signal_connect(
    programs,
    "clicked",
    G_CALLBACK(programs_clicked),
    screen
);

g_signal_connect(
    terminal,
    "clicked",
    G_CALLBACK(terminal_clicked),
    screen
);

g_signal_connect(
    settings,
    "clicked",
    G_CALLBACK(settings_clicked),
    screen
);

g_signal_connect(
    extra,
    "clicked",
    G_CALLBACK(extra_clicked),
    screen
);

addscreen(
    screen,
    box,
    "desktop"
);


}
