#pragma once

#include <stddef.h>

#define DSZ_CLIB_MODULE_VISIBILITY __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef DSZ_CLIB_MODULE_VISIBILITY
    #define DSZ_CLIB_MODULE_VISIBILITY __declspec(dllexport)
#endif /* defined(_MSC_VER) */

#define DSZ_CLIB_CALLING_CONVENTION
#if defined(_MSC_VER)
    #undef DSZ_CLIB_CALLING_CONVENTION
    #define DSZ_CLIB_CALLING_CONVENTION __stdcall
#endif /* defined(_MSC_VER) */

#if defined(__cplusplus)
    #define DSZ_CLIB_API(rt) \
        extern "C" \
        DSZ_CLIB_MODULE_VISIBILITY rt DSZ_CLIB_CALLING_CONVENTION
#else /* defined(__cplusplus) */
    #define DSZ_CLIB_API(rt) \
        DSZ_CLIB_MODULE_VISIBILITY rt DSZ_CLIB_CALLING_CONVENTION
#endif /* defined(__cplusplus) */

typedef size_t DszCLibErrNum;
typedef void* DszCLibAddress;
typedef void* DszCLibPerson;

DSZ_CLIB_API(size_t) DszCLibErrNumGetMessage(DszCLibErrNum errnum, char* message, size_t messageSize);

/* Library initialization routines. */
DSZ_CLIB_API(DszCLibErrNum) DszCLibLibraryInitialize(void);
DSZ_CLIB_API(void) DszCLibLibraryTerminate(void);

DSZ_CLIB_API(char const*) DszCLibLibraryGetVersionString(void);
DSZ_CLIB_API(size_t) DszCLibLibraryGetVersionMajor(void);
DSZ_CLIB_API(size_t) DszCLibLibraryGetVersionMinor(void);
DSZ_CLIB_API(size_t) DszCLibLibraryGetVersionPatch(void);
DSZ_CLIB_API(char const*) DszCLibLibraryGetVersionExtra(void);

DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressCreate(
    int streetNum, char const* street,
    char const* city, char const* province,
    char const* country, char const* zipCode,
    DszCLibAddress* newAddress
);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressDestroy(DszCLibAddress address);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressGetStreetNumber(DszCLibAddress address, int* streetNum);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressGetStreet(DszCLibAddress address, char* street, size_t streetSize, size_t* charWritten);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressGetCity(DszCLibAddress address, char* city, size_t citySize, size_t* charWritten);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressGetProvince(DszCLibAddress address, char* province, size_t provinceSize, size_t* charWritten);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressGetCountry(DszCLibAddress address, char* country, size_t countrySize, size_t* charWritten);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressGetZipCode(DszCLibAddress address, char* zipCode, size_t zipCodeSize, size_t* charWritten);
DSZ_CLIB_API(DszCLibErrNum) DszCLibAddressToString(DszCLibAddress address, char* str, size_t strSize, size_t* charWritten);

/*
** Note: DszCLibPersonCreate makes a copy of address argument, so it does not need to be persisted.
*/
DSZ_CLIB_API(DszCLibErrNum) DszCLibPersonCreate(
   char const* lastName, char const* firstName,
   int age, DszCLibAddress address,
   DszCLibPerson* newPerson
);
DSZ_CLIB_API(DszCLibErrNum) DszCLibPersonDestroy(DszCLibPerson person);
DSZ_CLIB_API(DszCLibErrNum) DszCLibPersonGetLastName(DszCLibPerson person, char* lastName, size_t lastNameSize, size_t* charWritten);
DSZ_CLIB_API(DszCLibErrNum) DszCLibPersonGetFirstName(DszCLibPerson person, char* firstName, size_t firstNameSize, size_t* charWritten);
DSZ_CLIB_API(DszCLibErrNum) DszCLibPersonGetAge(DszCLibPerson person, int* age);
DSZ_CLIB_API(DszCLibErrNum) DszCLibPersonGetAddress(DszCLibPerson person, DszCLibAddress* address);
DSZ_CLIB_API(DszCLibErrNum) DszCLibPersonToString(DszCLibPerson person, char* str, size_t strSize, size_t* charWritten);

#if defined(_MSC_VER)
typedef DSZ_CLIB_MODULE_VISIBILITY void (DSZ_CLIB_CALLING_CONVENTION *DszCLibGetNumberCallbackFunction)(int result);
#else /* defined(_MSC_VER) */
typedef void *DszCLibGetNumberCallbackFunction(int result);
#endif /* defined(_MSC_VER) */

DSZ_CLIB_API(DszCLibErrNum) DszCLibGetNumber(DszCLibGetNumberCallbackFunction fnCallback);
