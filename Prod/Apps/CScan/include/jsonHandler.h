#ifndef JSON_HANDLER
#define JSON_HANDLER

typedef enum JsonType{INTEGER,REAL,STRING,OBJECT,BOOLEAN,ARRAY} JsonType;


typedef struct JsonData{

    int sizeJsonDataArray;
    void *jsonName;
    void *jsonValue;
    JsonType jsonNameType;
    JsonType jsonValueType;

}JsonData;

typedef struct JsonName{
    
}

short fileFileIndex(FILE *srcFile, File ***fileIndex);


#endif