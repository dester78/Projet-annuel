#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <glib-2.0/glib/gi18n.h>
#include <gettext-po.h>

#include <gtkHandler.h>
#include <curlHandler.h>
#include <jsonHandler.h>
#include <bluetoothHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>
#include <convertCharHandler.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <bluetoothapis.h>

#include <gtk/gtk.h>

#define _(string) gettext(string)
#define GETTEXT_CREATE_POT



//
int main(int argc, char**argv){
//

    FileIndex *fileIndex;
    CurlRequest *curlRequest;
    File *curlFile;
    File *gtkFile;
    GtkGUI *gtkGUI;
    GtkWidget *vBox1;
    GtkWidget *addButton;

    JsonElement *gtkElement;
    JsonElement *urlElement;
    unsigned int elementPosition;
    char *tmpChar;



    setlocale (LC_ALL, "");
    bindtextdomain ("CScan", getenv("PWD"));
    textdomain ("CScan");

    fileIndex=initFileIndex("json/fileIndex.json");
    curlFile=selectFile(fileIndex,"curl");
    urlElement=readJsonFile(curlFile->filePointer,10);
    printf("\n");
    printf(_("OK CURL"));
    printf("\n");
    gtkFile=selectFile(fileIndex,"gtk");
    gtkElement=readJsonFile(gtkFile->filePointer,10);

    gtk_init(&argc, &argv);

    gtkGUI=initGtkGUI(gtkElement);


    gtkGUI->products=malloc(sizeof(GtkProduct*)*30);
    for(int i = 0 ; i<30 ; i ++){

        gtkGUI->builder=initGtkBuilder(gtkGUI->productGladeFileName);
        gtkGUI->products[i]=initGtkProduct(gtkGUI->builder);
        gtk_box_pack_start (GTK_BOX(gtkGUI->listContainer), gtkGUI->products[i]->box,FALSE,FALSE,0);
    }




    gtk_widget_show_all (gtkGUI->mainWindow);

    gtk_container_add(GTK_CONTAINER(gtkGUI->mainWindow),gtkGUI->loginForm);
    gtk_widget_set_parent(gtkGUI->loginForm,gtkGUI->mainWindow);
    gtk_window_set_position(GTK_WINDOW(gtkGUI->loginForm),GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_transient_for(GTK_WINDOW(gtkGUI->loginForm),GTK_WINDOW(gtkGUI->mainWindow));
    gtk_widget_show_all(gtkGUI->loginForm);


    gtk_main();

//    curlRequest=jsonToCurl(urlElement,L"test",fileIndex);
//    curlConnect(&curlRequest);
//
//    freeCurlRequest(&curlRequest);
    freeJsonElement(&urlElement);

    return 1;
}
