#define ERROR_BUFFER_SIZE 250
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#include <convertCharHandler.h>
#include <fileHandler.h>
#include <jsonHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>

FileIndex *initFileIndex(char *fileIndexPath){

    FileIndex *fileIndex;
    FILE *srcFile;
    JsonElement *jsonElement;
    char *jsonString;

    if((jsonString=malloc(sizeof(char)))==NULL){
        createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL;
    }
    if((fileIndex=malloc(sizeof(FileIndex)))==NULL){
        createErrorReport("Memory allocation error :  ",__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL;
    }

    jsonString[0]='\0';

    if((srcFile=openFile(fileIndexPath,"r"))!=NULL){

        jsonElement=readJsonFile(srcFile,10);
        
        writeJsonElement(jsonElement,&jsonString,3000);
        printf("OK WRITE : %s",jsonString);
        fileIndex->sizeArrayFile=jsonElement->arrChildElement[0]->sizeArrChildElement;
        if((fileIndex->arrayFile=malloc(sizeof(File*)*fileIndex->sizeArrayFile))==NULL){
            createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
            return NULL;
        }

        for(int i=0;i<(int)jsonElement->sizeArrChildElement;i++){
            for(int j=0;j<(int)jsonElement->arrChildElement[i]->sizeArrChildElement;j++){


                OpenTime openTime=strcmp("now",jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[4]->data->strData)==0?_NOW_:_LATER_;
                if((fileIndex->arrayFile[j]=initFileElement(
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[0]->data->strData,
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[1]->data->strData,
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[2]->data->strData,
                        jsonElement->arrChildElement[i]->arrChildElement[j]->arrChildElement[3]->data->strData,openTime))==NULL){
                    createErrorReport("Index file structure making error : ",__FILE__,__LINE__,__DATE__,__TIME__);
                    return NULL;
                }
            }
        }
        freeJsonElement(&jsonElement);
    }
    else{
        createErrorReport("Opening index file error : ",__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL;
    }

    printf("OK FILE INDEX");

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



 File *initFileElement(char *fileName, char *path, char *openMode,char *flux,OpenTime openTime){

    File *fileElement;

     if((fileElement=malloc(sizeof(File)))==NULL){
         createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
         return NULL;
     }

     fileElement->name=malloc(sizeof(char)*(strlen(fileName)+1));
     strcpy(fileElement->name,fileName);
     fileElement->path=malloc(sizeof(char)*(strlen(path)+1));
     strcpy(fileElement->path,path);
     fileElement->openMode=malloc(sizeof(char)*(strlen(openMode)+1));
     strcpy(fileElement->openMode,openMode);

    //  if(wCharToChar(&fileName,&fileElement->name,_CONSERV_,_CONSERV_)==0||wCharToChar(&path,&fileElement->path,_CONSERV_,_CONSERV_)==0 || wCharToChar(&openMode,&fileElement->openMode,_CONSERV_,_CONSERV_) == 0){
    //      createErrorReport("Translating wide char error : ",__FILE__,__LINE__,__DATE__,__TIME__);
    //      return NULL;
    //  }

     if(openTime==_NOW_){
         if(strcmp(flux,"null")!=0){
             if(strcmp(flux,"stderr")==0) {
                 fileElement->filePointer=openFluxFile(fileElement->path,fileElement->openMode,stderr);
             }
             else if(strcmp(flux,"stdin")==0) {
                 fileElement->filePointer=openFluxFile(fileElement->path,fileElement->openMode,stdin);
             }
             else if(strcmp(flux,"stdout")==0) {
                 fileElement->filePointer=openFluxFile(fileElement->path,fileElement->openMode,stdout);
             }
         }
         else{
             fileElement->filePointer=openFile(fileElement->path,fileElement->openMode);
         }
     }
     return fileElement;
 }

 short freeFileElement(File *fileElement){

    if(fileElement->name!=NULL){
        free(fileElement->name);
    }
    if(fileElement->path!=NULL){
        free(fileElement->path);
    }
    if(fileElement->openMode!=NULL){
        free(fileElement->openMode);
    }
    if(fileElement->filePointer!=NULL){
        fclose(fileElement->filePointer);
    }
    return 1;
}



/*
 * ─── FONCTIONS D'OUVERTURE DE FICHIERS ───────────────────────────────────────────
 */

FILE *openFile(char *path, char *openMode){

    FILE *file;
    short bol=0;

    printf("PATH : %s",path);
    printf("PATH : %s",openMode);
    fflush(stdout);
    if((file=fopen(path,openMode))!=NULL){
        bol=1;
    }
    else if((file=fopen(path,"w+"))!=NULL){
        bol=1;
    }
    else{
        createErrorReport("File opening error : ",__FILE__,__LINE__,__DATE__,__TIME__);
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
    else if((redirectFile=freopen(path,"w+",flux))!=NULL){//Si ca ne fonctionne pas le mode w+ permet de creer le fichier
        bol=1;
    }
    else{
        createErrorReport("File opening error : ",__FILE__,__LINE__,__DATE__,__TIME__);
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

char *getFileContent(FILE *filePtr){

    char *fileContent;

    unsigned long fileSize=countFileChar(filePtr);
    if((fileContent=malloc(sizeof(char)*fileSize+1))==NULL){
        createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL;
    }
    fread(fileContent, fileSize, 1, filePtr);
    // wcfread(fileContent,fileSize,filePtr);
    // printf("CONTENT : %s",fileContent);
    fileContent[fileSize]='\0';
    return fileContent;
}

// size_t wcfread(char *string,size_t size, FILE *file){

//     for (size_t i = 0; i < size; i++) { 
//         if ((string[i]=fgetwc(file)) == WEOF) { 
//             return i; 
//         } 
//     } 
//     return size; 
// }

void createErrorReport(char *errorMessage,char * fileError, int lineError, char  *dateError, char *timeError){

    char  errorReportString[ERROR_BUFFER_SIZE]; 

    if(sprintf(errorReportString,"| %s | file %s | line %d | date %s | time %s |",errorMessage,fileError,lineError,dateError,timeError)>0){
        
        perror(errorReportString);//Perror permet l'envoie d'une erreur dans le flux stderr, le message d'erreur comprend le fichier, la ligne, la date et l'heure de l'erreur
    }

    else{fprintf(stderr,"Erreur lors de la creation d'un message d'erreur dans la fonction %s",__func__);}
}








