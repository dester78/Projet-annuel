#ifndef CURL_HANDLER
#define CURL_HANDLER

#include <wchar.h>
#include <fileHandler.h>
#include <jsonHandler.h>

typedef struct Parameter{

    char *name;
    char *value;
}Parameter;

typedef struct CurlResponse {
    size_t size;
    char* data;
}CurlResponse;


typedef struct CurlRequest{

    char *url;
    char *body;
    char *method;
    int deepness;
    char **directSearchedValues;
    int sizeArrSearchedValues;

}CurlRequest;

size_t curlToString(void *ptr, size_t size, size_t nmemb, CurlResponse *curlResponse);

CurlRequest *jsonToCurl(JsonElement *jsonElement,wchar_t *urlName);
char *writeUrl(JsonElement *jsonElement,  JsonElement *urlNeededValues);
short catTokenWithUrl(JsonElement *urlToken, JsonElement *urlElement,wchar_t **url);

short curlConnect(CurlRequest *curlRequest,JsonElement **jsonElement);


#endif