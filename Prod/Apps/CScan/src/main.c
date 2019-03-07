#include <stdlib.h>
#include <stdio.h>
#include <curl.h>
#include <jsonHandler.h>

int main(int argc, char const *argv[]){

    File **fileIndex=initFileIndex("json/fileIndex.json");

    char *jsonString=malloc(sizeof(char));


    strcpy(jsonString,"\0");
    JsonElement *element=initJsonElement(0,0,"toto",dblToPtr(3.0),_DOUBLE_,NULL);
    
    
    // printf("pointeurJsonString:%p\n",jsonString);
    // element->parentElement=NULL;
    // element->name=NULL;

    // element->jsonType=_OBJECT_;
    // element->sizeArrChildElement=3;
    // element->arrChildElement=malloc(sizeof(JsonElement*)*element->sizeArrChildElement);
    

    // element->arrChildElement[0]=malloc(sizeof(JsonElement));
    // element->arrChildElement[0]->elementPosition=0;
    // element->arrChildElement[0]->sizeArrChildElement=0;
    // element->arrChildElement[0]->name="1level";
    // element->arrChildElement[0]->data.lngData=2;
    // element->arrChildElement[0]->jsonType=_LONG_;
    // element->arrChildElement[0]->parentElement=element;

    // element->arrChildElement[1]=malloc(sizeof(JsonElement));
    // element->arrChildElement[1]->elementPosition=1;
    // element->arrChildElement[1]->sizeArrChildElement=0;
    // element->arrChildElement[1]->name="1level";
    // element->arrChildElement[1]->data.lngData=3;
    // element->arrChildElement[1]->jsonType=_LONG_;
    // element->arrChildElement[1]->parentElement=element;

    // element->arrChildElement[2]=malloc(sizeof(JsonElement));
    // element->arrChildElement[2]->elementPosition=2;
    // element->arrChildElement[2]->name="1level";
    // element->arrChildElement[2]->sizeArrChildElement=1;
    // element->arrChildElement[2]->arrChildElement=malloc(sizeof(JsonElement*)*element->arrChildElement[1]->sizeArrChildElement);
    // element->arrChildElement[2]->jsonType=_ARRAY_;
    // element->arrChildElement[2]->parentElement=element;

    // element->arrChildElement[2]->arrChildElement[0]=malloc(sizeof(JsonElement));
    // element->arrChildElement[2]->arrChildElement[0]->sizeArrChildElement=0;
    // element->arrChildElement[2]->arrChildElement[0]->elementPosition=0;
    // element->arrChildElement[2]->arrChildElement[0]->name="2level";
    // element->arrChildElement[2]->arrChildElement[0]->data.lngData=213;
    // element->arrChildElement[2]->arrChildElement[0]->jsonType=_LONG_;
    // element->arrChildElement[2]->arrChildElement[0]->parentElement=element->arrChildElement[2];

    // printf("element : %p\n",element->arrChildElement[2]->arrChildElement[0]);
    writeJsonElement(element,&jsonString,1000);
    printf("END");
    // printf("%s",jsonString);

    free(jsonString);
    freeJsonElement(element);
    free(element);

    


    
}
