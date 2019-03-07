#ifndef JSON_HANDLER
#define JSON_HANDLER
#include <fileHandler.h>

typedef enum JsonType{_LONG_,_DOUBLE_,_STRING_,_OBJECT_,_BOOLEAN_,_ARRAY_} JsonType;

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

JsonElement *readJsonFile(FILE *jsonFile);
JsonElement *readJsonString(char *jsonString,unsigned long *cursor,unsigned long elementPosition, JsonElement *parentElement);

char *readName(char *jsonString, unsigned long *cursor);
short readData(char *jsonString, unsigned long *cursor,JsonType *dataType,Data *data);
short readDoubleType(char *jsonString,unsigned long cursor,Data *data);
short readLongType(char *jsonString,unsigned long cursor, Data *data);
char *readStringType(char *jsonString, unsigned long cursor);
short readBolType(char *jsonString,unsigned long cursor, Data *data);



short fileFileIndex(FILE *srcFile, File ***fileIndex);

short controlCharInString(char *jsonString,unsigned long cursor);

JsonElement *initJsonElement(unsigned long sizeArrChildElement, unsigned long elementPosition, char *name, Data  *data, JsonType dataType, JsonElement *parentElement);
Data defineJsonData(void *inputData, JsonType dataType);
double *dblToPtr(double dblVar);
long *lngToPtr(long lngVar);

//Supprime les caractères pouvant comprommettre la lecture du chaine de caractère JSON 
short formatJsonString(char **jsonString);



unsigned long countChildElements(char *jsonString,unsigned long cursor,JsonType dataType);


short writeJsonData(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);
short writeJsonName(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);
short writeJsonElement(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);
short writeAgreggatedValue(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);

short freeJsonElement(JsonElement *jsonElement);

#endif