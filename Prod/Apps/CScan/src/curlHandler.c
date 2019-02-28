#include <curl.h>

#include <curlHandler.h>



short curlConnect(char *url){

    CURL *curl = curl_easy_init();
    struct curl_slist *headers = NULL;
    CURLcode result;

    //Requête HTTP méthode GET
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    //URL de la requête
    curl_easy_setopt(curl, CURLOPT_URL, "https://ssl-api.openfoodfacts.org/api/v0/product/3392460342312.json");
    //Ne vérifie pas l'authenticité du certificat envoyé par le serveur quand la valeur est à 0, vérifie quand la valeur est à 1. 
    //J'ai mis la valeur à 0 car CURL stopait la connexion avec l'API openfoodfacts lorsqu'une vérification était effectuée. 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    //Création d'un header vide 

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    if((result = curl_easy_perform(curl)) != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));
    }
    curl_easy_cleanup(curl);

    curl_global_cleanup();

    return 0;
}
