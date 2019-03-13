#include <stdlib.h>
#include <stdio.h>
#include <curlHandler.h>
#include <locale.h>
#include <jsonHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>

int main(){

    FileIndex *fileIndex;
    CurlRequest *curlRequest;
    wchar_t *string;
    int null;

    string=malloc(sizeof(wchar_t));
    string[0]='\0';
    File *testFile;
    JsonElement *urlElement;
    JsonElement *testCurl;
    printf("TEST");

    fileIndex=initFileIndex("json/fileIndex.json");
    testFile=selectFile(fileIndex,"curl");
    urlElement=readJsonFile(testFile->filePointer,10);
    curlRequest=jsonToCurl(urlElement,L"getProduct");
    curlConnect(curlRequest,&testCurl);

    scanf("%d",&null);
    printf("END MAIN");
}
