#pragma once

#include <stddef.h>

#define CLIB_MODULE_VISIBILITY __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef CLIB_MODULE_VISIBILITY
    #define CLIB_MODULE_VISIBILITY __declspec(dllexport)
#endif /* defined(_MSC_VER) */

#define CLIB_CALLING_CONVENTION __stdcall

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

CLIB_API(size_t) CLErrNumGetMessage(CLibErrNum errnum, char* message, size_t messageSize);

/* Library initialization routines. */
CLIB_API(CLibErrNum) CLLibraryInitialize(void);
CLIB_API(void) CLLibraryTerminate(void);

CLIB_API(char const*) CLLibraryGetVersionString(void);
CLIB_API(size_t) CLLibraryGetVersionMajor(void);
CLIB_API(size_t) CLLibraryGetVersionMinor(void);
CLIB_API(size_t) CLLibraryGetVersionPatch(void);
CLIB_API(char const*) CLLibraryGetVersionExtra(void);

CLIB_API(CLibErrNum) CLAddressCreate(
    int streetNum, char const* street,
    char const* city, char const* province,
    char const* country, char const* zipCode,
    CLibAddress* newAddress
);
CLIB_API(CLibErrNum) CLAddressDestroy(CLibAddress address);
CLIB_API(CLibErrNum) CLAddressGetStreetNumber(CLibAddress address, int* streetNum);
CLIB_API(CLibErrNum) CLAddressGetStreet(CLibAddress address, char* street, size_t streetSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLAddressGetCity(CLibAddress address, char* city, size_t citySize, size_t* charWritten);
CLIB_API(CLibErrNum) CLAddressGetProvince(CLibAddress address, char* province, size_t provinceSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLAddressGetCountry(CLibAddress address, char* country, size_t countrySize, size_t* charWritten);
CLIB_API(CLibErrNum) CLAddressGetZipCode(CLibAddress address, char* zipCode, size_t zipCodeSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLAddressToString(CLibAddress address, char* str, size_t strSize, size_t* charWritten);

/*
** Note: CLPersonCreate makes a copy of address argument, so it does not need to be persisted.
*/
CLIB_API(CLibErrNum) CLPersonCreate(
   char const* lastName, char const* firstName,
   int age, CLibAddress address,
   CLibPerson* newPerson
);
CLIB_API(CLibErrNum) CLPersonDestroy(CLibPerson person);
CLIB_API(CLibErrNum) CLPersonGetLastName(CLibPerson person, char* lastName, size_t lastNameSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLPersonGetFirstName(CLibPerson person, char* firstName, size_t firstNameSize, size_t* charWritten);
CLIB_API(CLibErrNum) CLPersonGetAge(CLibPerson person, int* age);
CLIB_API(CLibErrNum) CLPersonGetAddress(CLibPerson person, CLibAddress* address);
CLIB_API(CLibErrNum) CLPersonToString(CLibPerson person, char* str, size_t strSize, size_t* charWritten);
