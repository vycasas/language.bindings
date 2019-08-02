#import <dotslashzero/objclib/ObjCLib.h>

#include <dotslashzero/clib/clib.h> /* it is also possible to use the C++ APIs instead */

#include <string.h>

#if defined(__cplusplus)
#error Please use pure C compiler for this file.
#endif /* defined(__cplusplus) */

#define DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum) \
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) \
        @throw ([[OLException alloc] initWithDszCLibErrorNum: cLibErrorNum]);

/* -------- */
/* additional interface declarations that are hidden from the public header file */

@interface OLAddress()
{
    DszCLibAddress m_impl;
}

- (instancetype) initWithImpl: (DszCLibAddress) impl;
- (DszCLibAddress) getImpl;

@end /* interface OLAddress */

@interface OLPerson()
{
    DszCLibPerson m_impl;
}
@end /* interface OLPerson */

@interface OLPrinter()
{
    DszCLibPrinter m_impl;
    id<OLGenerator> m_generator;
}

- (id<OLGenerator>) getGenerator;

@end /* interface OLPrinter */

/* -------- */

/* -------- */
/* internal helper functions that bridges C and Objective-C types */

static DszCLibErrorNum OLCorePrinterGenerateIntRedirect(
    int data,
    int* pInt,
    void* pUserData)
{
    id<OLGenerator> olGenerator = nil;

    if (pUserData == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    if (pInt == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    olGenerator = (__bridge id<OLGenerator>) pUserData; /* __bridge = no transfer of ownership */

    if (olGenerator == nil)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    *pInt = [olGenerator generateIntWithData: data];

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

static DszCLibErrorNum OLCorePrinterGenerateStringRedirect(
    int data,
    char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData)
{
    id<OLGenerator> olGenerator = nil;
    NSString* nsGeneratedString = nil;
    char const* cGeneratedString = NULL;
    size_t numChars = 0;

    if (pUserData == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    olGenerator = (__bridge id<OLGenerator>) pUserData; /* __bridge = no transfer of ownership */

    if (olGenerator == nil)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    nsGeneratedString = [olGenerator generateStringWithData: data];

    if (nsGeneratedString == nil)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    cGeneratedString = [nsGeneratedString UTF8String];
    numChars = strlen(cGeneratedString);

    if ((pString != NULL) && (stringSize > 0)) {
        strncpy(pString, cGeneratedString, stringSize);
        pString[stringSize - 1] = '\0';
        numChars = strlen(pString);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

/* -------- */

/* -------- */
/* implementation details */

@implementation OLException

- (instancetype) initWithDszCLibErrorNum: (DszCLibErrorNum) errorNum
{
    size_t const ERRORNUM_STRING_SIZE = 40;
    char errorNumString[ERRORNUM_STRING_SIZE];
    NSString* exceptionReason = nil;

    memset(errorNumString, 0, ERRORNUM_STRING_SIZE);

    DszCLibErrorNumGetMessage(
        errorNum,
        errorNumString, ERRORNUM_STRING_SIZE,
        NULL);

    exceptionReason = [[NSString alloc] initWithUTF8String: errorNumString];

    self = [super initWithName: @"OLException" reason: exceptionReason userInfo: nil];
    return (self);
}

@end /* implementation OLException */

@implementation OLLibrary

+ (BOOL) initialize
{
    DszCLibErrorNum errorNum = DszCLibLibraryInitialize();
    return ((errorNum == DSZ_CLIB_ERRORNUM_NO_ERROR) ? YES : NO);
}

+ (void) uninitialize
{
    DszCLibLibraryUninitialize();
    return;
}

+ (NSString*) getVersionString
{
    size_t const VERSION_STRING_SIZE = 16;
    char versionString[VERSION_STRING_SIZE];
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    memset(versionString, 0, VERSION_STRING_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionString(
        versionString, VERSION_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: versionString]);
}

+ (NSUInteger) getVersionMajor
{
    size_t versionMajor = 0;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    cLibErrorNum = DszCLibLibraryGetVersionMajor(&versionMajor);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ((NSUInteger) versionMajor);
}

+ (NSUInteger) getVersionMinor
{
    size_t versionMinor = 0;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    cLibErrorNum = DszCLibLibraryGetVersionMinor(&versionMinor);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ((NSUInteger) versionMinor);
}

+ (NSUInteger) getVersionPatch
{
    size_t versionPatch = 0;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    cLibErrorNum = DszCLibLibraryGetVersionPatch(&versionPatch);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ((NSUInteger) versionPatch);
}

+ (NSString*) getVersionExtra
{
    size_t const VERSION_EXTRA_STRING_SIZE = 16;
    char versionExtraString[VERSION_EXTRA_STRING_SIZE];
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    memset(versionExtraString, 0, VERSION_EXTRA_STRING_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionExtra(
        versionExtraString, VERSION_EXTRA_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: versionExtraString]);
}

@end /* implementation OLLibrary */

@implementation OLAddress

- (instancetype) initWithStreetNum: (int) streetNum
    street:(NSString*) street
    city:(NSString*) city
    province:(NSString*) province
    zipCode:(NSString*) zipCode
    country:(NSString*) country
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    self = [super init];
    if (self == nil)
        @throw ([[NSException alloc] initWithName: @"NSException" reason: @"Failed to create address." userInfo: nil]);

    m_impl = DSZ_CLIB_ADDRESS_INVALID;

    cLibErrorNum = DszCLibAddressCreate(
        streetNum,
        [street UTF8String],
        [city UTF8String],
        [province UTF8String],
        [country UTF8String],
        [zipCode UTF8String],
        &m_impl);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return (self);
}

- (void) dealloc
{
    if (m_impl != DSZ_CLIB_ADDRESS_INVALID) {
        DszCLibAddressDestroy(m_impl);
        m_impl = DSZ_CLIB_ADDRESS_INVALID;
    }

    return;
}

- (int) streetNum
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    int streetNum = 0;

    cLibErrorNum = DszCLibAddressGetStreetNum(m_impl, &streetNum);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return (streetNum);
}

- (NSString*) street
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const STREET_STRING_SIZE = 16;
    char streetString[STREET_STRING_SIZE];

    memset(streetString, 0, STREET_STRING_SIZE);

    cLibErrorNum = DszCLibAddressGetStreet(
        m_impl,
        streetString, STREET_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: streetString]);
}

- (NSString*) city
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const CITY_STRING_SIZE = 16;
    char cityString[CITY_STRING_SIZE];

    memset(cityString, 0, CITY_STRING_SIZE);

    cLibErrorNum = DszCLibAddressGetCity(
        m_impl,
        cityString, CITY_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: cityString]);
}

- (NSString*) province
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const PROVINCE_STRING_SIZE = 16;
    char provinceString[PROVINCE_STRING_SIZE];

    memset(provinceString, 0, PROVINCE_STRING_SIZE);

    cLibErrorNum = DszCLibAddressGetProvince(
        m_impl,
        provinceString, PROVINCE_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: provinceString]);
}

- (NSString*) zipCode
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const ZIP_CODE_STRING_SIZE = 16;
    char zipCodeString[ZIP_CODE_STRING_SIZE];

    memset(zipCodeString, 0, ZIP_CODE_STRING_SIZE);

    cLibErrorNum = DszCLibAddressGetProvince(
        m_impl,
        zipCodeString, ZIP_CODE_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: zipCodeString]);
}

- (NSString*) country
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const COUNTRY_STRING_SIZE = 16;
    char countryString[COUNTRY_STRING_SIZE];

    memset(countryString, 0, COUNTRY_STRING_SIZE);

    cLibErrorNum = DszCLibAddressGetProvince(
        m_impl,
        countryString, COUNTRY_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: countryString]);
}

- (NSString*) toString
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const ADDRESS_STRING_SIZE = 80;
    char addressString[ADDRESS_STRING_SIZE];

    memset(addressString, 0, ADDRESS_STRING_SIZE);

    cLibErrorNum = DszCLibAddressToString(
        m_impl,
        addressString, ADDRESS_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: addressString]);
}

- (instancetype) initWithImpl: (DszCLibAddress) impl
{
    self = [super init];
    if (self == nil)
        @throw ([[NSException alloc] initWithName: @"NSException" reason: @"Failed to create address." userInfo: nil]);

    if (impl == DSZ_CLIB_ADDRESS_INVALID)
        @throw ([[NSException alloc] initWithName: @"NSException" reason: @"Failed to create address." userInfo: nil]);

    m_impl = impl;

    return (self);
}

- (DszCLibAddress) getImpl
{
    return (m_impl);
}

@end /* implementation OLAddress */

@implementation OLPerson

- (instancetype) initWithLastName: (NSString*) lastName
    firstName: (NSString*) firstName
    age: (int) age
    address: (OLAddress*) address
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    self = [super init];
    if (self == nil)
        @throw ([[NSException alloc] initWithName: @"NSException" reason: @"Failed to create person." userInfo: nil]);

    m_impl = DSZ_CLIB_PERSON_INVALID;

    cLibErrorNum = DszCLibPersonCreate(
        [lastName UTF8String],
        [firstName UTF8String],
        age,
        [address getImpl],
        &m_impl);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return (self);
}

- (void) dealloc
{
    if (m_impl != DSZ_CLIB_PERSON_INVALID) {
        DszCLibPersonDestroy(m_impl);
        m_impl = DSZ_CLIB_PERSON_INVALID;
    }

    return;
}

- (NSString*) lastName
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const LAST_NAME_STRING_SIZE = 24;
    char lastNameString[LAST_NAME_STRING_SIZE];

    memset(lastNameString, 0, LAST_NAME_STRING_SIZE);

    cLibErrorNum = DszCLibPersonGetLastName(
        m_impl,
        lastNameString, LAST_NAME_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: lastNameString]);
}

- (NSString*) firstName
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const FIRST_NAME_STRING_SIZE = 24;
    char firstNameString[FIRST_NAME_STRING_SIZE];

    memset(firstNameString, 0, FIRST_NAME_STRING_SIZE);

    cLibErrorNum = DszCLibPersonGetFirstName(
        m_impl,
        firstNameString, FIRST_NAME_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: firstNameString]);
}

- (int) age
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    int age = 0;

    cLibErrorNum = DszCLibPersonGetAge(
        m_impl,
        &age);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return (age);
}

- (OLAddress*) address
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;

    cLibErrorNum = DszCLibPersonGetAddress(
        m_impl,
        &cLibAddress);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[OLAddress alloc] initWithImpl: cLibAddress]);
}

- (NSString*) toString
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const PERSON_STRING_SIZE = 160;
    char personString[PERSON_STRING_SIZE];

    memset(personString, 0, PERSON_STRING_SIZE);

    cLibErrorNum = DszCLibPersonToString(
        m_impl,
        personString, PERSON_STRING_SIZE,
        NULL);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return ([[NSString alloc] initWithUTF8String: personString]);
}

@end /* implementation OLPerson */

@implementation OLPrinter

- (instancetype) initWithGenerator: (id<OLGenerator>) generator
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszCLibGenerator cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;

    self = [super init];
    if (self == nil)
        @throw ([[NSException alloc] initWithName: @"NSException" reason: @"Failed to create printer." userInfo: nil]);

    m_generator = generator;

    cLibErrorNum = DszCLibGeneratorCreate(
        (DszCLibGenerateIntFunction) &OLCorePrinterGenerateIntRedirect,
        (DszCLibGenerateStringFunction) &OLCorePrinterGenerateStringRedirect,
        &cLibGenerator);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    cLibErrorNum = DszCLibPrinterCreate(
        cLibGenerator,
        &m_impl);
    /* DszCLibPrinterCreate will take ownership of cLibGenerator */

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return (self);
}

- (void) dealloc
{
    if (m_generator != nil) {
        m_generator = nil; /* let ARC take care of deallocation */
    }

    return;
}

- (void) printInt
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    id<OLGenerator> generator = [self getGenerator];

    cLibErrorNum = DszCLibPrinterPrintIntWithUserData(
        m_impl,
        (__bridge void*) generator);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return;
}

- (void) printString
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    id<OLGenerator> generator = [self getGenerator];

    cLibErrorNum = DszCLibPrinterPrintStringWithUserData(
        m_impl,
        (__bridge void*) generator);

    DSZ_OBJCLIBCORE_API_CHECK(cLibErrorNum);

    return;
}

- (id<OLGenerator>) getGenerator
{
    return (m_generator);
}

@end /* implementation OLPrinter */
