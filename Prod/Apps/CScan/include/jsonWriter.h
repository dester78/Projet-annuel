#ifndef JSONWRITER
#define JSONWRITER

#include <jsonHandler.h>


//Fonctions d'écriture JSON

//Ecrit la valeur d'un élément JSON dans une chaine de caractère en fonction de son type spécifié par l'énumération jsonType
short writeJsonData(JsonElement *jsonElement, wchar_t **jsonString,unsigned long bufferSize);

//Ecrit le nom d'un élément JSON dans une chaine de caractère en y ajoutant le symbole ":"
short writeJsonName(JsonElement *jsonElement, wchar_t **jsonString,unsigned long bufferSize);

//Ecrit un élément JSON dans une chaine de caractère, si l'élément fait partie d'un tableau ou d'un ensemble de propriété d'objet et qu'il ne se trouve pas en dernière position une virgule est ajouté
short writeJsonElement(JsonElement *jsonElement, wchar_t **jsonString,unsigned long bufferSize);

//Ecrit un élément JSON de type objet ou tableau dans une chaine de caractère rappelle la fonction writeJsonElement pour chaque élément le composant
short writeAgreggatedValue(JsonElement *jsonElement, wchar_t **jsonString,unsigned long bufferSize);




#endif
