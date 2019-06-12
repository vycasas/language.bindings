#include <dotslashzero/clib/clib.h>

#include <dotslashzero/lb_common/version.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DSZ_CLIBCORE_INLINE inline

#if defined(_MSC_VER)
    #undef strncpy
    #define strncpy(buffer, source, bufferSize) strncpy_s(buffer, bufferSize, source, _TRUNCATE)
    #define snprintf sprintf_s
    #undef DSZ_CLIBCORE_INLINE
    #define DSZ_CLIBCORE_INLINE __inline
#endif /* defined(_WIN32) */

typedef enum DszCLibCoreErrorNum_
{
    DSZ_CLIBCORE_ERRORNUM_NO_ERROR          = 0,
    DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR     = 1,
    DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR    = 2
}
DszCLibCoreErrorNum;

typedef struct DszCLibCoreAddress_
{
    int streetNum;
    char street[40];
    char city[16];
    char province[8];
    char country[16];
    char zipCode[8];
}
DszCLibCoreAddress;

typedef struct DszCLibCorePerson_
{
    char lastName[24];
    char firstName[24];
    int age;
    DszCLibCoreAddress* pAddress;
}
DszCLibCorePerson;

typedef DszCLibErrorNum (*DszCLibCoreGenerateIntFunction)(
    int data, int* pInt);
typedef DszCLibErrorNum (*DszCLibCoreGenerateStringFunction)(
    int data, char* pString, size_t stringSize,
    size_t* pCharsWritten);

typedef struct DszCLibCoreGenerator_
{
    DszCLibCoreGenerateIntFunction fnGenerateInt;
    DszCLibCoreGenerateStringFunction fnGenerateString;
}
DszCLibCoreGenerator;

typedef struct DszCLibCorePrinter_
{
    DszCLibCoreGenerator* pGenerator;
}
DszCLibCorePrinter;

#define DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(errorNum) ((size_t) errorNum)

#if defined(__cplusplus)
extern "C"
#endif // defined(__cplusplus)
static DSZ_CLIBCORE_INLINE char const* DszCLibCoreGetErrorMessage(
    DszCLibCoreErrorNum errorNum)
{
    switch (errorNum) {
        case (DSZ_CLIBCORE_ERRORNUM_NO_ERROR):
            return ("No error.");
        case (DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR):
            return ("General error.");
        case (DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR):
            return ("External error.");
        default:
            return ("Unknown error.");
    }
}

DSZ_CLIB_API(void) DszCLibErrorNumGetMessage(
    DszCLibErrorNum errorNum,
    char* pMessage, size_t messageSize,
    size_t* pCharsWritten)
{
    size_t result = 0;
    char const* errorMessage = DszCLibCoreGetErrorMessage(errorNum);

    if ((pMessage != NULL) && (messageSize > 0)) {
        strncpy(pMessage, errorMessage, messageSize);
        pMessage[messageSize - 1] = '\0'; /* for safety */
        result = strlen(pMessage);
    }
    else {
        result = strlen(errorMessage);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = result;

    return;
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryInitialize(void)
{
    /* no operation */
    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryUninitialize(void)
{
    /* no operation */
    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionString(
    char* pVersionString, size_t versionStringSize,
    size_t* pCharsWritten)
{
    size_t result = 0;

    if ((pVersionString != NULL) && (versionStringSize > 0)) {
        strncpy(pVersionString, DSZ_LB_VERSION_STRING, versionStringSize);
        pVersionString[versionStringSize - 1] = '\0'; /* for safety */
        result = strlen(pVersionString);
    }
    else {
        result = strlen(DSZ_LB_VERSION_STRING);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = result;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionMajor(
    size_t* pVersionMajor)
{
    if (pVersionMajor != NULL)
        *pVersionMajor = DSZ_LB_VERSION_MAJOR;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionMinor(
    size_t* pVersionMinor)
{
    if (pVersionMinor != NULL)
        *pVersionMinor = DSZ_LB_VERSION_MINOR;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionPatch(
    size_t* pVersionPatch)
{
    if (pVersionPatch != NULL)
        *pVersionPatch = DSZ_LB_VERSION_PATCH;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibLibraryGetVersionExtra(
    char* pVersionExtraString, size_t versionExtraStringSize,
    size_t* pCharsWritten)
{
    size_t result = 0;

    if ((pVersionExtraString != NULL) && (versionExtraStringSize > 0)) {
        strncpy(pVersionExtraString, DSZ_LB_VERSION_EXTRA, versionExtraStringSize);
        pVersionExtraString[versionExtraStringSize - 1] = '\0'; /* for safety */
        result = strlen(pVersionExtraString);
    }
    else {
        result = strlen(DSZ_LB_VERSION_EXTRA);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = result;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressCreate(
    int streetNum,
    char const* street,
    char const* city,
    char const* province,
    char const* zipCode,
    char const* country,
    DszCLibAddress* pAddress)
{
    DszCLibCoreAddress* pCoreAddress = NULL;

    if (pAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    pCoreAddress = (DszCLibCoreAddress*) malloc(sizeof (DszCLibCoreAddress));

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset((void*) pCoreAddress, 0, sizeof (pCoreAddress));

    pCoreAddress->streetNum = streetNum;
    strncpy(pCoreAddress->street, street, 40);
    strncpy(pCoreAddress->city, city, 16);
    strncpy(pCoreAddress->province, province, 8);
    strncpy(pCoreAddress->zipCode, zipCode, 8);
    strncpy(pCoreAddress->country, country, 16);

    *pAddress = (DszCLibAddress) pCoreAddress;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressDestroy(
    DszCLibAddress address)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    free(pCoreAddress);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetStreetNum(
    DszCLibAddress address,
    int* pStreetNum)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;

    if (pStreetNum == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    *pStreetNum = pCoreAddress->streetNum;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetStreet(
    DszCLibAddress address,
    char* pStreet, size_t streetSize,
    size_t* pCharsWritten)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->street);

    if ((pStreet != NULL) && (streetSize > 0)) {
        strncpy(pStreet, pCoreAddress->street, streetSize);
        pStreet[streetSize - 1] = '\0';
        numChars = strlen(pStreet);
    }
    else {
        numChars = strlen(pCoreAddress->street);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetCity(
    DszCLibAddress address,
    char* pCity, size_t citySize,
    size_t* pCharsWritten)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->city);

    if ((pCity != NULL) && (citySize > 0)) {
        strncpy(pCity, pCoreAddress->city, citySize);
        pCity[citySize - 1] = '\0';
        numChars = strlen(pCity);
    }
    else {
        numChars = strlen(pCoreAddress->city);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetProvince(
    DszCLibAddress address,
    char* pProvince, size_t provinceSize,
    size_t* pCharsWritten)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->province);

    if ((pProvince != NULL) && (provinceSize > 0)) {
        strncpy(pProvince, pCoreAddress->province, provinceSize);
        pProvince[provinceSize - 1] = '\0';
        numChars = strlen(pProvince);
    }
    else {
        numChars = strlen(pCoreAddress->province);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetCountry(
    DszCLibAddress address,
    char* pCountry, size_t countrySize,
    size_t* pCharsWritten)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    if ((pCountry != NULL) && (countrySize > 0)) {
        strncpy(pCountry, pCoreAddress->country, countrySize);
        pCountry[countrySize - 1] = '\0';
        numChars = strlen(pCountry);
    }
    else {
        numChars = strlen(pCoreAddress->country);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetZipCode(
    DszCLibAddress address,
    char* pZipCode, size_t zipCodeSize,
    size_t* pCharsWritten)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->zipCode);

    if ((pZipCode != NULL) && (zipCodeSize > 0)) {
        strncpy(pZipCode, pCoreAddress->zipCode, zipCodeSize);
        pZipCode[zipCodeSize - 1] = '\0';
        numChars = strlen(pZipCode);
    }
    else {
        numChars = strlen(pCoreAddress->zipCode);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressToString(
    DszCLibAddress address,
    char* pAddressString, size_t addressStringSize,
    size_t* pCharsWritten)
{
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;
    size_t numChars = 0;
    size_t const BUFFER_SIZE = 120;
    char buffer[BUFFER_SIZE];

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset(buffer, 0, BUFFER_SIZE);
    snprintf(
        buffer, BUFFER_SIZE,
        "%d %s\n%s, %s\n%s %s",
        pCoreAddress->streetNum,
        pCoreAddress->street,
        pCoreAddress->city,
        pCoreAddress->province,
        pCoreAddress->zipCode,
        pCoreAddress->country);

    if ((pAddressString != NULL) && (addressStringSize > 0)) {
        strncpy(pAddressString, buffer, addressStringSize);
        pAddressString[addressStringSize - 1] = '\0';
        numChars = strlen(pAddressString);
    }
    else {
        numChars = strlen(buffer);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonCreate(
   char const* lastName, char const* firstName,
   int age, DszCLibAddress address,
   DszCLibPerson* pPerson)
{
    DszCLibCorePerson* pCorePerson = NULL;
    DszCLibErrorNum errorNum = DSZ_CLIBCORE_ERRORNUM_NO_ERROR;
    DszCLibCoreAddress* pCoreAddress = (DszCLibCoreAddress*) address;

    if (pPerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCorePerson = (DszCLibCorePerson*) malloc(sizeof (DszCLibCorePerson));
    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset((void*) pCorePerson, 0, sizeof (DszCLibCorePerson));

    strncpy(pCorePerson->lastName, lastName, 24);
    strncpy(pCorePerson->firstName, firstName, 24);
    pCorePerson->age = age;

    /* make a copy of address */
    errorNum = DszCLibAddressCreate(
        pCoreAddress->streetNum,
        pCoreAddress->street,
        pCoreAddress->city,
        pCoreAddress->province,
        pCoreAddress->zipCode,
        pCoreAddress->country,
        (DszCLibAddress*) &(pCorePerson->pAddress));

    if (errorNum != 0) {
        if (pCorePerson != NULL)
            free(pCorePerson);
        return (errorNum);
    }

    *pPerson = (DszCLibPerson) pCorePerson;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonDestroy(
    DszCLibPerson person)
{
    DszCLibCorePerson* pCorePerson = (DszCLibCorePerson*) person;

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    DszCLibAddressDestroy((DszCLibAddress) pCorePerson->pAddress);
    free(pCorePerson);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetLastName(
    DszCLibPerson person,
    char* pLastName, size_t lastNameSize,
    size_t* pCharsWritten)
{
    DszCLibCorePerson* pCorePerson = (DszCLibCorePerson*) person;
    size_t numChars = 0;

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCorePerson->lastName);

    if ((pLastName != NULL) && (lastNameSize > 0)) {
        strncpy(pLastName, pCorePerson->lastName, lastNameSize);
        numChars = strlen(pLastName);
    }
    else {
        numChars = strlen(pCorePerson->lastName);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetFirstName(
    DszCLibPerson person,
    char* pFirstName, size_t firstNameSize,
    size_t* pCharsWritten)
{
    DszCLibCorePerson* pCorePerson = (DszCLibCorePerson*) person;
    size_t numChars = 0;

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCorePerson->firstName);

    if ((pFirstName != NULL) && (firstNameSize > 0)) {
        strncpy(pFirstName, pCorePerson->firstName, firstNameSize);
        numChars = strlen(pFirstName);
    }
    else {
        numChars = strlen(pCorePerson->firstName);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetAge(
    DszCLibPerson person,
    int* pAge)
{
    DszCLibCorePerson* pCorePerson = (DszCLibCorePerson*) person;

    if (pAge == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    *pAge = pCorePerson->age;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetAddress(
    DszCLibPerson person,
    DszCLibAddress* pAddress)
{
    DszCLibCorePerson* pCorePerson = (DszCLibCorePerson*) person;
    DszCLibCoreAddress* pCoreAddress = NULL;
    DszCLibErrorNum errorNum = DSZ_CLIBCORE_ERRORNUM_NO_ERROR;

    if (pAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCoreAddress = pCorePerson->pAddress;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    errorNum = DszCLibAddressCreate(
        pCoreAddress->streetNum,
        pCoreAddress->street,
        pCoreAddress->city,
        pCoreAddress->province,
        pCoreAddress->zipCode,
        pCoreAddress->country,
        pAddress);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(errorNum));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonToString(
    DszCLibPerson person,
    char* pPersonString, size_t personStringSize,
    size_t* pCharsWritten)
{
    DszCLibErrorNum errorNum = DSZ_CLIBCORE_ERRORNUM_NO_ERROR;
    DszCLibCorePerson* pCorePerson = (DszCLibCorePerson*) person;
    size_t numChars = 0;
    size_t const ADDRESS_STRING_SIZE = 120;
    char addressString[ADDRESS_STRING_SIZE];
    size_t const BUFFER_SIZE = 184;
    char buffer[BUFFER_SIZE];

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset(addressString, 0, ADDRESS_STRING_SIZE);
    errorNum = DszCLibAddressToString(
        (DszCLibAddress) pCorePerson->pAddress,
        addressString, ADDRESS_STRING_SIZE,
        NULL);

    if (errorNum != DSZ_CLIBCORE_ERRORNUM_NO_ERROR) {
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(errorNum));
    }

    memset(buffer, 0, BUFFER_SIZE);
    snprintf(
        buffer, BUFFER_SIZE,
        "%s, %s\n%d years old\n%s",
        pCorePerson->lastName, pCorePerson->firstName,
        pCorePerson->age,
        addressString);

    if ((pPersonString != NULL) && (personStringSize > 0)) {
        strncpy(pPersonString, buffer, personStringSize);
        pPersonString[personStringSize - 1] = '\0';
        numChars = strlen(pPersonString);
    }
    else {
        numChars = strlen(buffer);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibGeneratorCreate(
    DszCLibGenerateIntFunction fnGenerateInt,
    DszCLibGenerateStringFunction fnGenerateString,
    DszCLibGenerator* pGenerator)
{
    DszCLibCoreGenerator* pCoreGenerator = NULL;

    if (pGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    pCoreGenerator = (DszCLibCoreGenerator*) malloc(sizeof (DszCLibCoreGenerator));

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset((void*) pCoreGenerator, 0, sizeof (pCoreGenerator));

    pCoreGenerator->fnGenerateInt = (DszCLibCoreGenerateIntFunction) fnGenerateInt;
    pCoreGenerator->fnGenerateString = (DszCLibCoreGenerateStringFunction) fnGenerateString;

    *pGenerator = (DszCLibGenerator) pCoreGenerator;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibGeneratorDestroy(
    DszCLibGenerator generator)
{
    DszCLibCoreGenerator* pCoreGenerator = (DszCLibCoreGenerator*) generator;

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    free(pCoreGenerator);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterCreate(
    DszCLibGenerator generator,
    DszCLibPrinter* pPrinter)
{
    DszCLibCorePrinter* pCorePrinter = NULL;
    DszCLibCoreGenerator* pCoreGenerator = (DszCLibCoreGenerator*) generator;

    if (pPrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));
    
    pCorePrinter = (DszCLibCorePrinter*) malloc(sizeof (DszCLibCorePrinter));
    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCorePrinter->pGenerator = pCoreGenerator;

    *pPrinter = (DszCLibPrinter) pCorePrinter;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterDestroy(
    DszCLibPrinter printer)
{
    DszCLibCorePrinter* pCorePrinter = (DszCLibCorePrinter*) printer;

    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    DszCLibGeneratorDestroy((DszCLibGenerator) pCorePrinter->pGenerator);
    free(pCorePrinter);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintInt(
    DszCLibPrinter printer)
{
    DszCLibCorePrinter* pCorePrinter = (DszCLibCorePrinter*) printer;
    DszCLibCoreGenerator* pCoreGenerator = NULL;
    DszCLibCoreGenerateIntFunction fnCoreGenerateInt = NULL;
    time_t data = time(NULL);
    int generatedInt = 0;

    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCoreGenerator = pCorePrinter->pGenerator;

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    fnCoreGenerateInt = pCoreGenerator->fnGenerateInt;

    if (fnCoreGenerateInt == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    fnCoreGenerateInt(data, &generatedInt);
    fprintf(stdout, "The value of int is: %d\n", generatedInt);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));    
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintString(
    DszCLibPrinter printer)
{
    DszCLibCorePrinter* pCorePrinter = (DszCLibCorePrinter*) printer;
    DszCLibCoreGenerator* pCoreGenerator = NULL;
    DszCLibCoreGenerateStringFunction fnCoreGenerateString = NULL;
    time_t data = time(NULL);
    size_t const GENERATED_STRING_SIZE = 64;
    char generatedString[GENERATED_STRING_SIZE];

    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCoreGenerator = pCorePrinter->pGenerator;

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    fnCoreGenerateString = pCoreGenerator->fnGenerateString;

    if (fnCoreGenerateString == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset(generatedString, 0, GENERATED_STRING_SIZE);
    fnCoreGenerateString(data, generatedString, GENERATED_STRING_SIZE, NULL);
    generatedString[GENERATED_STRING_SIZE - 1] = '\0';
    fprintf(stdout, "The value of string is: %s\n", generatedString);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));    
}
