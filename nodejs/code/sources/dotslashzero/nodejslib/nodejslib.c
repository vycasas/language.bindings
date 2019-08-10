#include <dotslashzero/nodejslib/nodejslib.h>

#include <dotslashzero/clib/clib.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#error Please use pure C compiler for this file.
#endif /* defined(__cplusplus) */

NAPI_MODULE(nodejslib_native, DszNodeJsLibInitializeModule)

#if defined(_MSC_VER)
#undef strncpy
#define strncpy(buffer, source, bufferSize) strncpy_s(buffer, bufferSize, source, _TRUNCATE)
#endif /* defined(_MSC_VER) */

#define DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum) \
    if (cLibErrorNum != DSZ_CLIB_ERROR_NUM_NO_ERROR) { \
        char errorMessage[DSZ_NODEJSLIBCORE_ERROR_MESSAGE_SIZE]; \
        memset(errorMessage, 0, DSZ_NODEJSLIBCORE_ERROR_MESSAGE_SIZE); \
        DszCLibErrorNumGetMessage( \
            cLibErrorNum, \
            errorMessage, DSZ_NODEJSLIBCORE_ERROR_MESSAGE_SIZE, \
            NULL); \
        napi_throw_error( \
            env, \
            NULL, \
            errorMessage); \
        return (NULL); \
    }

#define DSZ_NODEJSLIBCORE_CHECK_STATUS(env, functionName, status) \
    if (status != napi_ok) { \
        napi_throw_error( \
            env, \
            NULL, \
            functionName " failed"); \
        return (NULL); \
    }

#if defined(_MSC_VER)
#define DSZ_NODEJSLIBCORE_ERROR_MESSAGE_SIZE 16
#define DSZ_NODEJSLIBCORE_LIBRARY_VERSION_STRING_SIZE 16
#define DSZ_NODEJSLIBCORE_LIBRARY_VERSION_EXTRA_SIZE 16
#define DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE 16
#define DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE 16
#define DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE 16
#define DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE 16
#define DSZ_NODEJSLIBCORE_ADDRESS_COUNTRY_SIZE 16
#define DSZ_NODEJSLIBCORE_ADDRESS_STRING_SIZE 80
#define DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE 16
#define DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE 16
#define DSZ_NODEJSLIBCORE_PERSON_STRING_SIZE 160
#else /* defined(_MSC_VER) */
static size_t const DSZ_NODEJSLIBCORE_ERROR_MESSAGE_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_LIBRARY_VERSION_STRING_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_LIBRARY_VERSION_EXTRA_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_ADDRESS_COUNTRY_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_ADDRESS_STRING_SIZE = 80;
static size_t const DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE = 16;
static size_t const DSZ_NODEJSLIBCORE_PERSON_STRING_SIZE = 160;
#endif /* defined(_MSC_VER) */

#define DSZ_NODEJSLIBCORE_PROPERTYDESCRIPTORS_COUNT 24

static napi_property_descriptor const DSZ_NODEJSLIBCORE_PROPERTYDESCRIPTORS[DSZ_NODEJSLIBCORE_PROPERTYDESCRIPTORS_COUNT] =
{
    { "libraryInitialize", NULL, DszNodeJsLibLibraryInitialize, NULL, NULL, NULL, napi_default, NULL },
    { "libraryUninitialize", NULL, DszNodeJsLibLibraryUninitialize, NULL, NULL, NULL, napi_default, NULL },
    { "libraryGetVersionString", NULL, DszNodeJsLibLibraryGetVersionString, NULL, NULL, NULL, napi_default, NULL },
    { "libraryGetVersionMajor", NULL, DszNodeJsLibLibraryGetVersionMajor, NULL, NULL, NULL, napi_default, NULL },
    { "libraryGetVersionMinor", NULL, DszNodeJsLibLibraryGetVersionMinor, NULL, NULL, NULL, napi_default, NULL },
    { "libraryGetVersionPatch", NULL, DszNodeJsLibLibraryGetVersionPatch, NULL, NULL, NULL, napi_default, NULL },
    { "libraryGetVersionExtra", NULL, DszNodeJsLibLibraryGetVersionExtra, NULL, NULL, NULL, napi_default, NULL },
    { "addressCreate", NULL, DszNodeJsLibAddressCreate, NULL, NULL, NULL, napi_default, NULL },
    { "addressGetStreetNum", NULL, DszNodeJsLibAddressGetStreetNum, NULL, NULL, NULL, napi_default, NULL },
    { "addressGetStreet", NULL, DszNodeJsLibAddressGetStreet, NULL, NULL, NULL, napi_default, NULL },
    { "addressGetCity", NULL, DszNodeJsLibAddressGetCity, NULL, NULL, NULL, napi_default, NULL },
    { "addressGetProvince", NULL, DszNodeJsLibAddressGetProvince, NULL, NULL, NULL, napi_default, NULL },
    { "addressGetZipCode", NULL, DszNodeJsLibAddressGetZipCode, NULL, NULL, NULL, napi_default, NULL },
    { "addressGetCountry", NULL, DszNodeJsLibAddressGetCountry, NULL, NULL, NULL, napi_default, NULL },
    { "addressToString", NULL, DszNodeJsLibAddressToString, NULL, NULL, NULL, napi_default, NULL },
    { "personCreate", NULL, DszNodeJsLibPersonCreate, NULL, NULL, NULL, napi_default, NULL },
    { "personGetLastName", NULL, DszNodeJsLibPersonGetLastName, NULL, NULL, NULL, napi_default, NULL },
    { "personGetFirstName", NULL, DszNodeJsLibPersonGetFirstName, NULL, NULL, NULL, napi_default, NULL },
    { "personGetAge", NULL, DszNodeJsLibPersonGetAge, NULL, NULL, NULL, napi_default, NULL },
    { "personGetAddress", NULL, DszNodeJsLibPersonGetAddress, NULL, NULL, NULL, napi_default, NULL },
    { "personToString", NULL, DszNodeJsLibPersonToString, NULL, NULL, NULL, napi_default, NULL },
    { "printerCreate", NULL, DszNodeJsLibPrinterCreate, NULL, NULL, NULL, napi_default, NULL },
    { "printerPrintInt", NULL, DszNodeJsLibPrinterPrintInt, NULL, NULL, NULL, napi_default, NULL },
    { "printerPrintString", NULL, DszNodeJsLibPrinterPrintString, NULL, NULL, NULL, napi_default, NULL }
};

static void DSZ_NODEJSLIB_CALLING_CONVENTION DszNodeJSLibCoreFinalizeAddress(
    napi_env env,
    void* finalizeData,
    void* finalizeHint)
{
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;

    (void) env;
    (void) finalizeHint;

    if (finalizeData == NULL)
        return;

    cLibAddress = (DszCLibAddress) finalizeData;

    if (cLibAddress != DSZ_CLIB_ADDRESS_INVALID)
        DszCLibAddressDestroy(cLibAddress);

    return;
}

static void DSZ_NODEJSLIB_CALLING_CONVENTION DszNodeJSLibCoreFinalizePerson(
    napi_env env,
    void* finalizeData,
    void* finalizeHint)
{
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;

    (void) env;
    (void) finalizeHint;

    if (finalizeData == NULL)
        return;

    cLibPerson = (DszCLibPerson) finalizeData;

    if (cLibPerson != DSZ_CLIB_PERSON_INVALID)
        DszCLibPersonDestroy(cLibPerson);

    return;
}

static void DSZ_NODEJSLIB_CALLING_CONVENTION DszNodeJSLibCoreFinalizePrinter(
    napi_env env,
    void* finalizeData,
    void* finalizeHint)
{
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;

    (void) env;
    (void) finalizeHint;

    if (finalizeData == NULL)
        return;

    cLibPrinter = (DszCLibPrinter) finalizeData;

    if (cLibPrinter != DSZ_CLIB_PRINTER_INVALID)
        DszCLibPrinterDestroy(cLibPrinter);

    return;
}

struct DszNodeJsLibCoreCallbackData_
{
    napi_env Env;
    napi_value Generator;
};

typedef struct DszNodeJsLibCoreCallbackData_ DszNodeJsLibCoreCallbackData;

static DszCLibErrorNum DSZ_NODEJSLIB_CALLING_CONVENTION DszNodeJsLibCoreGenerateIntRedirect(
    int data,
    int* pInt,
    void* pUserData)
{
    DszNodeJsLibCoreCallbackData* pCallbackData = (DszNodeJsLibCoreCallbackData*) pUserData;
    napi_status status = napi_ok;
    napi_value generateIntFn = NULL;
    int const ARG_COUNT = 1;
    napi_value args[] = { NULL };
    napi_value result = NULL;
    int32_t intResult = 0;

    if (pCallbackData == NULL)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    if ((pCallbackData->Env == NULL) || (pCallbackData->Generator == NULL))
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    if (pInt == NULL)
        return (DSZ_CLIB_ERROR_NUM_NO_ERROR);

    status = napi_create_int32(
        pCallbackData->Env,
        (int32_t) data,
        &(args[0]));

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    status = napi_get_named_property(
        pCallbackData->Env,
        pCallbackData->Generator,
        "generateInt",
        &generateIntFn);

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    status = napi_call_function(
        pCallbackData->Env,
        pCallbackData->Generator,
        generateIntFn,
        ARG_COUNT,
        args,
        &result);

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    status = napi_get_value_int32(
        pCallbackData->Env,
        result,
        &intResult);

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    *pInt = (int) intResult;

    return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
}

static DszCLibErrorNum DSZ_NODEJSLIB_CALLING_CONVENTION DszNodeJsLibCoreGenerateStringRedirect(
    int data,
    char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData)
{
    DszNodeJsLibCoreCallbackData* pCallbackData = (DszNodeJsLibCoreCallbackData*) pUserData;
    napi_status status = napi_ok;
    napi_value generateStringFn = NULL;
    int const ARG_COUNT = 1;
    napi_value args[] = { NULL };
    napi_value result = NULL;
    size_t numChars = 0;

    if (pCallbackData == NULL)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    if ((pCallbackData->Env == NULL) || (pCallbackData->Generator == NULL))
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    status = napi_create_int32(
        pCallbackData->Env,
        (int32_t) data,
        &(args[0]));

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    status = napi_get_named_property(
        pCallbackData->Env,
        pCallbackData->Generator,
        "generateString",
        &generateStringFn);

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    status = napi_call_function(
        pCallbackData->Env,
        pCallbackData->Generator,
        generateStringFn,
        ARG_COUNT,
        args,
        &result);

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    status = napi_get_value_string_utf8(
        pCallbackData->Env,
        result,
        NULL,
        0,
        &numChars);

    if (status != napi_ok)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    if ((pString != NULL) && (stringSize > 0)) {
        char* pStringResult = NULL;

        pStringResult = (char*) malloc((numChars + numChars) * sizeof (char));
        if (pStringResult == NULL)
            return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

        status = napi_get_value_string_utf8(
            pCallbackData->Env,
            result,
            pStringResult,
            numChars,
            &numChars);

        if (status == napi_ok) {
            strncpy(pString, pStringResult, stringSize);
            pString[stringSize - 1] = '\0';
            numChars = strlen(pString);
        }

        free(pStringResult);

        if (status != napi_ok)
            return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibInitializeModule(
    napi_env env,
    napi_value exports)
{
    napi_status status = napi_ok;

    status = napi_define_properties(
        env,
        exports,
        DSZ_NODEJSLIBCORE_PROPERTYDESCRIPTORS_COUNT,
        DSZ_NODEJSLIBCORE_PROPERTYDESCRIPTORS);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_define_properties", status);

    return (exports);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryInitialize(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    napi_status status = napi_ok;
    napi_value initOk = NULL;

    (void) callbackInfo;

    cLibErrorNum = DszCLibLibraryInitialize();

    status = napi_get_boolean(
        env,
        (cLibErrorNum == DSZ_CLIB_ERROR_NUM_NO_ERROR),
        &initOk);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_boolean", status);

    return (initOk);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryUninitialize(
    napi_env env,
    napi_callback_info callbackInfo)
{
    (void) env;
    (void) callbackInfo;

    DszCLibLibraryUninitialize();

    return (NULL);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionString(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    char cLibVersionString[DSZ_NODEJSLIBCORE_LIBRARY_VERSION_STRING_SIZE];
    napi_status status = napi_ok;
    napi_value versionString = NULL;

    (void) callbackInfo;

    memset(cLibVersionString, 0, DSZ_NODEJSLIBCORE_LIBRARY_VERSION_STRING_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionString(
        cLibVersionString, DSZ_NODEJSLIBCORE_LIBRARY_VERSION_STRING_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibVersionString,
        NAPI_AUTO_LENGTH,
        &versionString);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (versionString);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionMajor(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    size_t cLibVersionMajor = 0;
    napi_status status = napi_ok;
    napi_value versionMajor = NULL;

    (void) callbackInfo;

    cLibErrorNum = DszCLibLibraryGetVersionMajor(&cLibVersionMajor);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_int64(
        env,
        (int64_t) cLibVersionMajor,
        &versionMajor);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_int64", status);

    return (versionMajor);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionMinor(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    size_t cLibVersionMinor = 0;
    napi_status status = napi_ok;
    napi_value versionMinor = NULL;

    (void) callbackInfo;

    cLibErrorNum = DszCLibLibraryGetVersionMinor(&cLibVersionMinor);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_int64(
        env,
        (int64_t) cLibVersionMinor,
        &versionMinor);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_int64", status);

    return (versionMinor);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionPatch(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    size_t cLibVersionPatch = 0;
    napi_status status = napi_ok;
    napi_value versionPatch = NULL;

    (void) callbackInfo;

    cLibErrorNum = DszCLibLibraryGetVersionPatch(&cLibVersionPatch);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_int64(
        env,
        (int64_t) cLibVersionPatch,
        &versionPatch);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_int64", status);

    return (versionPatch);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionExtra(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    char cLibVersionExtra[DSZ_NODEJSLIBCORE_LIBRARY_VERSION_EXTRA_SIZE];
    napi_status status = napi_ok;
    napi_value versionExtra = NULL;

    (void) callbackInfo;

    memset(cLibVersionExtra, 0, DSZ_NODEJSLIBCORE_LIBRARY_VERSION_EXTRA_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionString(
        cLibVersionExtra, DSZ_NODEJSLIBCORE_LIBRARY_VERSION_EXTRA_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibVersionExtra,
        NAPI_AUTO_LENGTH,
        &versionExtra);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (versionExtra);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressCreate(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    int streetNum = 0;
    char street[DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE];
    char city[DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE];
    char province[DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE];
    char zipCode[DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE];
    char country[DSZ_NODEJSLIBCORE_ADDRESS_COUNTRY_SIZE];
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value argProperty = NULL;
    napi_value addressImpl = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "streetNum",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_int32(
        env,
        argProperty,
        (int32_t*) &streetNum);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_int32", status);

    status = napi_get_named_property(
        env,
        args,
        "street",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    memset(street, 0, DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE);

    status = napi_get_value_string_utf8(
        env,
        argProperty,
        street,
        DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_string_utf8", status);

    status = napi_get_named_property(
        env,
        args,
        "city",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    memset(city, 0, DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE);

    status = napi_get_value_string_utf8(
        env,
        argProperty,
        city,
        DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_string_utf8", status);

    status = napi_get_named_property(
        env,
        args,
        "province",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    memset(province, 0, DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE);

    status = napi_get_value_string_utf8(
        env,
        argProperty,
        province,
        DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_string_utf8", status);

    status = napi_get_named_property(
        env,
        args,
        "zipCode",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    memset(zipCode, 0, DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE);

    status = napi_get_value_string_utf8(
        env,
        argProperty,
        zipCode,
        DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_string_utf8", status);

    status = napi_get_named_property(
        env,
        args,
        "country",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    memset(country, 0, DSZ_NODEJSLIBCORE_ADDRESS_COUNTRY_SIZE);

    status = napi_get_value_string_utf8(
        env,
        argProperty,
        country,
        DSZ_NODEJSLIBCORE_ADDRESS_COUNTRY_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_string_utf8", status);

    cLibErrorNum = DszCLibAddressCreate(
        streetNum,
        street,
        city,
        province,
        zipCode,
        country,
        &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_external(
        env,
        (void*) cLibAddress,
        DszNodeJSLibCoreFinalizeAddress,
        NULL,
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_external", status);

    return (addressImpl);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetStreetNum(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    int cLibStreetNum = 0;
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value addressImpl = NULL;
    napi_value streetNum = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        addressImpl,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    cLibErrorNum = DszCLibAddressGetStreetNum(
        cLibAddress,
        &cLibStreetNum);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_int32(
        env,
        (int32_t) cLibStreetNum,
        &streetNum);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_int32", status);

    return (streetNum);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetStreet(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char cLibStreet[DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value addressImpl = NULL;
    napi_value street = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        addressImpl,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibStreet, 0, DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE);

    cLibErrorNum = DszCLibAddressGetStreet(
        cLibAddress,
        cLibStreet, DSZ_NODEJSLIBCORE_ADDRESS_STREET_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibStreet,
        NAPI_AUTO_LENGTH,
        &street);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (street);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetCity(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char cLibCity[DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value addressImpl = NULL;
    napi_value city = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        addressImpl,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibCity, 0, DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE);

    cLibErrorNum = DszCLibAddressGetCity(
        cLibAddress,
        cLibCity, DSZ_NODEJSLIBCORE_ADDRESS_CITY_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibCity,
        NAPI_AUTO_LENGTH,
        &city);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (city);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetProvince(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char cLibProvince[DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value addressImpl = NULL;
    napi_value province = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        addressImpl,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibProvince, 0, DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE);

    cLibErrorNum = DszCLibAddressGetProvince(
        cLibAddress,
        cLibProvince, DSZ_NODEJSLIBCORE_ADDRESS_PROVINCE_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibProvince,
        NAPI_AUTO_LENGTH,
        &province);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (province);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetZipCode(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char cLibZipCode[DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value addressImpl = NULL;
    napi_value zipCode = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        addressImpl,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibZipCode, 0, DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE);

    cLibErrorNum = DszCLibAddressGetZipCode(
        cLibAddress,
        cLibZipCode, DSZ_NODEJSLIBCORE_ADDRESS_ZIP_CODE_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibZipCode,
        NAPI_AUTO_LENGTH,
        &zipCode);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (zipCode);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetCountry(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char cLibCountry[DSZ_NODEJSLIBCORE_ADDRESS_COUNTRY_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value addressImpl = NULL;
    napi_value country = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        addressImpl,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibCountry, 0, DSZ_NODEJSLIBCORE_ADDRESS_STRING_SIZE);

    cLibErrorNum = DszCLibAddressGetCountry(
        cLibAddress,
        cLibCountry, DSZ_NODEJSLIBCORE_ADDRESS_COUNTRY_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibCountry,
        NAPI_AUTO_LENGTH,
        &country);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (country);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressToString(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char cLibAddressString[DSZ_NODEJSLIBCORE_ADDRESS_STRING_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value addressImpl = NULL;
    napi_value addressString = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        addressImpl,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibAddressString, 0, DSZ_NODEJSLIBCORE_ADDRESS_STRING_SIZE);

    cLibErrorNum = DszCLibAddressToString(
        cLibAddress,
        cLibAddressString, DSZ_NODEJSLIBCORE_ADDRESS_STRING_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibAddressString,
        NAPI_AUTO_LENGTH,
        &addressString);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (addressString);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonCreate(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    int age = 0;
    char lastName[DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE];
    char firstName[DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE];
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value argProperty = NULL;
    napi_value personImpl = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "lastName",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    memset(lastName, 0, DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE);

    status = napi_get_value_string_utf8(
        env,
        argProperty,
        lastName,
        DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_string_utf8", status);

    status = napi_get_named_property(
        env,
        args,
        "firstName",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    memset(firstName, 0, DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE);

    status = napi_get_value_string_utf8(
        env,
        argProperty,
        firstName,
        DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_string_utf8", status);

    status = napi_get_named_property(
        env,
        args,
        "age",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_int32(
        env,
        argProperty,
        (int32_t*) &age);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_int32", status);

    status = napi_get_named_property(
        env,
        args,
        "addressImpl",
        &argProperty);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        argProperty,
        (void**) &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    cLibErrorNum = DszCLibPersonCreate(
        lastName,
        firstName,
        age,
        cLibAddress,
        &cLibPerson);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_external(
        env,
        (void*) cLibPerson,
        DszNodeJSLibCoreFinalizePerson,
        NULL,
        &personImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_external", status);

    return (personImpl);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetLastName(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    char cLibLastName[DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value personImpl = NULL;
    napi_value lastName = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &personImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        personImpl,
        (void**) &cLibPerson);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibLastName, 0, DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE);

    cLibErrorNum = DszCLibPersonGetLastName(
        cLibPerson,
        cLibLastName, DSZ_NODEJSLIBCORE_PERSON_LAST_NAME_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibLastName,
        NAPI_AUTO_LENGTH,
        &lastName);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (lastName);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetFirstName(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    char cLibFirstName[DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value personImpl = NULL;
    napi_value firstName = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &personImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        personImpl,
        (void**) &cLibPerson);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibFirstName, 0, DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE);

    cLibErrorNum = DszCLibPersonGetFirstName(
        cLibPerson,
        cLibFirstName, DSZ_NODEJSLIBCORE_PERSON_FIRST_NAME_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibFirstName,
        NAPI_AUTO_LENGTH,
        &firstName);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (firstName);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetAge(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    int cLibAge = 0;
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value personImpl = NULL;
    napi_value age = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &personImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        personImpl,
        (void**) &cLibPerson);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    cLibErrorNum = DszCLibPersonGetAge(
        cLibPerson,
        &cLibAge);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_int32(
        env,
        (int32_t) cLibAge,
        &age);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_int32", status);

    return (age);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetAddress(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value personImpl = NULL;
    napi_value addressImpl = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &personImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        personImpl,
        (void**) &cLibPerson);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    cLibErrorNum = DszCLibPersonGetAddress(
        cLibPerson,
        &cLibAddress);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_external(
        env,
        (void*) cLibAddress,
        DszNodeJSLibCoreFinalizeAddress,
        NULL,
        &addressImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_external", status);

    return (addressImpl);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonToString(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    char cLibPersonString[DSZ_NODEJSLIBCORE_PERSON_STRING_SIZE];
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value personImpl = NULL;
    napi_value personString = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &personImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        personImpl,
        (void**) &cLibPerson);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    memset(cLibPersonString, 0, DSZ_NODEJSLIBCORE_PERSON_STRING_SIZE);

    cLibErrorNum = DszCLibPersonToString(
        cLibPerson,
        cLibPersonString, DSZ_NODEJSLIBCORE_PERSON_STRING_SIZE,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_string_utf8(
        env,
        cLibPersonString,
        NAPI_AUTO_LENGTH,
        &personString);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_string_utf8", status);

    return (personString);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPrinterCreate(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibGenerator cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;
    napi_status status = napi_ok;
    napi_value printerImpl = NULL;

    (void) callbackInfo;

    cLibErrorNum = DszCLibGeneratorCreate(
        &DszNodeJsLibCoreGenerateIntRedirect,
        &DszNodeJsLibCoreGenerateStringRedirect,
        &cLibGenerator);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    cLibErrorNum = DszCLibPrinterCreate(
        cLibGenerator,
        &cLibPrinter);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    status = napi_create_external(
        env,
        (void*) cLibPrinter,
        DszNodeJSLibCoreFinalizePrinter,
        NULL,
        &printerImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_create_external", status);

    return (printerImpl);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPrinterPrintInt(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;
    DszNodeJsLibCoreCallbackData callbackData;
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value printerImpl = NULL;
    napi_value generator = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &printerImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        printerImpl,
        (void**) &cLibPrinter);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    status = napi_get_named_property(
        env,
        args,
        "generator",
        &generator);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    callbackData.Env = env;
    callbackData.Generator = generator;

    cLibErrorNum = DszCLibPrinterPrintIntWithUserData(
        cLibPrinter,
        (void*) &callbackData);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    return (NULL);
}

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPrinterPrintString(
    napi_env env,
    napi_callback_info callbackInfo)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;
    DszNodeJsLibCoreCallbackData callbackData;
    napi_status status = napi_ok;
    size_t numArgs = 1;
    napi_value args = NULL;
    napi_value printerImpl = NULL;
    napi_value generator = NULL;

    status = napi_get_cb_info(
        env,
        callbackInfo,
        &numArgs,
        &args,
        NULL,
        NULL);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_cb_info", status);

    status = napi_get_named_property(
        env,
        args,
        "impl",
        &printerImpl);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    status = napi_get_value_external(
        env,
        printerImpl,
        (void**) &cLibPrinter);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_value_external", status);

    status = napi_get_named_property(
        env,
        args,
        "generator",
        &generator);

    DSZ_NODEJSLIBCORE_CHECK_STATUS(env, "napi_get_named_property", status);

    callbackData.Env = env;
    callbackData.Generator = generator;

    cLibErrorNum = DszCLibPrinterPrintStringWithUserData(
        cLibPrinter,
        (void*) &callbackData);

    DSZ_NODEJSLIBCORE_CHECK_ERROR_NUM(env, cLibErrorNum);

    return (NULL);
}
