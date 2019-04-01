#ifndef JSON_HANDLER
#define JSON_HANDLER

#include <wchar.h>
#include <wctype.h>

typedef enum JsonType{_LONG_,_DOUBLE_,_STRING_,_OBJECT_,__BOOLEAN__,_ARRAY_,_NULL_} JsonType;

typedef union Data{

    char *strData;
    short bolData;
    long lngData;
    double dblData;
    void *nulData;

}Data;


typedef struct JsonElement{

    unsigned long sizeArrChildElement;
    unsigned long elementPosition;
    char *name;
    Data *data;
    JsonType jsonType;
    struct JsonElement **arrChildElement;
    struct JsonElement *parentElement;

}JsonElement;


/*
 * ─── FONCTIONS DE LECTURE ───────────────────────────────────────────────────────
 */

JsonElement *searchJsonElement(JsonElement *jsonElement, char *searchedName,Data *searchedValue, JsonType searchedType, unsigned int *elementPosition);


int countDeepness(JsonElement *parentElement,int deepness);


//Initialise un élément JSON avec les valeurs passées en paramètre
JsonElement *initJsonElement(unsigned long sizeArrChildElement, unsigned long elementPosition, char *name, Data  *data, JsonType dataType, JsonElement *parentElement);


//Libère en mémoire un élément JSON
short freeJsonElement(JsonElement **jsonElement);

#endif