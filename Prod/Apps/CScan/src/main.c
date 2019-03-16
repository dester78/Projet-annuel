#include <stdlib.h>
#include <stdio.h>
#include <curlHandler.h>
#include <locale.h>
#include <jsonHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>

#include <gtk/gtk.h>

//static void activate(GtkApplication *app,
//         gpointer user_data) {
//    GtkWidget *window;
//    window = gtk_application_window_new(app);
//    gtk_window_set_title(GTK_WINDOW(window), "Hello GNOME");
//    gtk_widget_show_all(window);
//}

int main(int argc, char**argv){
//
//    GtkWidget *fenetre_principale = NULL;
//    GtkBuilder *builder = NULL;
//    GError *error = NULL;
//    gchar *filename = NULL;
//    /* Initialisation de la librairie Gtk. */
//    gtk_init(&argc, &argv);
//
//    /* Ouverture du fichier Glade de la fenêtre principale */
//    builder = gtk_builder_new();
//
//    /* Création du chemin complet pour accéder au fichier test.glade. */
//    /* g_build_filename(); construit le chemin complet en fonction du système */
//    /* d'exploitation. ( / pour Linux et \ pour Windows) */
//    filename =  g_build_filename ("glade/window_main.glade", NULL);
//
//    /* Chargement du fichier test.glade. */
//    gtk_builder_add_from_file (builder, filename, &error);
//    g_free (filename);
//    if (error)
//    {
//        gint code = error->code;
//        g_printerr("%s\n", error->message);
//        g_error_free (error);
//        return code;
//    }
//
//    /* Récupération du pointeur de la fenêtre principale */
//    fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));
//
//    /* Affectation du signal "destroy" à la fonction gtk_main_quit(); pour la */
//    /* fermeture de la fenêtre. */
//    g_signal_connect (G_OBJECT (fenetre_principale), "destroy", (GCallback)gtk_main_quit, NULL);
//
//    /* Affichage de la fenêtre principale. */
//    gtk_widget_show_all (fenetre_principale);
//
//    gtk_main();

    FileIndex *fileIndex;
    CurlRequest *curlRequest;
    wchar_t *string;
    int null;

    string=malloc(sizeof(wchar_t));
    string[0]='\0';
    File *testFile;
    JsonElement *urlElement;
    JsonElement *testCurl;
    printf("TEST");

    fileIndex=initFileIndex("json/fileIndex.json");
    testFile=selectFile(fileIndex,"curl");
    urlElement=readJsonFile(testFile->filePointer,10);
    printf("OK SELECT CURL");

    printf("APRES OPEN");
    curlRequest=jsonToCurl(urlElement,L"test",fileIndex);
    curlConnect(&curlRequest);

    scanf("%d",&null);
    printf("END MAIN");
    return 1;
}

//
//void on_window_main_destroy()
//{
//    gtk_main_quit();
//}