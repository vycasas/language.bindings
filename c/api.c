#include "api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _CoreErrNums
{
    COREERRNUM_NO_ERROR = 0,
    COREERRNUM_GENERAL_ERROR = 1,
    COREERRNUM_EXTERNAL_ERROR = 2
}
CoreErrNums;

#define TO_CLIBERRNUM(errnum) ((CoreErrNums) errnum)

#if defined(__cplusplus)
extern "C"
#endif // defined(__cplusplus)
static inline const char* CoreGetErrorMessage(CoreErrNums errnum)
{
    switch (errnum) {
        case (COREERRNUM_NO_ERROR):
            return ("No error.");
        case (COREERRNUM_GENERAL_ERROR):
            return ("General error.");
        case (COREERRNUM_EXTERNAL_ERROR):
            return ("External error.");
        default:
            return ("Unknown error.");
    }
}

CLIB_API size_t CLErrNumGetMessage(CLibErrNum errnum, char* message, size_t messageSize)
{
    size_t result = 0;
    const char* const errMessage = CoreGetErrorMessage(errnum);

    if (message != NULL && messageSize > 0) {
        strncpy(message, errMessage, messageSize);
        result = strlen(message);
    }
    else {
        result = strlen(errMessage);
    }

    return (result);
}

typedef struct _CoreAddress
{
    int streetNum;
    char street[40];
    char city[16];
    char province[8];
    char country[16];
    char zipCode[8];
}
CoreAddress;

CLIB_API CLibErrNum CLAddressCreate(
    int streetNum, const char* street,
    const char* city, const char* province,
    const char* country, const char* zipCode,
    CLibAddress* newAddress
)
{
    if (newAddress == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) malloc(sizeof (CoreAddress));
    if (coreAddr == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_GENERAL_ERROR));

    memset((void*) coreAddr, 0, sizeof (CoreAddress));

    coreAddr->streetNum = streetNum;
    strncpy(coreAddr->street, street, 40);
    strncpy(coreAddr->city, city, 16);
    strncpy(coreAddr->province, province, 8);
    strncpy(coreAddr->country, country, 16);
    strncpy(coreAddr->zipCode, zipCode, 8);

    *newAddress = (CLibAddress) coreAddr;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressDestroy(CLibAddress address)
{
    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) address;
    free(coreAddr);

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressGetStreetNumber(CLibAddress address, int* streetNum)
{
    if (address == NULL || streetNum == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));
    
    CoreAddress* coreAddr = (CoreAddress*) address;
    *streetNum = coreAddr->streetNum;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressGetStreet(CLibAddress address, char* street, size_t streetSize, size_t* charWritten)
{
    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) address;
    size_t numChars = strlen(coreAddr->street);

    if (street != NULL && streetSize > 0) {
        strncpy(street, coreAddr->street, streetSize);
        numChars = strlen(street);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressGetCity(CLibAddress address, char* city, size_t citySize, size_t* charWritten)
{
    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) address;
    size_t numChars = strlen(coreAddr->street);

    if (city != NULL && citySize > 0) {
        strncpy(city, coreAddr->city, citySize);
        numChars = strlen(city);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressGetProvince(CLibAddress address, char* province, size_t provinceSize, size_t* charWritten)
{
    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) address;
    size_t numChars = strlen(coreAddr->street);

    if (province != NULL && provinceSize > 0) {
        strncpy(province, coreAddr->province, provinceSize);
        numChars = strlen(province);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressGetCountry(CLibAddress address, char* country, size_t countrySize, size_t* charWritten)
{
    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) address;
    size_t numChars = strlen(coreAddr->street);

    if (country != NULL && countrySize > 0) {
        strncpy(country, coreAddr->country, countrySize);
        numChars = strlen(country);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressGetZipCode(CLibAddress address, char* zipCode, size_t zipCodeSize, size_t* charWritten)
{
    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) address;
    size_t numChars = strlen(coreAddr->street);

    if (zipCode != NULL && zipCodeSize > 0) {
        strncpy(zipCode, coreAddr->zipCode, zipCodeSize);
        numChars = strlen(zipCode);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLAddressToString(CLibAddress address, char* str, size_t strSize, size_t* charWritten)
{
    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CoreAddress* coreAddr = (CoreAddress*) address;
    size_t numChars = 0;
    char buffer[120];

    memset(buffer, 0, 120);
    snprintf(
        buffer, 120,
        "%d %s\n%s, %s\n%s %s",
        coreAddr->streetNum, coreAddr->street,
        coreAddr->city, coreAddr->province,
        coreAddr->country, coreAddr->zipCode
    );

    numChars = strlen(buffer);

    if (str != NULL && strSize > 0) {
        strncpy(str, buffer, strSize);
        numChars = strlen(str);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

typedef struct _CorePerson
{
    char lastName[24];
    char firstName[24];
    int age;
    CoreAddress address;
}
CorePerson;

CLIB_API CLibErrNum CLPersonCreate(
   const char* lastName, const char* firstName,
   int age, CLibAddress address,
   CLibPerson* newPerson
)
{
    if (newPerson == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    if (address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CorePerson* corePerson = (CorePerson*) malloc(sizeof (CorePerson));
    if (corePerson == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_GENERAL_ERROR));

    memset((void*) corePerson, 0, sizeof (CorePerson));

    CoreAddress* coreAddr = (CoreAddress*) address;

    strncpy(corePerson->lastName, lastName, 24);
    strncpy(corePerson->firstName, firstName, 24);
    corePerson->age = age;
    memcpy(&(corePerson->address), coreAddr, sizeof (CoreAddress));

    *newPerson = (CLibPerson) corePerson;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLPersonDestroy(CLibPerson person)
{
    if (person == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CorePerson* corePerson = (CorePerson*) person;
    free(corePerson);

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLPersonGetLastName(CLibPerson person, char* lastName, size_t lastNameSize, size_t* charWritten)
{
    if (person == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CorePerson* corePerson = (CorePerson*) person;
    size_t numChars = strlen(corePerson->lastName);

    if (lastName != NULL && lastNameSize > 0) {
        strncpy(lastName, corePerson->lastName, lastNameSize);
        numChars = strlen(lastName);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLPersonGetFirstName(CLibPerson person, char* firstName, size_t firstNameSize, size_t* charWritten)
{
    if (person == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CorePerson* corePerson = (CorePerson*) person;
    size_t numChars = strlen(corePerson->firstName);

    if (firstName != NULL && firstNameSize > 0) {
        strncpy(firstName, corePerson->firstName, firstNameSize);
        numChars = strlen(firstName);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLPersonGetAge(CLibPerson person, int* age)
{
    if (person == NULL || age == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CorePerson* corePerson = (CorePerson*) person;
    *age = corePerson->age;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLPersonGetAddress(CLibPerson person, CLibAddress* address)
{
    if (person == NULL || address == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CorePerson* corePerson = (CorePerson*) person;
    CLAddressCreate(
        corePerson->address.streetNum, corePerson->address.street,
        corePerson->address.city, corePerson->address.province,
        corePerson->address.country, corePerson->address.zipCode,
        address
    );

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}

CLIB_API CLibErrNum CLPersonToString(CLibPerson person, char* str, size_t strSize, size_t* charWritten)
{
    if (person == NULL)
        return (TO_CLIBERRNUM(COREERRNUM_EXTERNAL_ERROR));

    CorePerson* corePerson = (CorePerson*) person;
    CoreAddress* coreAddr = &(corePerson->address);
    size_t numChars = 0;
    char buffer[184];

    memset(buffer, 0, 184);
    snprintf(
        buffer, 184,
        "%s, %s\n%d years old\n%d %s\n%s, %s\n%s %s",
        corePerson->lastName, corePerson->firstName,
        corePerson->age,
        coreAddr->streetNum, coreAddr->street,
        coreAddr->city, coreAddr->province,
        coreAddr->country, coreAddr->zipCode
    );

    numChars = strlen(buffer);

    if (str != NULL && strSize > 0) {
        strncpy(str, buffer, strSize);
        numChars = strlen(str);
    }

    if (charWritten != NULL)
        *charWritten = numChars;

    return (TO_CLIBERRNUM(COREERRNUM_NO_ERROR));
}
