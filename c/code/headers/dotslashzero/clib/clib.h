#pragma once

#include <stddef.h>

#define CLIB_MODULE_VISIBILITY __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef CLIB_MODULE_VISIBILITY
    #define CLIB_MODULE_VISIBILITY __declspec(dllexport)
#endif /* defined(_MSC_VER) */

#define CLIB_CALLING_CONVENTION
#if defined(_MSC_VER)
    #undef CLIB_CALLING_CONVENTION
    #define CLIB_CALLING_CONVENTION __stdcall
#endif /* defined(_MSC_VER) */

#if defined(__cplusplus)
    #define CLIB_API(rt) \
        extern "C" \
        CLIB_MODULE_VISIBILITY rt CLIB_CALLING_CONVENTION
#else /* defined(__cplusplus) */
    #define CLIB_API(rt) \
        CLIB_MODULE_VISIBILITY rt CLIB_CALLING_CONVENTION
#endif /* defined(__cplusplus) */

typedef size_t CLibErrNum;
typedef void* CLibAddress;
typedef void* CLibPerson;

CLIB_API(size_t) CLibErrNumGetMessage(CLibErrNum errnum, char* message, size_t messageSize);

/* Library initialization routines. */
CLIB_API(CLibErrNum) CLibLibraryInitialize(void);
CLIB_API(void) CLibLibraryTerminate(void);

CLIB_API(char const*) CLibLibraryGetVersionString(void);
CLIB_API(size_t) CLibLibraryGetVersionMajor(void);
CLIB_API(size_t) CLibLibraryGetVersionMinor(void);
CLIB_API(size_t) CLibLibraryGetVersionPatch(void);
CLIB_API(char const*) CLibLibraryGetVersionExtra(void);

CLIB_API(CLibErrNum) CLibAddressCreate(
    int streetNum, char const* street,
    char const* city, char const* province,
    char const* country, char const* zipCode,
    CLibAddress* newAddress
);
CLIB_API(CLibErrNum) CLibAddressDestroy(CLibAddress address);
CLIB_API(CLibErrNum) CLibAddressGetStreetNumber(CLibAddress address, int* streetNum);
CLIB_API(CLibErrNum) CLibAddressGetStreet(CLibAddress address, char* street, size_t streetSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLibAddressGetCity(CLibAddress address, char* city, size_t citySize, size_t* charWritten);
CLIB_API(CLibErrNum) CLibAddressGetProvince(CLibAddress address, char* province, size_t provinceSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLibAddressGetCountry(CLibAddress address, char* country, size_t countrySize, size_t* charWritten);
CLIB_API(CLibErrNum) CLibAddressGetZipCode(CLibAddress address, char* zipCode, size_t zipCodeSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLibAddressToString(CLibAddress address, char* str, size_t strSize, size_t* charWritten);

/*
** Note: CLibPersonCreate makes a copy of address argument, so it does not need to be persisted.
*/
CLIB_API(CLibErrNum) CLibPersonCreate(
   char const* lastName, char const* firstName,
   int age, CLibAddress address,
   CLibPerson* newPerson
);
CLIB_API(CLibErrNum) CLibPersonDestroy(CLibPerson person);
CLIB_API(CLibErrNum) CLibPersonGetLastName(CLibPerson person, char* lastName, size_t lastNameSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLibPersonGetFirstName(CLibPerson person, char* firstName, size_t firstNameSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLibPersonGetAge(CLibPerson person, int* age);
CLIB_API(CLibErrNum) CLibPersonGetAddress(CLibPerson person, CLibAddress* address);
CLIB_API(CLibErrNum) CLibPersonToString(CLibPerson person, char* str, size_t strSize, size_t* charWritten);

#if defined(_MSC_VER)
typedef CLIB_MODULE_VISIBILITY void (CLIB_CALLING_CONVENTION *CLibGetNumberCallbackFunction)(int result);
#else /* defined(_MSC_VER) */
typedef void *CLibGetNumberCallbackFunction(int result);
#endif /* defined(_MSC_VER) */

CLIB_API(CLibErrNum) CLibGetNumber(CLibGetNumberCallbackFunction fnCallback);
