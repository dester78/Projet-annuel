#ifndef CURL_HANDLER
#define CURL_HANDLER

#include <curl/curl.h>
#include <wchar.h>
#include <fileHandler.h>
#include <jsonHandler.h>

typedef enum ResponseType{_JSON_,_FILE_}ResponseType;

typedef struct Parameter{

    char *name;
    char *value;
}Parameter;

typedef struct CurlResponse {
    size_t size;
    char* string;
    JsonElement *json;
    File *file;
}CurlResponse;


typedef struct CurlRequest{

    char *url;
    char *body;
    char *method;
    int deepness;
    char **directSearchedValues;
    void *writeMethod;
    int sizeArrSearchedValues;
    CurlResponse *response;
    ResponseType responseType;
}CurlRequest;

size_t curlToString(void *ptr, size_t size, size_t nmemb, CurlResponse *curlResponse);

size_t curlToFile(void *ptr, size_t size, size_t nmemb, FILE *curlResponse);

short curlGetJson(CurlRequest **curlRequest);

short curlSetOpt(CurlRequest **curlRequest, CURL *curl, struct curl_slist *header);
CurlRequest *jsonToCurl(JsonElement *jsonElement,char *urlName,FileIndex *fileIndex);
char *writeUrl(JsonElement *jsonElement,  JsonElement *urlNeededValues);
short catTokenWithUrl(JsonElement *urlToken, JsonElement *urlElement,char **url);

short curlConnect(CurlRequest **curlRequest);
short freeCurlResponse(CurlResponse **curlResponse);
short freeCurlRequest(CurlRequest **curlRequest);



#endif