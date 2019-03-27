#include <gtk/gtk.h>
#include<gtkCallbacks.h>
#include <gtkHandler.h>


void cb_on_addButton_clicked(GtkWidget *widget, gpointer *data){

    GtkProduct *gtkProduct=(GtkProduct*)data;

    g_print("HelloWorld");
}

void cb_on_subButton_clicked(GtkWidget *widget, gpointer *data){

    GtkProduct *gtkProduct=(GtkProduct*)data;

    g_print("HelloWorld");
}
