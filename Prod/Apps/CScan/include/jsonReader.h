#ifndef JSONREADER
#define JSONREADER

#include <wchar.h>

#include <jsonHandler.h>
#include <fileHandler.h>



//Lit un fichier au format JSON encodé en ISO-8859-1 et retourne un élément JSON contenant les données JSON récoltées dans ce fichier
JsonElement *readJsonFile(FILE *jsonFile,int deepness);

//Lit une chaine JSON composée de wide char et retourne un élement JSON contenant les données récoltées dans la chaine
JsonElement *readJsonString(wchar_t *jsonString,unsigned long *cursor,unsigned long elementPosition, JsonElement *parentElement, int deepness);

//Lit une chaine de caractère et retourne le nom d'une propriété d'objet
wchar_t *readName(wchar_t *jsonString, unsigned long *cursor);

//Lit une chaine de caractère et stock la valeur d'un élément JSON dans *data ainsi que son type dans *dataType
short readData(wchar_t *jsonString, unsigned long *cursor,JsonType *dataType,Data *data);

//Lit une chaine de caractère pour y stocker un nombre réel dans *data
short readDoubleType(wchar_t *jsonString,unsigned long cursor,Data *data);

//Lit une chaine de caractère pour y stocker un nombre entier dans *data
short readLongType(wchar_t *jsonString,unsigned long cursor, Data *data);

//Lit une chaine de caractère pour y stocker la chaine de caractère "null" dans *data
short readNullType(wchar_t *jsonString,unsigned long cursor,Data *data);

//Lit une chaine de caractère retourne une chaine de caractère si la fonction trouve une chaine de caractère délimitée par des guillemets
wchar_t *readStringType(wchar_t *jsonString, unsigned long cursor);

//Lit une chaine de caractère stock dans *data un 0 si la chaine false est trouvée, stock 1 si la chaine true est trouvée
short readBolType(wchar_t *jsonString,unsigned long cursor, Data *data);

//Vérifie si un caractère situé à l'emplacement du curseur passé en paramètre se trouve dans une chaine de caractère délimitée par des guillemets
short controlCharInString(wchar_t *jsonString,unsigned long cursor);

//Supprime les caractères " " et "\n" pouvant comprommettre la lecture du chaine de caractère JSON
short formatJsonString(wchar_t **jsonString);


//Compte le nombre d'élément présents dans un tableau ou objet, cette fonction se base sur les "," pour déterminer le nombre d'éléments
unsigned long countChildElements(wchar_t *jsonString,unsigned long cursor,JsonType dataType,unsigned long *cursorEndArray);

#endif
