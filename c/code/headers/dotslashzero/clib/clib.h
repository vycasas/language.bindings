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

typedef size_t DszCLibErrorNum;
typedef struct DszCLibAddress_* DszCLibAddress;
typedef struct DszCLibPerson_* DszCLibPerson;
typedef struct DszCLibGenerator_* DszCLibGenerator;
typedef struct DszCLibPrinter_* DszCLibPrinter;

/* Default values */
static DszCLibErrorNum const DSZ_CLIB_ERRORNUM_NO_ERROR     = (DszCLibErrorNum) 0;
static DszCLibAddress const DSZ_CLIB_ADDRESS_INVALID        = (DszCLibAddress) NULL;
static DszCLibPerson const DSZ_CLIB_PERSON_INVALID          = (DszCLibPerson) NULL;
static DszCLibGenerator const DSZ_CLIB_GENERATOR_INVALID    = (DszCLibGenerator) NULL;
static DszCLibPrinter const DSZ_CLIB_PRINTER_INVALID        = (DszCLibPrinter) NULL;

DSZ_CLIB_API(void) DszCLibErrorNumGetMessage(
    DszCLibErrorNum errnum,
    char* pMessage, size_t messageSize,
    size_t* pCharsWritten);

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryInitialize(void);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryUninitialize(void);

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionString(
    char* pVersionString, size_t versionStringSize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionMajor(
    size_t* pVersionMajor);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionMinor(
    size_t* pVersionMinor);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionPatch(
    size_t* pVersionPatch);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionExtra(
    char* pVersionExtraString, size_t versionExtraStringSize,
    size_t* pCharsWritten);

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressCreate(
    int streetNum,
    char const* street,
    char const* city,
    char const* province,
    char const* zipCode,
    char const* country,
    DszCLibAddress* pAddress);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressDestroy(
    DszCLibAddress address);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetStreetNum(
    DszCLibAddress address,
    int* pStreetNum);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetStreet(
    DszCLibAddress address,
    char* pStreet, size_t streetSize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetCity(
    DszCLibAddress address,
    char* pCity, size_t citySize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetProvince(
    DszCLibAddress address,
    char* pProvince, size_t provinceSize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetZipCode(
    DszCLibAddress address,
    char* pZipCode, size_t zipCodeSize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetCountry(
    DszCLibAddress address,
    char* pCountry, size_t countrySize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressToString(
    DszCLibAddress address,
    char* pAddressString, size_t addressStringSize,
    size_t* pCharsWritten);

/*
** Note: DszCLibPersonCreate makes a copy of address argument, so it does not need to be persisted.
*/
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonCreate(
    char const* lastName,
    char const* firstName,
    int age,
    DszCLibAddress address,
    DszCLibPerson* pPerson);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonDestroy(
    DszCLibPerson person);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetLastName(
    DszCLibPerson person,
    char* pLastName, size_t lastNameSize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetFirstName(
    DszCLibPerson person,
    char* pFirstName, size_t firstNameSize,
    size_t* pCharsWritten);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetAge(
    DszCLibPerson person,
    int* pAge);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetAddress(
    DszCLibPerson person,
    DszCLibAddress* pAddress);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonToString(
    DszCLibPerson person,
    char* pPersonString, size_t personStringSize,
    size_t* pCharsWritten);

#if defined(_MSC_VER)
typedef DSZ_CLIB_MODULE_VISIBILITY DszCLibErrorNum (DSZ_CLIB_CALLING_CONVENTION *DszCLibGenerateIntFunction)(
    int data,
    int* pInt,
    void* pUserData);
typedef DSZ_CLIB_MODULE_VISIBILITY DszCLibErrorNum (DSZ_CLIB_CALLING_CONVENTION *DszCLibGenerateStringFunction)(
    int data,
    char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData);
#else /* defined(_MSC_VER) */
typedef DszCLibErrorNum (*DszCLibGenerateIntFunction)(
    int data,
    int* pInt,
    void* pUserData);
typedef DszCLibErrorNum (*DszCLibGenerateStringFunction)(
    int data,
    char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData);
#endif /* defined(_MSC_VER) */

DSZ_CLIB_API(DszCLibErrorNum) DszCLibGeneratorCreate(
    DszCLibGenerateIntFunction fnGenerateInt,
    DszCLibGenerateStringFunction fnGenerateString,
    DszCLibGenerator* pGenerator);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibGeneratorDestroy(
    DszCLibGenerator generator);

/*
** Takes ownership of generator. Do not destroy generator after creating printer with this function.
*/
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterCreate(
    DszCLibGenerator generator,
    DszCLibPrinter* pPrinter);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterDestroy(
    DszCLibPrinter printer);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintInt(
    DszCLibPrinter printer);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintIntWithUserData(
    DszCLibPrinter printer,
    void* pUserData);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintString(
    DszCLibPrinter printer);
DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintStringWithUserData(
    DszCLibPrinter printer,
    void* pUserData);
