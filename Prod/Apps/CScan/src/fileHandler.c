#define ERROR_BUFFER_SIZE 250
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>
#include <locale.h>

#include <wCharHandler.h>
#include <fileHandler.h>
#include <jsonHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>

FileIndex *initFileIndex(char *fileIndexPath){

    FileIndex *fileIndex;
    FILE *srcFile;
    JsonElement *jsonElement;
    wchar_t *jsonString=malloc(sizeof(wchar_t));
    setlocale(LC_ALL,"");

    wcscpy(jsonString,L"\0");
    fileIndex=malloc(sizeof(FileIndex));

    if((srcFile=openFile(fileIndexPath,"r"))!=NULL){

        jsonElement=readJsonFile(srcFile,6);
        writeJsonElement(jsonElement,&jsonString,3000);
        fileIndex->sizeArrayFile=jsonElement->arrChildElement[0]->sizeArrChildElement;
        fileIndex->arrayFile=malloc(sizeof(File*)*fileIndex->sizeArrayFile);


        for(int i=0;i<(int)jsonElement->sizeArrChildElement;i++){
            for(int j=0;j<(int)jsonElement->arrChildElement[i]->sizeArrChildElement;j++){

                OpenTime openTime=wcscmp(L"now",jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[4]->data->strData)==0?_NOW_:_LATER_;
                printf("INITFILE\n");
                fileIndex->arrayFile[j]=initFileElement(
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[0]->data->strData,
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[1]->data->strData,
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[2]->data->strData,
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[3]->data->strData,openTime);
            }
        }
    }
    else{
        return NULL;
    }

    return fileIndex;
}

File *selectFile(FileIndex *fileIndex, char *searchedName){


    for(unsigned int i=0; i<fileIndex->sizeArrayFile;i++){

        if(strcmp(fileIndex->arrayFile[i]->name,searchedName)==0){
            return fileIndex->arrayFile[i];
        }
    }
    return NULL;
}



 File *initFileElement(wchar_t *fileName, wchar_t *path, wchar_t *openMode,wchar_t *flux,OpenTime openTime){

    printf("BEGIN INIT");
     File *fileElement;

     fileElement=malloc(sizeof(File));

     wCharToChar(&fileName,&fileElement->name,_CONSERV_,_CONSERV_);
     wCharToChar(&path,&fileElement->path,_CONSERV_,_CONSERV_);
     wCharToChar(&openMode,&fileElement->openMode,_CONSERV_,_CONSERV_);

     if(openTime==_NOW_){

         if(wcscmp(flux,L"null")!=0){
             if(wcscmp(flux,L"stderr")==0) {
                 fileElement->filePointer=openFluxFile(fileElement->path,fileElement->openMode,stderr);
             }
             else if(wcscmp(flux,L"stdin")==0) {
                 fileElement->filePointer=openFluxFile(fileElement->path,fileElement->openMode,stdin);
             }
             else if(wcscmp(flux,L"stdout")==0) {
                 fileElement->filePointer=openFluxFile(fileElement->path,fileElement->openMode,stdout);
             }
         }
         else{
             fileElement->filePointer=openFile(fileElement->path,fileElement->openMode);
         }
     }


     printf("OK INIT\n");
     return fileElement;
 }




/*
 * ─── FONCTIONS D'OUVERTURE DE FICHIERS ───────────────────────────────────────────
 */

FILE *openFile(char *path, char *openMode){

    FILE *file;
    short bol=0;


    if((file=fopen(path,openMode))!=NULL){
        bol=1;
    }
    else if((file=fopen(path,"w+"))!=NULL){
        bol=1;
    }
    else{
        createErrorReport("Echec lors de l'ouverture d'un fichier",__FILE__,__LINE__,__DATE__,__TIME__);
    }
    if(bol){
        return file;
    }

    return NULL;
}


FILE *openFluxFile(char *path, char *openMode,  FILE * flux){

    FILE * redirectFile;
    short bol=0;

    if((redirectFile=freopen(path,openMode,flux))!=NULL){//freopen permet de rediriger un flux vers un fichier dans ce cas
        bol=1;
    }
    else if((redirectFile=freopen(path,"w+",flux))!=NULL){//Si ca ne fonctionne pas le mode w+ permet de créer le fichier
        bol=1;
    }
    else{
        createErrorReport("Echec lors de l'ouverture d'un flux",__FILE__,__LINE__,__DATE__,__TIME__);
    }

    if(bol){
        return redirectFile;
    }
    return NULL;
}


/*
 * ─── FONCTIONS DE LECTURE DE FICHIERS ───────────────────────────────────────────
 */
unsigned long countFileChar(FILE *filePtr){
    unsigned long fileSize;
    fseek(filePtr,0,SEEK_END);
    fileSize=(unsigned long)ftell(filePtr);
    fseek(filePtr,0,SEEK_SET);

    return fileSize;
}

wchar_t *getFileContent(FILE *filePtr){

    unsigned long fileSize=countFileChar(filePtr);
    wchar_t *fileContent=malloc(sizeof(wchar_t)*fileSize+1);
    wcfread(fileContent,fileSize,filePtr);
    fileContent[fileSize]=L'\0';
    return fileContent;
}

size_t wcfread(wchar_t *string,size_t size, FILE *file){

    for (size_t i = 0; i < size; i++) { 
        if ((string[i]=fgetwc(file)) == WEOF) { 
            return i; 
        } 
    } 
    return size; 
}

void createErrorReport(char *errorMessage,char * fileError, int lineError, char  *dateError, char *timeError){

    char  errorReportString[ERROR_BUFFER_SIZE]; 

    if(sprintf(errorReportString,"| %s | file %s | line %d | date %s | time %s |",errorMessage,fileError,lineError,dateError,timeError)>0){
        
        perror(errorReportString);//Perror permet l'envoie d'une erreur dans le flux stderr, le message d'erreur comprend le fichier, la ligne, la date et l'heure de l'erreur
    }

    else{fprintf(stderr,"Erreur lors de la creation d'un message d'erreur dans la fonction %s",__func__);}
}








