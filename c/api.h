#if !defined(CLIB_API_H)
#define CLIB_API_H

#define CLIB_MODULE_VISIBLE __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef CLIB_MODULE_VISIBLE
    #define CLIB_MODULE_VISIBLE __declspec(dllexport)
#endif /* defined(_MSC_VER) */

#if defined(__cplusplus)
    #define CLIB_API \
        extern "C" \
        CLIB_MODULE_VISIBLE
#else /* defined(__cplusplus) */
    #define CLIB_API \
        CLIB_MODULE_VISIBLE
#endif /* defined(__cplusplus) */

#include <stddef.h>

typedef size_t CLibErrNum;
typedef void* CLibAddress;
typedef void* CLibPerson;

CLIB_API size_t CLErrNumGetMessage(CLibErrNum errnum, char* message, size_t messageSize);

/* Library initialization routines. */
CLIB_API CLibErrNum CLLibraryInitialize(void);
CLIB_API void CLLibraryTerminate(void);

CLIB_API CLibErrNum CLAddressCreate(
    int streetNum, const char* street,
    const char* city, const char* province,
    const char* country, const char* zipCode,
    CLibAddress* newAddress
);
CLIB_API CLibErrNum CLAddressDestroy(CLibAddress address);
CLIB_API CLibErrNum CLAddressGetStreetNumber(CLibAddress address, int* streetNum);
CLIB_API CLibErrNum CLAddressGetStreet(CLibAddress address, char* street, size_t streetSize, size_t* charWritten);
CLIB_API CLibErrNum CLAddressGetCity(CLibAddress address, char* city, size_t citySize, size_t* charWritten);
CLIB_API CLibErrNum CLAddressGetProvince(CLibAddress address, char* province, size_t provinceSize, size_t* charWritten);
CLIB_API CLibErrNum CLAddressGetCountry(CLibAddress address, char* country, size_t countrySize, size_t* charWritten);
CLIB_API CLibErrNum CLAddressGetZipCode(CLibAddress address, char* zipCode, size_t zipCodeSize, size_t* charWritten);
CLIB_API CLibErrNum CLAddressToString(CLibAddress address, char* str, size_t strSize, size_t* charWritten);

/*
** Note: CLPersonCreate makes a copy of address argument, so it does not need to be persisted.
*/
CLIB_API CLibErrNum CLPersonCreate(
   const char* lastName, const char* firstName,
   int age, CLibAddress address,
   CLibPerson* newPerson
);
CLIB_API CLibErrNum CLPersonDestroy(CLibPerson person);
CLIB_API CLibErrNum CLPersonGetLastName(CLibPerson person, char* lastName, size_t lastNameSize, size_t* charWritten);
CLIB_API CLibErrNum CLPersonGetFirstName(CLibPerson person, char* firstName, size_t firstNameSize, size_t* charWritten);
CLIB_API CLibErrNum CLPersonGetAge(CLibPerson person, int* age);
CLIB_API CLibErrNum CLPersonGetAddress(CLibPerson person, CLibAddress* address);
CLIB_API CLibErrNum CLPersonToString(CLibPerson person, char* str, size_t strSize, size_t* charWritten);

#endif /* !defined(CLIB_API_H) */
