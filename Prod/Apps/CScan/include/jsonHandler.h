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

    Data data;
    JsonType jsonType;
    struct JsonElement **arrChildElement;
    struct JsonElement *parentElement;

}JsonElement;





short fileFileIndex(FILE *srcFile, File ***fileIndex);

short controlCharInString(char *jsonString,unsigned long cursor);

JsonElement *initJsonElement(unsigned long sizeArrChildElement, unsigned long elementPosition, char *name, void *data, JsonType dataType, JsonElement *parentElement);
Data defineJsonData(void *inputData, JsonType dataType);
double *dblToPtr(double dblVar);
long *lngToPtr(long lngVar);


JsonElement *readJsonFile(FILE *jsonFile);
JsonElement *readJsonString(char *jsonString,unsigned long *cursor,unsigned long elementPosition, JsonElement *parentElement);
char *readName(char *jsonString, unsigned long *cursor);

void *readData(char *jsonString, unsigned long *cursor,JsonType *dataType);

double *readDoubleType(char *jsonString,unsigned long cursor);
long *readLongType(char *jsonString,unsigned long cursor);
char *readStringType(char *jsonString, unsigned long cursor);
short *readBolType(char *jsonString,unsigned long cursor);

unsigned long countChildElements(char *jsonString,unsigned long cursor,JsonType dataType);


short writeJsonData(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);
short writeJsonName(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);
short writeJsonElement(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);
short writeAgreggatedValue(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize);

short freeJsonElement(JsonElement *jsonElement);

#endif