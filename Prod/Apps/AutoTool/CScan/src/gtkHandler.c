#include <stdlib.h>

#include <gtk/gtk.h>
#include <gtkHandler.h>
#include <gtkCallbacks.h>
#include <jsonHandler.h>
#include <convertCharHandler.h>


GtkGUI *initGtkGUI(JsonElement *jsonGlade){

    GtkGUI *gtkGUI;
    unsigned int elementPosition;

    gtkGUI=malloc(sizeof(GtkGUI));
    gtkGUI->products=malloc(sizeof(GtkProduct*));
    gtkGUI->products=NULL;
    gtkGUI->sizeArrayProducts=0;
    char *tmpChar;

    if((jsonGlade=searchJsonElement(jsonGlade,L"mainWindow",NULL,_STRING_,&elementPosition))!=NULL) {
        wCharToChar(&jsonGlade->arrChildElement[elementPosition]->data->strData,&tmpChar,_CONSERV_,_CONSERV_);
        gtkGUI->builder=initGtkBuilder(tmpChar);
        gtkGUI->mainWindow=initGtkWindow(gtkGUI->builder,"mainWindow");
        gtkGUI->listContainer=GTK_WIDGET(gtk_builder_get_object(gtkGUI->builder,"productListContainer"));
    }
    if((jsonGlade=searchJsonElement(jsonGlade,L"listElement",NULL,_STRING_,&elementPosition))!=NULL) {
        wCharToChar(&jsonGlade->arrChildElement[elementPosition]->data->strData,&gtkGUI->productGladeFileName,_CONSERV_,_CONSERV_);
        gtkGUI->builder=initGtkBuilder(gtkGUI->productGladeFileName);

    }
    if((jsonGlade=searchJsonElement(jsonGlade,L"loginForm",NULL,_STRING_,&elementPosition))!=NULL) {
        wCharToChar(&jsonGlade->arrChildElement[elementPosition]->data->strData,&tmpChar,_CONSERV_,_FREE_);
        gtkGUI->builder=initGtkBuilder(tmpChar);
        gtkGUI->loginForm=GTK_WIDGET(gtk_builder_get_object(gtkGUI->builder,"loginWindow"));
    }

    if((jsonGlade=searchJsonElement(jsonGlade,L"styleSheet",NULL,_STRING_,&elementPosition))!=NULL) {
        wCharToChar(&jsonGlade->arrChildElement[elementPosition]->data->strData,&tmpChar,_CONSERV_,_FREE_);
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(cssProvider), tmpChar, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER);
    }


    return gtkGUI;

}


GtkBuilder *initGtkBuilder(char *cFileName){

    GtkBuilder *builder = NULL;
    GError *error = NULL;

    gchar *gFileName;

    //Ouverture du fichier .glade
    builder = gtk_builder_new();

    gFileName = g_build_filename (cFileName, NULL);
    //Chargement du fichier .glade
    gtk_builder_add_from_file (builder, gFileName, &error);
    g_free (gFileName);

    return builder;
}

GtkWidget *initGtkWindow(GtkBuilder *builder, char *windowName){

    GtkWidget *mainWindow;


    /* Récupération du pointeur de la fenêtre principale */
    mainWindow = GTK_WIDGET(gtk_builder_get_object (builder, windowName));
//    gtk_widget_get_toplevel(mainWindow);

    /* Affectation du signal "destroy" à la fonction gtk_main_quit(); pour la */
    /* fermeture de la fenêtre. */
    g_signal_connect (G_OBJECT (mainWindow), "destroy", (GCallback)gtk_main_quit, NULL);

    /* Affichage de la fenêtre principale. */


    return mainWindow;
}

GtkProduct *initGtkProduct(GtkBuilder *builder){

    GtkProduct *gtkProduct;

    gtkProduct=malloc(sizeof(GtkProduct));


    gtkProduct->box=GTK_WIDGET(gtk_builder_get_object (builder, "productContainer"));
    gtk_widget_set_name(gtkProduct->box,"productContainer");

    gtkProduct->addButton=GTK_WIDGET(gtk_builder_get_object (builder, "addButton"));
    gtk_widget_set_name(gtkProduct->addButton,"addButton");

    gtkProduct->subButton=GTK_WIDGET(gtk_builder_get_object (builder, "subButton"));
    gtk_widget_set_name(gtkProduct->subButton,"subButton");

    gtkProduct->productLabel=GTK_WIDGET(gtk_builder_get_object (builder, "productLabel"));
    gtk_widget_set_name(gtkProduct->productLabel,"productLabel");

    gtkProduct->quantityLabel=GTK_WIDGET(gtk_builder_get_object (builder, "quantityLabel"));
    gtk_widget_set_name(gtkProduct->quantityLabel,"quantityLabel");

    g_signal_connect (G_OBJECT (gtkProduct->addButton),"clicked",G_CALLBACK (cb_on_addButton_clicked),gtkProduct);
    g_signal_connect (G_OBJECT (gtkProduct->subButton),"clicked",G_CALLBACK (cb_on_subButton_clicked),gtkProduct);


    return gtkProduct;
}

