#include <stdio.h>
#include <curl/curl.h>
#include <wchar.h>
#include <locale.h>

#include <curlHandler.h>
#include <jsonHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>
#include <fileHandler.h>
#include <convertCharHandler.h>



CurlRequest *initCurlRequest(){

    CurlRequest *curlRequest;

    curlRequest=malloc(sizeof(CurlRequest));
    curlRequest->method=NULL;
    curlRequest->body=NULL;
    curlRequest->url=NULL;
    curlRequest->deepness=0;
    curlRequest->directSearchedValues=NULL;
    curlRequest->sizeArrSearchedValues=0;

    curlRequest->response=malloc(sizeof(CurlResponse));
    curlRequest->response->size=0;
    curlRequest->response->string=malloc(sizeof(char));
    curlRequest->response->string[0]='\0';
    curlRequest->response->file=NULL;

    return curlRequest;

}

short freeCurlRequest(CurlRequest **curlRequest){


    if((*curlRequest)->method!=NULL){
        free((*curlRequest)->method);
        (*curlRequest)->method=NULL;
    }
    if((*curlRequest)->body!=NULL){
        free((*curlRequest)->body);
        (*curlRequest)->body=NULL;
    }
    if((*curlRequest)->url!=NULL){
        free((*curlRequest)->url);
        (*curlRequest)->url=NULL;
    }
    if((*curlRequest)->response!=NULL){
        freeCurlResponse(&(*curlRequest)->response);
    }
    free(*curlRequest);
    (*curlRequest)=NULL;

    return 1;
}

short freeCurlResponse(CurlResponse **curlResponse){


    if((*curlResponse)->string!=NULL){
        free((*curlResponse)->string);
        (*curlResponse)->string=NULL;
    }
    if((*curlResponse)->file!=NULL){
        freeFileElement((*curlResponse)->file);
        (*curlResponse)->file=NULL;
    }
    if((*curlResponse)->json!=NULL){
        freeJsonElement(&(*curlResponse)->json);
    }

    free(*curlResponse);
    *curlResponse=NULL;
    return 1;
}





CurlRequest *jsonToCurl(JsonElement *jsonElement,wchar_t *urlName,FileIndex *fileIndex){

    CurlRequest *curl;
    Data data;
    JsonElement *jsonCurlElement;
    JsonElement *jsonParameterElement;
    JsonElement *tmpJsonElement;
    unsigned int elementPosition;
    unsigned int parameterPosition;
    wchar_t *tmpWchar;
    char *tmpChar;

    curl=initCurlRequest();

    printf("WAIT");
    data.strData=urlName;

    if((jsonCurlElement=searchJsonElement(jsonElement,L"curl",NULL,_ARRAY_,&elementPosition))!=NULL){
        if((jsonCurlElement=searchJsonElement(jsonCurlElement,L"name",&data,_STRING_,&elementPosition))!=NULL){

            if((jsonParameterElement=searchJsonElement(jsonElement,L"urlNeededValues",NULL,_ARRAY_,&parameterPosition))){
                curl->url=writeUrl(jsonCurlElement,jsonParameterElement->arrChildElement[parameterPosition]);
                printf("URL : %s\n",curl->url);
            }
            if((tmpJsonElement=searchJsonElement(jsonElement,L"file",NULL,_STRING_,&parameterPosition))!=NULL){
                wCharToChar(&tmpJsonElement->arrChildElement[elementPosition]->data->strData,&tmpChar,_CONSERV_,_CONSERV_);
                curl->response->file=selectFile(fileIndex,tmpChar);
                curl->responseType=_FILE_;
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"method",NULL,_STRING_,&elementPosition))!=NULL){
                wCharToChar(&tmpJsonElement->arrChildElement[elementPosition]->data->strData,&curl->method,_CONSERV_,_CONSERV_);
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"deepness",NULL,_LONG_,&elementPosition))!=NULL){
                curl->deepness=(int)tmpJsonElement->arrChildElement[elementPosition]->data->lngData;
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"json",NULL,_LONG_,&elementPosition))!=NULL){
                curl->responseType=_JSON_;
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"directSearchedValues",NULL,_ARRAY_,&elementPosition))!=NULL&&tmpJsonElement->arrChildElement[elementPosition]->sizeArrChildElement!=0){
                curl->sizeArrSearchedValues=(int)tmpJsonElement->arrChildElement[elementPosition]->sizeArrChildElement;
                curl->directSearchedValues=malloc(sizeof(char*)*curl->sizeArrSearchedValues);
                for(int i=0;i<curl->sizeArrSearchedValues;i++){
                    wCharToChar(&tmpJsonElement->arrChildElement[elementPosition]->arrChildElement[i]->data->strData,&curl->directSearchedValues[i],_CONSERV_,_CONSERV_);
                }
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"body",NULL,_OBJECT_,&elementPosition))!=NULL){
                tmpWchar=malloc(sizeof(wchar_t));
                tmpWchar[0]='\0';
                writeAgreggatedValue(tmpJsonElement->arrChildElement[elementPosition],&tmpWchar,1000);
                wCharToChar(&tmpWchar,&curl->body,_FREE_,_CONSERV_);
            }
        }
    }

    return curl;
}

char *writeUrl(JsonElement *jsonElement,  JsonElement *urlNeededValues){

    wchar_t *wUrl;
    char *cUrl;
    JsonElement *tmpElement;
    unsigned int elementPosition;

    wUrl=malloc(sizeof(wchar_t));


    for(unsigned long i=0;i<urlNeededValues->sizeArrChildElement;i++){
        if((tmpElement=searchJsonElement(jsonElement,urlNeededValues->arrChildElement[i]->arrChildElement[0]->name,NULL,_ARRAY_,&elementPosition))!=NULL){
            catTokenWithUrl(urlNeededValues->arrChildElement[i]->arrChildElement[0],tmpElement->arrChildElement[elementPosition],&wUrl);
        }
    }
//    wUrl[wcslen(wUrl)]=L'\0';
    wCharToChar(&wUrl,&cUrl,_FREE_,_CONSERV_);
    printf(" C URL : %s|\n",cUrl);
    return cUrl;
}

short catTokenWithUrl(JsonElement *urlToken, JsonElement *urlElement,wchar_t **url){

    for(unsigned long i=0;i<urlElement->sizeArrChildElement;i++){
        if(urlElement->arrChildElement[i]->arrChildElement==NULL){
            if(wcslen(urlElement->arrChildElement[i]->data->strData)>0){
                if(wcslen(urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData)>0){
                    *url=realloc(*url,sizeof(wchar_t)*(wcslen(*url)+wcslen(urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData)+1));
                    wcscat(*url,urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData);
                }
                *url=realloc(*url,sizeof(wchar_t)*(wcslen(*url)+wcslen(urlElement->arrChildElement[i]->data->strData)+1));
                wcscat(*url,urlElement->arrChildElement[i]->data->strData);
                if(wcslen(urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData)>0){
                    *url=realloc(*url,sizeof(wchar_t)*(wcslen(*url)+wcslen(urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData)+1));
                    wcscat(*url,urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData);
                }
            }
        }
        else{
            catTokenWithUrl(urlToken->arrChildElement[0],urlElement->arrChildElement[i],url);
        }
    }
    return 0;
}



short curlConnect(CurlRequest **curlRequest){


    CURL *curl = curl_easy_init();
    struct curl_slist *headers = NULL;

    CURLcode result;

    curlSetOpt(curlRequest, curl, headers);

    if((result = curl_easy_perform(curl)) != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();


    if((*curlRequest)->responseType==_JSON_){
        curlGetJson(curlRequest);
    }

    if((*curlRequest)->responseType==_FILE_){
        fclose((*curlRequest)->response->file->filePointer);
//        system("setup.exe");
    }

    if((result = curl_easy_perform(curl)) != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));

    }


//    wprintf(L"CHAR END : %ls",tmpWChar);

    return 0;
}



size_t curlToString(void *ptr, size_t size, size_t nmemb, CurlResponse *curlResponse){


    size_t newSize=curlResponse->size+size*nmemb;
    curlResponse->string=realloc(curlResponse->string,newSize+1);
    memcpy(curlResponse->string+curlResponse->size,ptr,size*nmemb);
    curlResponse->string[newSize]='\0';
    curlResponse->size=newSize;
    return size*nmemb;

}

size_t curlToFile(void *ptr, size_t size, size_t nmemb, FILE *curlResponse) {
    size_t newSize=fwrite(ptr, size, nmemb, curlResponse);
    return newSize;
}


short curlGetJson(CurlRequest **curlRequest){

    wchar_t *tmpWChar;
    unsigned long cursor=0;


    if((*curlRequest)->directSearchedValues!=NULL){
        for(int i = 0; i < (*curlRequest)->sizeArrSearchedValues ; i++){
            if(strpbrk((*curlRequest)->response->string,(*curlRequest)->directSearchedValues[i])!=NULL){
                printf("OK CURL FIND");
                return 1;
            }
        }
    }


    charToWchar(&(*curlRequest)->response->string,&tmpWChar,_FREE_,_CONSERV_);
    wprintf(L"RESULT !!! :%ls",tmpWChar);
    (*curlRequest)->response->json=readJsonString(tmpWChar,&cursor,0,NULL,(*curlRequest)->deepness);
//    wprintf(L"Result : %ls",tmpWChar);
    free(tmpWChar);
    tmpWChar=malloc(sizeof(wchar_t));
    tmpWChar[0]=L'\0';
    writeJsonElement((*curlRequest)->response->json,&tmpWChar,1000);
    wprintf(L"Result : %s",tmpWChar);

    return 1;

}


short curlSetOpt(CurlRequest **curlRequest, CURL *curl, struct curl_slist *headers){


    //Requête HTTP méthode GET
    if((*curlRequest)->method!=NULL){
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, (*curlRequest)->method);
    }
    //URL de la requête
    curl_easy_setopt(curl, CURLOPT_URL, (*curlRequest)->url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    //Ne vérifie pas l'authenticité du certificat envoyé par le serveur quand la valeur est à 0, vérifie quand la valeur est à 1.
    //J'ai mis la valeur à 0 car *curl stopait la connexion avec l'API openfoodfacts lorsqu'une vérification était effectuée.
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    //Création d'un header vide
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    if((*curlRequest)->responseType==_JSON_){
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (*curlRequest)->response);
    }
    else if((*curlRequest)->responseType==_FILE_){
        (*curlRequest)->response->file->filePointer=openFile((*curlRequest)->response->file->path,(*curlRequest)->response->file->openMode);
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToFile);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (*curlRequest)->response->file->filePointer);
    }

    return 1;
}


