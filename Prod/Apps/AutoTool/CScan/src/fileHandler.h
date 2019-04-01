#ifndef FILE_HANDLER
#define FILE_HANDLER

#include <stdio.h>
#include <wchar.h>

typedef enum OpenTime{_NOW_,_LATER_}OpenTime;

typedef struct File{
    char *name;
    char *path;
    char *openMode;
    FILE *filePointer;
}File;

typedef struct FileIndex{

    File **arrayFile;
    unsigned int sizeArrayFile;
}FileIndex;

File *initFileElement(char *fileName, char *path, char *openMode,char *flux,OpenTime openTime);
FileIndex *initFileIndex(char *fileIndexPath);

File *selectFile(FileIndex *fileIndex, char *searchedName);
short freeFileElement(File *fileElement);


/*
 * ─── FONCTIONS D'OUVERTURE DE FICHIERS ───────────────────────────────────────────
 */

//Fonction permettant d'ouvrir un fichier, en fonction de son nom complet, et son mode d'ouverture, 
//si le mode d'ouverture n'est pas specifie alors il sera ouvert en w+ par defaut.
FILE *openFile(char *fileName, char *openMode);

//Fonction permettant d'ouvrir un fichier, en fonction de son nom complet, et son mode d'ouverture, 
//si le mode d'ouverture n'est pas specifie alors il sera ouvert en w+ par defaut. 
//Elle a pour rôle supplementaire de redirige un flux vers le fichier ouvert, par defaut stderr.
FILE *openFluxFile(char *path, char *openMode, FILE * flux);



/*
 * ─── FONCTIONS DE LECTURE DE FICHIERS ───────────────────────────────────────────
 */
unsigned long countFileChar(FILE *filePtr);
char *getFileContent(FILE *filePtr);

size_t wcfread(char *string,size_t size, FILE *file);
//Fonction remplissant un tableau de pointeur de tableau de chaine de caractère, la variable lastRow est modifié pour pouvoir parcourir ce tableau en dehors de la fonction
short returnConfigFileParameters(FILE *configFile, int *lastRow,char ***arrayParameters );

//Fonction renvoyant un tableau contenant les informations de tailles d'un fichier (nb lignes / nb caractères), le pointeur FILE* passe en paramètre permet le parcours de ce fichier, 
//le second paramètre est un pointeur permettant de recuperer la longueur du tableau. La valeur de retour est le nombre de paramètres presents dans le fichier.
int countFileRowChar(int **arrayRowChar,FILE *file, int *lastRow);

/*
 * ─── FONCTIONS D'ECRITURE DE FICHIERS ────────────────────────────────────────────
 */

//Fonction creant un message d'erreur comprenant, le fichier, la ligne, la date et l'heure de l'erreur, 
//ce message est ensuite envoye dans le flux d'erreur avec la fonction perror
void createErrorReport(char *errorMessage,char * fileError, int lineError, char  *dateError, char *timeError);


#endif