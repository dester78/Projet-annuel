#ifndef GTKHANDLER
#define GTKHANDLER

#include <gtk/gtk.h>
#include "jsonHandler.h"



typedef struct GtkProduct{

    GtkWidget *box;
    GtkWidget *addButton;
    GtkWidget *subButton;
    GtkWidget *productLabel;
    GtkWidget *quantityLabel;
}GtkProduct;


typedef struct GtkGUI{

    GtkBuilder *builder;
    GtkWidget *mainWindow;
    GtkWidget *loginForm;
    GtkWidget *listContainer;
    GtkProduct **products;
    char *productGladeFileName;

    int sizeArrayProducts;
    JsonElement *gladeSrc;

}GtkGUI;

GtkGUI *initGtkGUI(JsonElement *jsonGlade);
GtkProduct *initGtkProduct(GtkBuilder *builder);
GtkBuilder *initGtkBuilder(char *cFileName);
GtkWidget *initGtkWindow(GtkBuilder *builder, char *windowName);




#endif
