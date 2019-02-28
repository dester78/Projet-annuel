#ifndef CURL_HANDLER
#define CURL_HANDLER

    typedef struct Parameter{

        char *name;
        char *value;
    }Parameter;

    typedef struct Url{

        char *protocol;
        char *domain;
        char **path;
        Parameter **parameters;
    }Url;

    

#endif