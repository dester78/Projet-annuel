#ifndef CONVERTCHARHANDLER
#define CONVERTCHARHANDLER
#include <wchar.h>

typedef enum ConvertParameter{_FREE_,_CONSERV_}ConvertParameter ;


short wCharToChar(char **wValue,char **cValue,ConvertParameter wConvertParameter,ConvertParameter cConvertParameter);

short charToWchar(char **cValue,char **wValue,ConvertParameter cConvertParameter,ConvertParameter wConvertParameter);


short allocCopyString(char **dst, char *src);


#endif
