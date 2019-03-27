#ifndef CONVERTCHARHANDLER
#define CONVERTCHARHANDLER
#include <wchar.h>

typedef enum ConvertParameter{_FREE_,_CONSERV_}ConvertParameter ;


short wCharToChar(wchar_t **wValue,char **cValue,ConvertParameter wConvertParameter,ConvertParameter cConvertParameter);

short charToWchar(char **cValue,wchar_t **wValue,ConvertParameter cConvertParameter,ConvertParameter wConvertParameter);


short allocCopyString(wchar_t **dst, wchar_t *src);


#endif
