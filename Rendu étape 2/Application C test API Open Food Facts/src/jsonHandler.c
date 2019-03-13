#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <system.h>
#include <wchar.h>
#include <locale.h>


#include <fileHandler.h>
#include <jsonHandler.h>





JsonElement *initJsonElement(unsigned long sizeArrChildElement, unsigned long elementPosition, wchar_t *name, Data *data, JsonType dataType, JsonElement *parentElement){

    JsonElement *jsonElement;
    jsonElement=malloc(sizeof(JsonElement));
    jsonElement->name=name;
    jsonElement->sizeArrChildElement=sizeArrChildElement;
    jsonElement->elementPosition=elementPosition;
    jsonElement->jsonType=dataType;
    jsonElement->parentElement=parentElement;
    jsonElement->data=data;
    if(jsonElement->sizeArrChildElement>0){
        jsonElement->arrChildElement=malloc(sizeof(JsonElement*)*jsonElement->sizeArrChildElement);
    }
    else{
        jsonElement->arrChildElement=NULL;
    }
    return jsonElement;
}



JsonElement *searchJsonElement(JsonElement *jsonElement, wchar_t *searchedName,Data *searchedValue, JsonType searchedType, unsigned int *elementPosition){
    short bol=0;

        if(jsonElement->name!=NULL&&jsonElement->jsonType==searchedType&&wcscmp(jsonElement->name,searchedName)==0){
            if(searchedValue!=NULL) {
                switch (jsonElement->jsonType) {
                    case _DOUBLE_:
                        bol = jsonElement->data->dblData == searchedValue->dblData ? (short) 1 : (short) 0;
                        break;
                    case _BOOLEAN_:
                        bol = jsonElement->data->bolData == searchedValue->bolData ? (short) 1 : (short) 0;
                        break;
                    case _LONG_:
                        bol = jsonElement->data->lngData == searchedValue->lngData ? (short) 1 : (short) 0;
                        break;
                    default:
                        if (jsonElement->jsonType == _STRING_ || jsonElement->jsonType == _NULL_) {
                            bol = wcscmp(jsonElement->data->strData, searchedValue->strData) == 0 ? (short) 1
                                                                                                  : (short) 0;
                        }
                        break;
                }
                if (jsonElement->jsonType == _ARRAY_ || jsonElement->jsonType == _OBJECT_) {
                    *elementPosition = jsonElement->elementPosition;
                    return jsonElement;
                }
            }
            else{bol=1;}
        }
        if(bol){
            *elementPosition=jsonElement->elementPosition;
            return jsonElement->parentElement;
        }
        if(jsonElement->jsonType==_ARRAY_||jsonElement->jsonType==_OBJECT_){
            for(unsigned int i =0;i<(unsigned int)jsonElement->sizeArrChildElement;i++){

                JsonElement *resultElement;
                if((resultElement=searchJsonElement(jsonElement->arrChildElement[i],searchedName,searchedValue,searchedType,elementPosition))!=NULL){
                    return resultElement;
                }
            }
        }
        return NULL;
}



int countDeepness(JsonElement *parentElement,int deepness){

    if(parentElement->parentElement!=NULL){
        deepness=countDeepness(parentElement->parentElement,deepness);
    }
    return ++deepness;
}



short freeJsonElement(JsonElement *jsonElement){

    if(jsonElement->sizeArrChildElement>0){
        for(unsigned long i=0;i<jsonElement->sizeArrChildElement;i++){
            freeJsonElement(jsonElement->arrChildElement[i]);
            free(jsonElement->arrChildElement[i]);
        }
    }
    if(jsonElement->jsonType==_STRING_){
        free(jsonElement->data->strData);
    }
    free(jsonElement->data);
    return 1;
}


