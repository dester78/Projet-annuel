#define ERROR_BUFFER_SIZE 250
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fileHandler.h>
#include <jsonHandler.h>

File **initFileIndex(char *fileIndexPath){

    File **fileIndex;
    FILE *srcFile;

    fileIndex=malloc(sizeof(File*));

    if((srcFile=openFile(fileIndexPath,"r"))!=NULL){
        fileFileIndex(srcFile,&fileIndex);
    }   
}

File *initFileElement(char *fileName, char *path, char *openMode){

    File *fileElement;

    fileElement=malloc(sizeof(File));

    allocCopyString(&fileElement->name,fileName);
    allocCopyString(&fileElement->path,path);
    allocCopyString(&fileElement->openMode,openMode);
    fileElement->filePointer=openFile(fileElement->path,fileElement->openMode);

    return fileElement;
}


void allocCopyString(char **dst, char *src){

    *dst=malloc(sizeof(char)*(strlen(src)+1));
    strcpy(*dst,src);
}

/*
 * ─── FONCTIONS D'OUVERTURE DE FICHIERS ───────────────────────────────────────────
 */

FILE *openFile(char *fileName, char *openMode){

    FILE *file;

    if((file=fopen(fileName,openMode))!=NULL){
        return file;
    }
        
    else if((file=fopen(fileName,"w+"))!=NULL){
        return file;
    }

    else{
        createErrorReport("Echec lors de l'ouverture d'un fichier",__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL;
    }

    return NULL;
}


FILE *openFluxFile(char *fileName, char *openMode){

    FILE * redirectFile; 

    if((redirectFile=freopen(fileName,openMode,stderr))!=NULL){//freopen permet de rediriger un flux vers un fichier dans ce cas
        return redirectFile;
    }

    else if((redirectFile=freopen(fileName,"w+",stderr))!=NULL){//Si ca ne fonctionne pas le mode w+ permet de créer le fichier 
        return redirectFile;
    }

    else{
        createErrorReport("Echec lors de l'ouverture d'un flux",__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL; 
    }

    return NULL;
}

/*
 * ─── FONCTIONS DE LECTURE DE FICHIERS ───────────────────────────────────────────
 */

short returnConfigFileParameters(FILE *configFile, int *lastRow, char ***arrayParameters){

    char *fileRow; 
    int counterFileRow;
    int *arrayRowChar;
    char commentChar[2];
    int counterParameters=0;
    int sizeArrayParameters;
    
    if((sizeArrayParameters=countFileRowChar(&arrayRowChar,configFile,lastRow))==-1){
        createErrorReport("Echec lors de la lecture du fichier de configuration ",__FILE__,__LINE__,__DATE__,__TIME__);
        return 0;
    }
    

    if(((*arrayParameters)=malloc(sizeof(char*)*sizeArrayParameters))==NULL){
        createErrorReport("Echec d'allocation memoire",__FILE__,__LINE__,__DATE__,__TIME__);
        return 0;
    }
        

    for(counterFileRow=0;counterFileRow<*lastRow;counterFileRow++){

        errno=0;
        if((fileRow=calloc(((arrayRowChar[counterFileRow])+1),sizeof(char)))==NULL){
            createErrorReport("Echec d'allocation memoire",__FILE__,__LINE__,__DATE__,__TIME__);
            return 0;
        }
            
        if(arrayRowChar[counterFileRow]!=-1){
            
            if(arrayRowChar[counterFileRow] <= 1 && arrayRowChar[counterFileRow] < *lastRow - 1){   //Utile dans le cas d'une ligne compose seulement d'un \n
                if(fseek(configFile,2,SEEK_CUR)!=0){//Déplace le curseur de fichier de 2 caractère en partant de sa position courante, utilisé pour sauté les lignes vides
                    createErrorReport("Echec lors du deplacement du curseur de fichier",__FILE__,__LINE__,__DATE__,__TIME__);
                    return 0;
                }
            }
            else{
                fgets(fileRow,arrayRowChar[counterFileRow]+1,configFile);//Lit une ligne du fichier et déplace le curseur automatiquement

                if(errno==EAGAIN||errno==EBADF||errno==EINTR||errno==EIO||errno==ENOMEM||errno==ENXIO){
                    createErrorReport("Echec lors de la recuperation d'une ligne d'un fichier",__FILE__,__LINE__,__DATE__,__TIME__);
                    return 0;
                }
                
                if(strncpy(commentChar,fileRow,1)==NULL){
                    createErrorReport("Echec lors de la copie d'une chaine de caractere",__FILE__,__LINE__,__DATE__,__TIME__);
                    return 0;
                }
                

                if(commentChar[0]!='#'){//Vérifie si la ligne correspond à un paramètre ou un commentaire
                    
                    if(deleteLineFeed(&fileRow)==0){
                        createErrorReport("La suppression des retours a la ligne a echoue",__FILE__,__LINE__,__DATE__,__TIME__);
                        return 0;
                    }
                    if(deleteEndSpace(&fileRow)==0){
                        createErrorReport("La suprression des espaces de fin de chaine a echoue",__FILE__,__LINE__,__DATE__,__TIME__);
                        return 0;
                    }

                    if(((*arrayParameters)[counterParameters] = malloc( sizeof( char ) * ( strlen( fileRow ) + 1) ))==NULL){
                        createErrorReport("Echec d'allocation memoire",__FILE__,__LINE__,__DATE__,__TIME__);
                        return 0;
                    }  
                    if(strcpy( (*arrayParameters)[counterParameters],fileRow)==NULL){
                        createErrorReport("La copie de la chaine de caractere a echoue",__FILE__,__LINE__,__DATE__,__TIME__);
                        return 0;
                    }

                counterParameters++;              
                }  
            }   
        }
        free(fileRow);//Désalloue la ligne
    }
    
    free(arrayRowChar);

    *lastRow=counterParameters;

    return 1;
}

int countFileRowChar(int **arrayRowChar,FILE *file, int *lastRow){

    int counterRow=0; 
    int counterParameters=0;
    char bufferChar;
    int counterChar=0;
    errno=0;
    
    if(fseek(file,0,SEEK_SET)!=0){
        createErrorReport("Echec lors du deplacement d'un curseur de fichier",__FILE__,__LINE__,__DATE__,__TIME__);
        return 0;
    }
   
    if((*arrayRowChar=malloc(sizeof(int)))==NULL){
        createErrorReport("Echec d'allocation memoire",__FILE__,__LINE__,__DATE__,__TIME__);
        return -1;
    }

    while(counterChar!=-1){
        errno=0;
        bufferChar=fgetc(file);//Lit le fichier caractère par caractère

        if(errno==EAGAIN||errno==EBADF||errno==EILSEQ||errno==EINTR||errno==EINVAL||errno==ENOMEM||errno==ERANGE){
            createErrorReport("Echec lors de la recuperation de caractere d'un fichier",__FILE__,__LINE__,__DATE__,__TIME__);
            return -1;
        }

        counterChar++;

        if(bufferChar=='#'){
            counterParameters++;
        }
            
        if(bufferChar=='\n'||bufferChar==EOF){

            if((*arrayRowChar=realloc(*arrayRowChar,sizeof(int)*(counterRow+1)))==NULL){//Réalloue le tableau à chaque nouvelle ligne
                createErrorReport("Echec d'allocation memoire",__FILE__,__LINE__,__DATE__,__TIME__);
                return -1;
            }
            (*arrayRowChar)[counterRow]=counterChar;
            
            if(bufferChar==EOF){//Stop la boucle quand le curseur arrive à la fin du fichier
                counterChar=-1;
            }
            else{counterChar=0;}

            counterRow++;
        }
    }
    errno=0;
    fseek(file,0,SEEK_SET);//Replace le curseur en début de fichier

    if(errno==EAGAIN||errno==EBADF||errno==EILSEQ||errno==EINTR||errno==EINVAL||errno==ENOMEM||errno==ERANGE){
        createErrorReport("Echec lors d'un deplacement d'un curseur dans un fichier",__FILE__,__LINE__,__DATE__,__TIME__);
        return -1;
    }        

    *lastRow=counterRow;
    
    return counterParameters; 
}

/*
 * ─── FONCTIONS D'ECRITURE DE FICHIERS ────────────────────────────────────────────
 */


void createErrorReport(char *errorMessage,char * fileError, int lineError, char  *dateError, char *timeError){

    char  errorReportString[ERROR_BUFFER_SIZE]; 

    if(sprintf(errorReportString,"| %s | file %s | line %d | date %s | time %s |",errorMessage,fileError,lineError,dateError,timeError)>0){
        
        perror(errorReportString);//Perror permet l'envoie d'une erreur dans le flux stderr, le message d'erreur comprend le fichier, la ligne, la date et l'heure de l'erreur
    }

    else{fprintf(stderr,"Erreur lors de la creation d'un message d'erreur dans la fonction %s",__func__);}
}

void freeCharArray(char*** arrayChar, int lastRow ){

    int counter=0;

    while(counter<lastRow){
        
        free((*arrayChar)[counter]);
        counter++;
    }
    free(*arrayChar);
}







