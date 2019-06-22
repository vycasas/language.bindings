#include <dotslashzero/clib/clib.h>

#include <dotslashzero/lb_common/version.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__cplusplus)
#error Please use pure C compiler for this file.
#endif /* defined(__cplusplus) */

#define DSZ_CLIBCORE_INLINE inline

#if defined(_MSC_VER)
#define snprintf sprintf_s
#undef strncpy
#define strncpy(buffer, source, bufferSize) strncpy_s(buffer, bufferSize, source, _TRUNCATE)
#undef DSZ_CLIBCORE_INLINE
#define DSZ_CLIBCORE_INLINE __inline
#endif /* defined(_MSC_VER) */

#define DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(errorNum) ((size_t) errorNum)

#define DSZ_CLIBCORE_ADDRESS_STREET_SIZE 40
#define DSZ_CLIBCORE_ADDRESS_CITY_SIZE 16
#define DSZ_CLIBCORE_ADDRESS_PROVINCE_SIZE 16
#define DSZ_CLIBCORE_ADDRESS_ZIPCODE_SIZE 8
#define DSZ_CLIBCORE_ADDRESS_COUNTRY_SIZE 16
#define DSZ_CLIBCORE_ADDRESS_STRING_SIZE 128
#define DSZ_CLIBCORE_PERSON_LASTNAME_SIZE 24
#define DSZ_CLIBCORE_PERSON_FIRSTNAME_SIZE 24
#define DSZ_CLIBCORE_PERSON_STRING_SIZE 256

typedef enum DszCLibCoreErrorNum_
{
    DSZ_CLIBCORE_ERRORNUM_NO_ERROR          = 0,
    DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR     = 1,
    DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR    = 2
}
DszCLibCoreErrorNum;

typedef DszCLibCoreErrorNum* DszCLibCoreErrorNumPtr;

typedef struct DszCLibCoreAddress_
{
    int StreetNum;
    char Street[DSZ_CLIBCORE_ADDRESS_STREET_SIZE];
    char City[DSZ_CLIBCORE_ADDRESS_CITY_SIZE];
    char Province[DSZ_CLIBCORE_ADDRESS_PROVINCE_SIZE];
    char ZipCode[DSZ_CLIBCORE_ADDRESS_ZIPCODE_SIZE];
    char Country[DSZ_CLIBCORE_ADDRESS_COUNTRY_SIZE];
}
DszCLibCoreAddress;

typedef DszCLibCoreAddress* DszCLibCoreAddressPtr;

typedef struct DszCLibCorePerson_
{
    char LastName[DSZ_CLIBCORE_PERSON_LASTNAME_SIZE];
    char FirstName[DSZ_CLIBCORE_PERSON_FIRSTNAME_SIZE];
    int Age;
    DszCLibCoreAddressPtr Address;
}
DszCLibCorePerson;

typedef DszCLibCorePerson* DszCLibCorePersonPtr;

typedef DszCLibErrorNum (*DszCLibCoreGenerateIntFunction)(
    int data, int* pInt,
    void* pUserData);
typedef DszCLibErrorNum (*DszCLibCoreGenerateStringFunction)(
    int data, char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData);

typedef struct DszCLibCoreGenerator_
{
    DszCLibCoreGenerateIntFunction GenerateInt;
    DszCLibCoreGenerateStringFunction GenerateString;
}
DszCLibCoreGenerator;

typedef DszCLibCoreGenerator* DszCLibCoreGeneratorPtr;

typedef struct DszCLibCorePrinter_
{
    DszCLibCoreGeneratorPtr Generator;
}
DszCLibCorePrinter;

typedef DszCLibCorePrinter* DszCLibCorePrinterPtr;

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
    char const* errorMessage = DszCLibCoreGetErrorMessage(errorNum);
    size_t numChars = strlen(errorMessage);

    if ((pMessage != NULL) && (messageSize > 0)) {
        strncpy(pMessage, errorMessage, messageSize);
        pMessage[messageSize - 1] = '\0';
        numChars = strlen(pMessage);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

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
    size_t numChars = strlen(DSZ_LB_VERSION_STRING);

    if ((pVersionString != NULL) && (versionStringSize > 0)) {
        strncpy(pVersionString, DSZ_LB_VERSION_STRING, versionStringSize);
        pVersionString[versionStringSize - 1] = '\0';
        numChars = strlen(pVersionString);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

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
    size_t numChars = strlen(DSZ_LB_VERSION_EXTRA);

    if ((pVersionExtraString != NULL) && (versionExtraStringSize > 0)) {
        strncpy(pVersionExtraString, DSZ_LB_VERSION_EXTRA, versionExtraStringSize);
        pVersionExtraString[versionExtraStringSize - 1] = '\0'; /* for safety */
        numChars = strlen(pVersionExtraString);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

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
    DszCLibCoreAddressPtr pCoreAddress = NULL;

    if (pAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    pCoreAddress = (DszCLibCoreAddressPtr) malloc(sizeof (DszCLibCoreAddress));

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCoreAddress->StreetNum = streetNum;

    strncpy(pCoreAddress->Street, street, DSZ_CLIBCORE_ADDRESS_STREET_SIZE);
    pCoreAddress->Street[DSZ_CLIBCORE_ADDRESS_STREET_SIZE - 1] = '\0';

    strncpy(pCoreAddress->City, city, DSZ_CLIBCORE_ADDRESS_CITY_SIZE);
    pCoreAddress->City[DSZ_CLIBCORE_ADDRESS_CITY_SIZE - 1] = '\0';

    strncpy(pCoreAddress->Province, province, DSZ_CLIBCORE_ADDRESS_PROVINCE_SIZE);
    pCoreAddress->Province[DSZ_CLIBCORE_ADDRESS_PROVINCE_SIZE - 1] = '\0';

    strncpy(pCoreAddress->ZipCode, zipCode, DSZ_CLIBCORE_ADDRESS_ZIPCODE_SIZE);
    pCoreAddress->ZipCode[DSZ_CLIBCORE_ADDRESS_ZIPCODE_SIZE - 1] = '\0';

    strncpy(pCoreAddress->Country, country, DSZ_CLIBCORE_ADDRESS_COUNTRY_SIZE);
    pCoreAddress->Country[DSZ_CLIBCORE_ADDRESS_COUNTRY_SIZE - 1] = '\0';

    *pAddress = (DszCLibAddress) pCoreAddress; /* DszCLibAddress is an opaque pointer to DszCLibCoreAddress */

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressDestroy(
    DszCLibAddress address)
{
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    free(pCoreAddress);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetStreetNum(
    DszCLibAddress address,
    int* pStreetNum)
{
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;

    if (pStreetNum == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    *pStreetNum = pCoreAddress->StreetNum;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibAddressGetStreet(
    DszCLibAddress address,
    char* pStreet, size_t streetSize,
    size_t* pCharsWritten)
{
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->Street);

    if ((pStreet != NULL) && (streetSize > 0)) {
        strncpy(pStreet, pCoreAddress->Street, streetSize);
        pStreet[streetSize - 1] = '\0';
        numChars = strlen(pStreet);
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
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->City);

    if ((pCity != NULL) && (citySize > 0)) {
        strncpy(pCity, pCoreAddress->City, citySize);
        pCity[citySize - 1] = '\0';
        numChars = strlen(pCity);
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
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->Province);

    if ((pProvince != NULL) && (provinceSize > 0)) {
        strncpy(pProvince, pCoreAddress->Province, provinceSize);
        pProvince[provinceSize - 1] = '\0';
        numChars = strlen(pProvince);
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
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->ZipCode);

    if ((pZipCode != NULL) && (zipCodeSize > 0)) {
        strncpy(pZipCode, pCoreAddress->ZipCode, zipCodeSize);
        pZipCode[zipCodeSize - 1] = '\0';
        numChars = strlen(pZipCode);
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
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;
    size_t numChars = 0;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCoreAddress->Country);

    if ((pCountry != NULL) && (countrySize > 0)) {
        strncpy(pCountry, pCoreAddress->Country, countrySize);
        pCountry[countrySize - 1] = '\0';
        numChars = strlen(pCountry);
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
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;
    size_t numChars = 0;
    char coreAddressString[DSZ_CLIBCORE_ADDRESS_STRING_SIZE];

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset(coreAddressString, 0, DSZ_CLIBCORE_ADDRESS_STRING_SIZE);
    snprintf(
        coreAddressString, DSZ_CLIBCORE_ADDRESS_STRING_SIZE,
        "%d %s\n%s, %s\n%s %s",
        pCoreAddress->StreetNum,
        pCoreAddress->Street,
        pCoreAddress->City,
        pCoreAddress->Province,
        pCoreAddress->ZipCode,
        pCoreAddress->Country);

    numChars = strlen(coreAddressString);

    if ((pAddressString != NULL) && (addressStringSize > 0)) {
        strncpy(pAddressString, coreAddressString, addressStringSize);
        pAddressString[addressStringSize - 1] = '\0';
        numChars = strlen(pAddressString);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonCreate(
    char const* lastName,
    char const* firstName,
    int age,
    DszCLibAddress address,
    DszCLibPerson* pPerson)
{
    DszCLibCorePersonPtr pCorePerson = NULL;
    DszCLibErrorNum errorNum = DSZ_CLIBCORE_ERRORNUM_NO_ERROR;
    DszCLibCoreAddressPtr pCoreAddress = (DszCLibCoreAddressPtr) address;
    DszCLibCoreAddressPtr pCorePersonAddress = NULL;

    if (pPerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCorePerson = (DszCLibCorePersonPtr) malloc(sizeof (DszCLibCorePerson));
    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    strncpy(pCorePerson->LastName, lastName, DSZ_CLIBCORE_PERSON_LASTNAME_SIZE);
    pCorePerson->LastName[DSZ_CLIBCORE_PERSON_LASTNAME_SIZE - 1] = '\0';

    strncpy(pCorePerson->FirstName, firstName, DSZ_CLIBCORE_PERSON_FIRSTNAME_SIZE);
    pCorePerson->FirstName[DSZ_CLIBCORE_PERSON_FIRSTNAME_SIZE - 1] = '\0';

    pCorePerson->Age = age;

    /* make a copy of address */
    errorNum = DszCLibAddressCreate(
        pCoreAddress->StreetNum,
        pCoreAddress->Street,
        pCoreAddress->City,
        pCoreAddress->Province,
        pCoreAddress->ZipCode,
        pCoreAddress->Country,
        (DszCLibAddress*) &(pCorePersonAddress));

    if (errorNum != 0) {
        if (pCorePerson != NULL)
            free(pCorePerson);
        return (errorNum);
    }

    pCorePerson->Address = pCorePersonAddress;

    *pPerson = (DszCLibPerson) pCorePerson;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonDestroy(
    DszCLibPerson person)
{
    DszCLibCorePersonPtr pCorePerson = (DszCLibCorePersonPtr) person;

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    DszCLibAddressDestroy((DszCLibAddress) pCorePerson->Address);
    free(pCorePerson);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetLastName(
    DszCLibPerson person,
    char* pLastName, size_t lastNameSize,
    size_t* pCharsWritten)
{
    DszCLibCorePersonPtr pCorePerson = (DszCLibCorePersonPtr) person;
    size_t numChars = 0;

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCorePerson->LastName);

    if ((pLastName != NULL) && (lastNameSize > 0)) {
        strncpy(pLastName, pCorePerson->LastName, lastNameSize);
        pLastName[lastNameSize - 1] = '\0';
        numChars = strlen(pLastName);
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
    DszCLibCorePersonPtr pCorePerson = (DszCLibCorePersonPtr) person;
    size_t numChars = 0;

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    numChars = strlen(pCorePerson->FirstName);

    if ((pFirstName != NULL) && (firstNameSize > 0)) {
        strncpy(pFirstName, pCorePerson->FirstName, firstNameSize);
        pFirstName[firstNameSize - 1] = '\0';
        numChars = strlen(pFirstName);
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

    *pAge = pCorePerson->Age;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonGetAddress(
    DszCLibPerson person,
    DszCLibAddress* pAddress)
{
    DszCLibCorePersonPtr pCorePerson = (DszCLibCorePersonPtr) person;
    DszCLibCoreAddressPtr pCoreAddress = NULL;
    DszCLibErrorNum errorNum = DSZ_CLIBCORE_ERRORNUM_NO_ERROR;

    if (pAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCoreAddress = pCorePerson->Address;

    if (pCoreAddress == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    errorNum = DszCLibAddressCreate(
        pCoreAddress->StreetNum,
        pCoreAddress->Street,
        pCoreAddress->City,
        pCoreAddress->Province,
        pCoreAddress->ZipCode,
        pCoreAddress->Country,
        pAddress);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(errorNum));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPersonToString(
    DszCLibPerson person,
    char* pPersonString, size_t personStringSize,
    size_t* pCharsWritten)
{
    DszCLibCorePersonPtr pCorePerson = (DszCLibCorePersonPtr) person;
    DszCLibErrorNum errorNum = DSZ_CLIBCORE_ERRORNUM_NO_ERROR;
    size_t numChars = 0;
    char coreAddressString[DSZ_CLIBCORE_ADDRESS_STRING_SIZE];
    char corePersonString[DSZ_CLIBCORE_PERSON_STRING_SIZE];

    if (pCorePerson == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset(coreAddressString, 0, DSZ_CLIBCORE_ADDRESS_STRING_SIZE);
    errorNum = DszCLibAddressToString(
        (DszCLibAddress) pCorePerson->Address,
        coreAddressString, DSZ_CLIBCORE_ADDRESS_STRING_SIZE,
        NULL);

    if (errorNum != DSZ_CLIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(errorNum));

    coreAddressString[DSZ_CLIBCORE_ADDRESS_STRING_SIZE - 1] = '\0';

    memset(corePersonString, 0, DSZ_CLIBCORE_PERSON_STRING_SIZE);
    snprintf(
        corePersonString, DSZ_CLIBCORE_PERSON_STRING_SIZE,
        "%s, %s\n%d years old\n%s",
        pCorePerson->LastName, pCorePerson->FirstName,
        pCorePerson->Age,
        coreAddressString);

    numChars = strlen(corePersonString);

    if ((pPersonString != NULL) && (personStringSize > 0)) {
        strncpy(pPersonString, corePersonString, personStringSize);
        pPersonString[personStringSize - 1] = '\0';
        numChars = strlen(pPersonString);
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
    DszCLibCoreGeneratorPtr pCoreGenerator = NULL;

    if (pGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    pCoreGenerator = (DszCLibCoreGeneratorPtr) malloc(sizeof (DszCLibCoreGenerator));

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset((void*) pCoreGenerator, 0, sizeof (pCoreGenerator));

    pCoreGenerator->GenerateInt = (DszCLibCoreGenerateIntFunction) fnGenerateInt;
    pCoreGenerator->GenerateString = (DszCLibCoreGenerateStringFunction) fnGenerateString;

    *pGenerator = (DszCLibGenerator) pCoreGenerator;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibGeneratorDestroy(
    DszCLibGenerator generator)
{
    DszCLibCoreGeneratorPtr pCoreGenerator = (DszCLibCoreGeneratorPtr) generator;

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    free(pCoreGenerator);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterCreate(
    DszCLibGenerator generator,
    DszCLibPrinter* pPrinter)
{
    DszCLibCorePrinterPtr pCorePrinter = NULL;
    DszCLibCoreGeneratorPtr pCoreGenerator = (DszCLibCoreGeneratorPtr) generator;

    if (pPrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_EXTERNAL_ERROR));

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCorePrinter = (DszCLibCorePrinterPtr) malloc(sizeof (DszCLibCorePrinter));
    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCorePrinter->Generator = pCoreGenerator;

    *pPrinter = (DszCLibPrinter) pCorePrinter;

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterDestroy(
    DszCLibPrinter printer)
{
    DszCLibCorePrinterPtr pCorePrinter = (DszCLibCorePrinterPtr) printer;

    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    DszCLibGeneratorDestroy((DszCLibGenerator) pCorePrinter->Generator);
    free(pCorePrinter);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintInt(
    DszCLibPrinter printer)
{
    return (DszCLibPrinterPrintIntWithUserData(printer, NULL));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintIntWithUserData(
    DszCLibPrinter printer,
    void* pUserData)
{
    DszCLibCorePrinterPtr pCorePrinter = (DszCLibCorePrinterPtr) printer;
    DszCLibCoreGeneratorPtr pCoreGenerator = NULL;
    DszCLibCoreGenerateIntFunction fnCoreGenerateInt = NULL;
    time_t data = time(NULL);
    int generatedInt = 0;

    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCoreGenerator = pCorePrinter->Generator;

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    fnCoreGenerateInt = pCoreGenerator->GenerateInt;

    if (fnCoreGenerateInt == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    fnCoreGenerateInt((int) data, &generatedInt, pUserData);
    fprintf(stdout, "The value of int is: %d\n", generatedInt);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintString(
    DszCLibPrinter printer)
{
    return(DszCLibPrinterPrintStringWithUserData(printer, NULL));
}

DSZ_CLIB_API(DszCLibErrorNum) DszCLibPrinterPrintStringWithUserData(
    DszCLibPrinter printer,
    void* pUserData)
{
    size_t const GENERATED_STRING_SIZE_EXTRA = 8; /* for buffer safety */

    DszCLibCorePrinterPtr pCorePrinter = (DszCLibCorePrinterPtr) printer;
    DszCLibCoreGeneratorPtr pCoreGenerator = NULL;
    DszCLibCoreGenerateStringFunction fnCoreGenerateString = NULL;
    int data = (int) time(NULL);
    size_t generatedStringSize = 0;
    char* pGeneratedString = NULL;

    if (pCorePrinter == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    pCoreGenerator = pCorePrinter->Generator;

    if (pCoreGenerator == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    fnCoreGenerateString = pCoreGenerator->GenerateString;

    if (fnCoreGenerateString == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    fnCoreGenerateString(data, NULL, 0, &generatedStringSize, pUserData);

    if (generatedStringSize == 0)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    generatedStringSize += GENERATED_STRING_SIZE_EXTRA; /* for buffer safety */

    pGeneratedString = (char*) malloc((sizeof (char)) * (generatedStringSize));

    if (pGeneratedString == NULL)
        return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_GENERAL_ERROR));

    memset(pGeneratedString, 0, generatedStringSize);

    fnCoreGenerateString((int) data, pGeneratedString, generatedStringSize, NULL, pUserData);
    pGeneratedString[generatedStringSize - 1] = '\0';

    fprintf(stdout, "The value of string is: %s\n", pGeneratedString);

    free(pGeneratedString);

    return (DSZ_CLIBCORE_ERRORNUM_TO_CLIBERRORNUM(DSZ_CLIBCORE_ERRORNUM_NO_ERROR));
}
