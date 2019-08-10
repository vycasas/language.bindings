#include <dotslashzero/pythonlib/pythonlib.h>

#include <dotslashzero/clib/clib.h>

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <stdio.h>

#if defined(__cplusplus)
#error Please use pure C compiler for this file.
#endif /* defined(__cplusplus) */

#if defined(_MSC_VER)
#undef strncpy
#define strncpy(buffer, source, bufferSize) strncpy_s(buffer, bufferSize, source, _TRUNCATE)
#endif /* defined(_MSC_VER) */

/*
// Documentation:
// https://docs.python.org/3.7/extending/extending.html
// https://docs.python.org/3/howto/cporting.html
*/

#if defined(_MSC_VER)
#define DSZ_PYTHONLIBCORE_ERROR_MESSAGE_SIZE 16
#define DSZ_PYTHONLIBCORE_LIBRARY_VERSION_STRING_SIZE 16
#define DSZ_PYTHONLIBCORE_LIBRARY_VERSION_EXTRA_SIZE 16
#define DSZ_PYTHONLIBCORE_ADDRESS_STREET_SIZE 16
#define DSZ_PYTHONLIBCORE_ADDRESS_CITY_SIZE 16
#define DSZ_PYTHONLIBCORE_ADDRESS_PROVINCE_SIZE 16
#define DSZ_PYTHONLIBCORE_ADDRESS_ZIP_CODE_SIZE 16
#define DSZ_PYTHONLIBCORE_ADDRESS_COUNTRY_SIZE 16
#define DSZ_PYTHONLIBCORE_ADDRESS_STRING_SIZE 80
#define DSZ_PYTHONLIBCORE_PERSON_LAST_NAME_SIZE 16
#define DSZ_PYTHONLIBCORE_PERSON_FIRST_NAME_SIZE 16
#define DSZ_PYTHONLIBCORE_PERSON_STRING_SIZE 160
#else /* defined(_MSC_VER) */
static size_t const DSZ_PYTHONLIBCORE_ERROR_MESSAGE_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_LIBRARY_VERSION_STRING_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_LIBRARY_VERSION_EXTRA_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_ADDRESS_STREET_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_ADDRESS_CITY_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_ADDRESS_PROVINCE_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_ADDRESS_ZIP_CODE_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_ADDRESS_COUNTRY_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_ADDRESS_STRING_SIZE = 80;
static size_t const DSZ_PYTHONLIBCORE_PERSON_LAST_NAME_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_PERSON_FIRST_NAME_SIZE = 16;
static size_t const DSZ_PYTHONLIBCORE_PERSON_STRING_SIZE = 160;
#endif /* defined(_MSC_VER) */

#define DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum) \
    if (cLibErrorNum != DSZ_CLIB_ERROR_NUM_NO_ERROR) { \
        char errorMessage[DSZ_PYTHONLIBCORE_ERROR_MESSAGE_SIZE]; \
        memset(errorMessage, 0, DSZ_PYTHONLIBCORE_ERROR_MESSAGE_SIZE); \
        DszCLibErrorNumGetMessage(cLibErrorNum, errorMessage, DSZ_PYTHONLIBCORE_ERROR_MESSAGE_SIZE, NULL); \
        PyErr_SetString(PyExc_RuntimeError, errorMessage); \
        return (NULL); \
    }

/*
// PyDoc strings for descriptions
*/
PyDoc_STRVAR(g_pythonLibModuleDoc, "Native Python library");
PyDoc_STRVAR(g_pythonLibLibraryInitializeDoc, "Initialize the library");
PyDoc_STRVAR(g_pythonLibLibraryUninitializeDoc, "Uninitialize the library");
PyDoc_STRVAR(g_pythonLibLibraryGetVersionStringDoc, "Gets the library version as string");
PyDoc_STRVAR(g_pythonLibLibraryGetVersionMajorDoc, "Gets the major library version");
PyDoc_STRVAR(g_pythonLibLibraryGetVersionMinorDoc, "Gets the minor library version");
PyDoc_STRVAR(g_pythonLibLibraryGetVersionPatchDoc, "Gets the patch library version");
PyDoc_STRVAR(g_pythonLibLibraryGetVersionExtraDoc, "Gets the extra library version");
PyDoc_STRVAR(g_pythonLibAddressCreateDoc, "Creates a new Address");
PyDoc_STRVAR(g_pythonLibAddressDestroyDoc, "Destroys an existing Address");
PyDoc_STRVAR(g_pythonLibAddressGetStreetNumDoc, "Gets the street number");
PyDoc_STRVAR(g_pythonLibAddressGetStreetDoc, "Gets the street");
PyDoc_STRVAR(g_pythonLibAddressGetCityDoc, "Gets the city");
PyDoc_STRVAR(g_pythonLibAddressGetProvinceDoc, "Gets the province");
PyDoc_STRVAR(g_pythonLibAddressGetZipCodeDoc, "Gets the zip code");
PyDoc_STRVAR(g_pythonLibAddressGetCountryDoc, "Gets the country");
PyDoc_STRVAR(g_pythonLibAddressToStringDoc, "Gets a string representation of an existing Address");
PyDoc_STRVAR(g_pythonLibPersonCreateDoc, "Creates a new Person");
PyDoc_STRVAR(g_pythonLibPersonDestroyDoc, "Destroys an existing Person");
PyDoc_STRVAR(g_pythonLibPersonGetLastNameDoc, "Gets the last name");
PyDoc_STRVAR(g_pythonLibPersonGetFirstNameDoc, "Gets the first name");
PyDoc_STRVAR(g_pythonLibPersonGetAgeDoc, "Gets the age");
PyDoc_STRVAR(g_pythonLibPersonGetAddressDoc, "Gets the address as address_impl");
PyDoc_STRVAR(g_pythonLibPersonToStringDoc, "Gets a string representation of an existing Person");
PyDoc_STRVAR(g_pythonLibPrinterCreateDoc, "Creates a new Printer");
PyDoc_STRVAR(g_pythonLibPrinterDestroyDoc, "Destroyes an existing Printer");
PyDoc_STRVAR(g_pythonLibPrinterPrintIntDoc, "Prints an integer fed to the generator");
PyDoc_STRVAR(g_pythonLibPrinterPrintStringDoc, "Prints a string fed to the generator");

static struct PyMethodDef g_pythonLibMethods[] =
{
    /* Library */
    { "library_initialize", DszPythonLibLibraryInitialize, METH_NOARGS, g_pythonLibLibraryInitializeDoc },
    { "library_uninitialize", DszPythonLibLibraryUninitialize, METH_NOARGS, g_pythonLibLibraryUninitializeDoc },
    { "library_get_version_string", DszPythonLibLibraryGetVersionString, METH_NOARGS, g_pythonLibLibraryGetVersionStringDoc },
    { "library_get_version_major", DszPythonLibLibraryGetVersionMajor, METH_NOARGS, g_pythonLibLibraryGetVersionMajorDoc },
    { "library_get_version_minor", DszPythonLibLibraryGetVersionMinor, METH_NOARGS, g_pythonLibLibraryGetVersionMinorDoc },
    { "library_get_version_patch", DszPythonLibLibraryGetVersionPatch, METH_NOARGS, g_pythonLibLibraryGetVersionPatchDoc },
    { "library_get_version_extra", DszPythonLibLibraryGetVersionExtra, METH_NOARGS, g_pythonLibLibraryGetVersionExtraDoc },

    /* Address */
    { "address_create", (PyCFunction) &DszPythonLibAddressCreate, METH_VARARGS | METH_KEYWORDS,  g_pythonLibAddressCreateDoc },
    { "address_destroy", (PyCFunction) &DszPythonLibAddressDestroy, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressDestroyDoc },
    { "address_get_street_num", (PyCFunction) &DszPythonLibAddressGetStreetNum, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressGetStreetNumDoc },
    { "address_get_street", (PyCFunction) &DszPythonLibAddressGetStreet, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressGetStreetDoc },
    { "address_get_city", (PyCFunction) &DszPythonLibAddressGetCity, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressGetCityDoc },
    { "address_get_province", (PyCFunction) &DszPythonLibAddressGetProvince, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressGetProvinceDoc },
    { "address_get_zip_code", (PyCFunction) &DszPythonLibAddressGetZipCode, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressGetZipCodeDoc },
    { "address_get_country", (PyCFunction) &DszPythonLibAddressGetCountry, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressGetCountryDoc },
    { "address_to_string", (PyCFunction) &DszPythonLibAddressToString, METH_VARARGS | METH_KEYWORDS, g_pythonLibAddressToStringDoc },

    /* Person */
    { "person_create", (PyCFunction) &DszPythonLibPersonCreate, METH_VARARGS | METH_KEYWORDS,  g_pythonLibPersonCreateDoc },
    { "person_destroy", (PyCFunction) &DszPythonLibPersonDestroy, METH_VARARGS | METH_KEYWORDS, g_pythonLibPersonDestroyDoc },
    { "person_get_last_name", (PyCFunction) &DszPythonLibPersonGetLastName, METH_VARARGS | METH_KEYWORDS, g_pythonLibPersonGetLastNameDoc },
    { "person_get_first_name", (PyCFunction) &DszPythonLibPersonGetFirstName, METH_VARARGS | METH_KEYWORDS, g_pythonLibPersonGetFirstNameDoc },
    { "person_get_age", (PyCFunction) &DszPythonLibPersonGetAge, METH_VARARGS | METH_KEYWORDS, g_pythonLibPersonGetAgeDoc },
    { "person_get_address", (PyCFunction) &DszPythonLibPersonGetAddress, METH_VARARGS | METH_KEYWORDS, g_pythonLibPersonGetAddressDoc },
    { "person_to_string", (PyCFunction) &DszPythonLibPersonToString, METH_VARARGS | METH_KEYWORDS, g_pythonLibPersonToStringDoc },

    /* Printer */
    { "printer_create", (PyCFunction) &DszPythonLibPrinterCreate, METH_VARARGS | METH_KEYWORDS,  g_pythonLibPrinterCreateDoc },
    { "printer_destroy", (PyCFunction) &DszPythonLibPrinterDestroy, METH_VARARGS | METH_KEYWORDS,  g_pythonLibPrinterDestroyDoc },
    { "printer_print_int", (PyCFunction) &DszPythonLibPrinterPrintInt, METH_VARARGS | METH_KEYWORDS,  g_pythonLibPrinterPrintIntDoc },
    { "printer_print_string", (PyCFunction) &DszPythonLibPrinterPrintString, METH_VARARGS | METH_KEYWORDS,  g_pythonLibPrinterPrintStringDoc },

    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef g_pythonLibModule =
{
    PyModuleDef_HEAD_INIT,
    "pythonlib_native",
    g_pythonLibModuleDoc,
    -1,
    g_pythonLibMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

static inline
intptr_t DszPythonLibCoreGetImpl(PyObject* pArgs, PyObject* pKwArgs)
{
    char const* const KEYWORDS_FORMAT = "n";

    char const* const KEYWORDS[] = {
        "impl",
        NULL };

    Py_ssize_t implAddress = 0;
    int pyOpResult = 0;

    assert(pArgs != NULL);
    assert(pKwArgs != NULL);

    pyOpResult = PyArg_ParseTupleAndKeywords(
        pArgs,
        pKwArgs,
        KEYWORDS_FORMAT,
        (char**) KEYWORDS,
        &implAddress);

    if (pyOpResult == 0)
        return (0);

    return ((intptr_t) implAddress);
}

static inline
DszCLibAddress DszPythonLibCoreGetAddressImpl(PyObject* pArgs, PyObject* pKwArgs)
{
    intptr_t implAddress = 0;

    assert(pArgs != NULL);
    assert(pKwArgs != NULL);

    implAddress = DszPythonLibCoreGetImpl(pArgs, pKwArgs);

    return ((DszCLibAddress) implAddress);
}

static inline
DszCLibPerson DszPythonLibCoreGetPersonImpl(PyObject* pArgs, PyObject* pKwArgs)
{
    intptr_t implAddress = 0;

    assert(pArgs != NULL);
    assert(pKwArgs != NULL);

    implAddress = DszPythonLibCoreGetImpl(pArgs, pKwArgs);

    return ((DszCLibPerson) implAddress);
}

static inline
DszCLibPrinter DszPythonLibCoreGetPrinterImpl(PyObject* pArgs, PyObject* pKwArgs)
{
    intptr_t implAddress = 0;

    assert(pArgs != NULL);
    assert(pKwArgs != NULL);

    implAddress = DszPythonLibCoreGetImpl(pArgs, pKwArgs);

    return ((DszCLibPrinter) implAddress);
}

DszCLibErrorNum DSZ_CLIB_CALLING_CONVENTION DszPythonLibCoreGenerateIntRedirect(
    int data,
    int* pInt,
    void* pUserData)
{
    PyObject* pGenerator = (PyObject*) pUserData;
    PyObject* pResult = NULL;

    if (pGenerator == NULL)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    pResult = PyObject_CallMethod(pGenerator, "generate_int", "(i)", data);

    if (pResult == NULL)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    if (pInt != NULL)
        *pInt = (int) PyLong_AsSsize_t(pResult);

    return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
}

DszCLibErrorNum DSZ_CLIB_CALLING_CONVENTION DszPythonLibCoreGenerateStringRedirect(
    int data,
    char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData)
{
    PyObject* pGenerator = (PyObject*) pUserData;
    PyObject* pResult = NULL;
    char const* pResultCstr = NULL;
    size_t numChars = 0;

    if (pGenerator == NULL)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    pResult = PyObject_CallMethod(pGenerator, "generate_string", "(i)", data);

    if (pResult == NULL)
        return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

    pResultCstr = PyUnicode_AsUTF8(pResult);

    numChars = strlen(pResultCstr);
    if ((pString != NULL) && (stringSize > 0)) {
        strncpy(pString, pResultCstr, stringSize);
        pString[stringSize - 1] = '\0';
        numChars = strlen(pString);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
}

#if !defined(_MSC_VER)
DSZ_PYTHONLIB_MODULE_VISIBILITY
#endif /* !defined(_MSC_VER) */
PyMODINIT_FUNC DSZ_PYTHONLIB_CALLING_CONVENTION PyInit_pythonlib_native(void)
{
    PyObject* pModule = PyModule_Create(&g_pythonLibModule);

    if (pModule == NULL)
        PyErr_SetString(PyExc_RuntimeError, "Failed to create pythonlib_native");

    return (pModule);
}

#if !defined(_MSC_VER)
DSZ_PYTHONLIB_MODULE_VISIBILITY
#endif /* !defined(_MSC_VER) */
PyMODINIT_FUNC DSZ_PYTHONLIB_CALLING_CONVENTION PyInitU_pythonlib_native(void)
{
    return (PyInit_pythonlib_native());
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryInitialize(
    PyObject* pSelf,
    PyObject* pArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;

    (void) pSelf;
    (void) pArgs;

    cLibErrorNum = DszCLibLibraryInitialize();

    if (cLibErrorNum != DSZ_CLIB_ERROR_NUM_NO_ERROR)
        return (Py_False);

    return (Py_True);
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryUninitialize(
    PyObject* pSelf,
    PyObject* pArgs)
{
    (void) pSelf;
    (void) pArgs;

    DszCLibLibraryUninitialize();

    return (PyLong_FromLong(0));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionString(
    PyObject* pSelf,
    PyObject* pArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    char versionString[DSZ_PYTHONLIBCORE_LIBRARY_VERSION_STRING_SIZE];

    (void) pSelf;
    (void) pArgs;

    memset(versionString, 0, DSZ_PYTHONLIBCORE_LIBRARY_VERSION_STRING_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionString(
        versionString,
        DSZ_PYTHONLIBCORE_LIBRARY_VERSION_STRING_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    versionString[DSZ_PYTHONLIBCORE_LIBRARY_VERSION_STRING_SIZE - 1] = '\0';

    return (PyUnicode_FromString(versionString));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionMajor(
    PyObject* pSelf,
    PyObject* pArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    size_t versionMajor = 0;

    (void) pSelf;
    (void) pArgs;

    cLibErrorNum = DszCLibLibraryGetVersionMajor(&versionMajor);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromSize_t(versionMajor));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionMinor(
    PyObject* pSelf,
    PyObject* pArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    size_t versionMinor = 0;

    (void) pSelf;
    (void) pArgs;

    cLibErrorNum = DszCLibLibraryGetVersionMinor(&versionMinor);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromSize_t(versionMinor));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionPatch(
    PyObject* pSelf,
    PyObject* pArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    size_t versionPatch = 0;

    (void) pSelf;
    (void) pArgs;

    cLibErrorNum = DszCLibLibraryGetVersionPatch(&versionPatch);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromSize_t(versionPatch));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionExtra(
    PyObject* pSelf,
    PyObject* pArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    char versionExtra[DSZ_PYTHONLIBCORE_LIBRARY_VERSION_EXTRA_SIZE];

    (void) pSelf;
    (void) pArgs;

    memset(versionExtra, 0, DSZ_PYTHONLIBCORE_LIBRARY_VERSION_EXTRA_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionExtra(
        versionExtra,
        DSZ_PYTHONLIBCORE_LIBRARY_VERSION_EXTRA_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    versionExtra[DSZ_PYTHONLIBCORE_LIBRARY_VERSION_EXTRA_SIZE - 1] = '\0';

    return (PyUnicode_FromString(versionExtra));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressCreate(
    PyObject* pSelf,
    PyObject* pArgs,
    PyObject* pKwArgs)
{
    char const* const KEYWORDS_FORMAT = "isssss";

    char const* const KEYWORDS[] = {
        "street_num",
        "street",
        "city",
        "province",
        "zip_code",
        "country",
        NULL };

    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    int streetNum = 0;
    char const* street = NULL;
    char const* city = NULL;
    char const* province = NULL;
    char const* zipCode = NULL;
    char const* country = NULL;
    int pyOpResult = 0;

    (void) pSelf;

    pyOpResult = PyArg_ParseTupleAndKeywords(
        pArgs,
        pKwArgs,
        KEYWORDS_FORMAT,
        (char**) KEYWORDS,
        &streetNum,
        &street,
        &city,
        &province,
        &zipCode,
        &country);

    if (pyOpResult == 0)
        return (NULL);

    cLibErrorNum = DszCLibAddressCreate(
        streetNum,
        street,
        city,
        province,
        zipCode,
        country,
        &cLibAddress);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromSize_t((size_t) cLibAddress));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressDestroy(
    PyObject* pSelf,
    PyObject* pArgs,
    PyObject* pKwArgs)
{
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    DszCLibAddressDestroy(cLibAddress);

    return (PyLong_FromLong(0));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetStreetNum(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    int streetNum = 0;

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    cLibErrorNum = DszCLibAddressGetStreetNum(
        cLibAddress,
        &streetNum);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromLong(streetNum));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetStreet(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char street[DSZ_PYTHONLIBCORE_ADDRESS_STREET_SIZE];

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    memset(street, 0, DSZ_PYTHONLIBCORE_ADDRESS_STREET_SIZE);

    cLibErrorNum = DszCLibAddressGetStreet(
        cLibAddress,
        street, DSZ_PYTHONLIBCORE_ADDRESS_STREET_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    street[DSZ_PYTHONLIBCORE_ADDRESS_STREET_SIZE - 1] = '\0';

    return (PyUnicode_FromString(street));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetCity(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char city[DSZ_PYTHONLIBCORE_ADDRESS_CITY_SIZE];

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    memset(city, 0, DSZ_PYTHONLIBCORE_ADDRESS_CITY_SIZE);

    cLibErrorNum = DszCLibAddressGetCity(
        cLibAddress,
        city, DSZ_PYTHONLIBCORE_ADDRESS_CITY_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    city[DSZ_PYTHONLIBCORE_ADDRESS_CITY_SIZE - 1] = '\0';

    return (PyUnicode_FromString(city));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetProvince(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char province[DSZ_PYTHONLIBCORE_ADDRESS_PROVINCE_SIZE];

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    memset(province, 0, DSZ_PYTHONLIBCORE_ADDRESS_PROVINCE_SIZE);

    cLibErrorNum = DszCLibAddressGetProvince(
        cLibAddress,
        province, DSZ_PYTHONLIBCORE_ADDRESS_PROVINCE_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    province[DSZ_PYTHONLIBCORE_ADDRESS_PROVINCE_SIZE - 1] = '\0';

    return (PyUnicode_FromString(province));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetZipCode(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char zipCode[DSZ_PYTHONLIBCORE_ADDRESS_ZIP_CODE_SIZE];

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    memset(zipCode, 0, DSZ_PYTHONLIBCORE_ADDRESS_ZIP_CODE_SIZE);

    cLibErrorNum = DszCLibAddressGetZipCode(
        cLibAddress,
        zipCode, DSZ_PYTHONLIBCORE_ADDRESS_ZIP_CODE_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    zipCode[DSZ_PYTHONLIBCORE_ADDRESS_ZIP_CODE_SIZE - 1] = '\0';

    return (PyUnicode_FromString(zipCode));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetCountry(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char country[DSZ_PYTHONLIBCORE_ADDRESS_COUNTRY_SIZE];

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    memset(country, 0, DSZ_PYTHONLIBCORE_ADDRESS_COUNTRY_SIZE);

    cLibErrorNum = DszCLibAddressGetCountry(
        cLibAddress,
        country, DSZ_PYTHONLIBCORE_ADDRESS_COUNTRY_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    country[DSZ_PYTHONLIBCORE_ADDRESS_COUNTRY_SIZE - 1] = '\0';

    return (PyUnicode_FromString(country));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressToString(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    char addressString[DSZ_PYTHONLIBCORE_ADDRESS_STRING_SIZE];

    (void) pSelf;

    cLibAddress = DszPythonLibCoreGetAddressImpl(pArgs, pKwArgs);

    if ((cLibAddress == 0) ||
        (cLibAddress == NULL) ||
        (cLibAddress == DSZ_CLIB_ADDRESS_INVALID))
        return (NULL);

    memset(addressString, 0, DSZ_PYTHONLIBCORE_ADDRESS_STRING_SIZE);

    cLibErrorNum = DszCLibAddressToString(
        cLibAddress,
        addressString, DSZ_PYTHONLIBCORE_ADDRESS_STRING_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    addressString[DSZ_PYTHONLIBCORE_ADDRESS_STRING_SIZE - 1] = '\0';

    return (PyUnicode_FromString(addressString));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonCreate(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    char const* const KEYWORDS_FORMAT = "ssin";

    char const* const KEYWORDS[] = {
        "last_name",
        "first_name",
        "age",
        "address_impl",
        NULL };

    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    char const* lastName = NULL;
    char const* firstName = NULL;
    int age = 0;
    Py_ssize_t addressImpl = 0;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;
    int pyOpResult = 0;

    (void) pSelf;

    pyOpResult = PyArg_ParseTupleAndKeywords(
        pArgs,
        pKwArgs,
        KEYWORDS_FORMAT,
        (char**) KEYWORDS,
        &lastName,
        &firstName,
        &age,
        &addressImpl);

    if (pyOpResult == 0)
        return (NULL);

    cLibAddress = (DszCLibAddress) addressImpl;

    cLibErrorNum = DszCLibPersonCreate(
        lastName,
        firstName,
        age,
        cLibAddress,
        &cLibPerson);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromSize_t((size_t) cLibPerson));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonDestroy(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;

    (void) pSelf;

    cLibPerson = DszPythonLibCoreGetPersonImpl(pArgs, pKwArgs);

    if ((cLibPerson == 0) ||
        (cLibPerson == NULL) ||
        (cLibPerson == DSZ_CLIB_PERSON_INVALID))
        return (NULL);

    DszCLibPersonDestroy(cLibPerson);

    return (PyLong_FromLong(0));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetLastName(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    char lastName[DSZ_PYTHONLIBCORE_PERSON_LAST_NAME_SIZE];

    (void) pSelf;

    cLibPerson = DszPythonLibCoreGetPersonImpl(pArgs, pKwArgs);

    if ((cLibPerson == 0) ||
        (cLibPerson == NULL) ||
        (cLibPerson == DSZ_CLIB_PERSON_INVALID))
        return (NULL);

    memset(lastName, 0, DSZ_PYTHONLIBCORE_PERSON_LAST_NAME_SIZE);

    cLibErrorNum = DszCLibPersonGetLastName(
        cLibPerson,
        lastName, DSZ_PYTHONLIBCORE_PERSON_LAST_NAME_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    lastName[DSZ_PYTHONLIBCORE_PERSON_LAST_NAME_SIZE - 1] = '\0';

    return (PyUnicode_FromString(lastName));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetFirstName(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    char firstName[DSZ_PYTHONLIBCORE_PERSON_FIRST_NAME_SIZE];

    (void) pSelf;

    cLibPerson = DszPythonLibCoreGetPersonImpl(pArgs, pKwArgs);

    if ((cLibPerson == 0) ||
        (cLibPerson == NULL) ||
        (cLibPerson == DSZ_CLIB_PERSON_INVALID))
        return (NULL);

    memset(firstName, 0, DSZ_PYTHONLIBCORE_PERSON_FIRST_NAME_SIZE);

    cLibErrorNum = DszCLibPersonGetFirstName(
        cLibPerson,
        firstName, DSZ_PYTHONLIBCORE_PERSON_FIRST_NAME_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    firstName[DSZ_PYTHONLIBCORE_PERSON_FIRST_NAME_SIZE - 1] = '\0';

    return (PyUnicode_FromString(firstName));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetAge(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    int age = 0;

    (void) pSelf;

    cLibPerson = DszPythonLibCoreGetPersonImpl(pArgs, pKwArgs);

    if ((cLibPerson == 0) ||
        (cLibPerson == NULL) ||
        (cLibPerson == DSZ_CLIB_PERSON_INVALID))
        return (NULL);

    cLibErrorNum = DszCLibPersonGetAge(
        cLibPerson,
        &age);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromLong(age));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetAddress(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    DszCLibAddress cLibAddress = DSZ_CLIB_ADDRESS_INVALID;

    (void) pSelf;

    cLibPerson = DszPythonLibCoreGetPersonImpl(pArgs, pKwArgs);

    if ((cLibPerson == 0) ||
        (cLibPerson == NULL) ||
        (cLibPerson == DSZ_CLIB_PERSON_INVALID))
        return (NULL);

    cLibErrorNum = DszCLibPersonGetAddress(
        cLibPerson,
        &cLibAddress);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromSize_t((size_t) cLibAddress));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonToString(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPerson cLibPerson = DSZ_CLIB_PERSON_INVALID;
    char personString[DSZ_PYTHONLIBCORE_PERSON_STRING_SIZE];

    (void) pSelf;

    cLibPerson = DszPythonLibCoreGetPersonImpl(pArgs, pKwArgs);

    if ((cLibPerson == 0) ||
        (cLibPerson == NULL) ||
        (cLibPerson == DSZ_CLIB_PERSON_INVALID))
        return (NULL);

    memset(personString, 0, DSZ_PYTHONLIBCORE_PERSON_STRING_SIZE);

    cLibErrorNum = DszCLibPersonToString(
        cLibPerson,
        personString, DSZ_PYTHONLIBCORE_PERSON_STRING_SIZE,
        NULL);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    personString[DSZ_PYTHONLIBCORE_PERSON_STRING_SIZE - 1] = '\0';

    return (PyUnicode_FromString(personString));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterCreate(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;
    DszCLibGenerator cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;

    (void) pSelf;
    (void) pArgs;
    (void) pKwArgs;

    cLibErrorNum = DszCLibGeneratorCreate(
        &DszPythonLibCoreGenerateIntRedirect,
        &DszPythonLibCoreGenerateStringRedirect,
        &cLibGenerator);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    cLibErrorNum = DszCLibPrinterCreate(
        cLibGenerator,
        &cLibPrinter);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromSize_t((size_t) cLibPrinter));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterDestroy(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;

    (void) pSelf;

    cLibPrinter = DszPythonLibCoreGetPrinterImpl(pArgs, pKwArgs);

    if ((cLibPrinter == 0) ||
        (cLibPrinter == NULL) ||
        (cLibPrinter == DSZ_CLIB_PRINTER_INVALID))
        return (NULL);

    DszCLibPrinterDestroy(cLibPrinter);

    return (PyLong_FromLong(0));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterPrintInt(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    char const* const KEYWORDS_FORMAT = "nO";

    char const* const KEYWORDS[] = {
        "impl",
        "generator",
        NULL };

    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;
    Py_ssize_t implAddress = 0;
    PyObject* pGenerator = NULL;
    int pyOpResult;

    (void) pSelf;

    pyOpResult = PyArg_ParseTupleAndKeywords(
        pArgs,
        pKwArgs,
        KEYWORDS_FORMAT,
        (char**) KEYWORDS,
        &implAddress,
        &pGenerator);

    if (pyOpResult == 0)
        return (NULL);

    cLibPrinter = (DszCLibPrinter) implAddress;

    cLibErrorNum = DszCLibPrinterPrintIntWithUserData(
        cLibPrinter,
        (void*) pGenerator);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromLong(0));
}

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterPrintString(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs)
{
    char const* const KEYWORDS_FORMAT = "nO";

    char const* const KEYWORDS[] = {
        "impl",
        "generator",
        NULL };

    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERROR_NUM_NO_ERROR;
    DszCLibPrinter cLibPrinter = DSZ_CLIB_PRINTER_INVALID;
    Py_ssize_t implAddress = 0;
    PyObject* pGenerator = NULL;
    int pyOpResult;

    (void) pSelf;

    pyOpResult = PyArg_ParseTupleAndKeywords(
        pArgs,
        pKwArgs,
        KEYWORDS_FORMAT,
        (char**) KEYWORDS,
        &implAddress,
        &pGenerator);

    if (pyOpResult == 0)
        return (NULL);

    cLibPrinter = (DszCLibPrinter) implAddress;

    cLibErrorNum = DszCLibPrinterPrintStringWithUserData(
        cLibPrinter,
        (void*) pGenerator);

    DSZ_PYTHONLIBCORE_API_CHECK(cLibErrorNum);

    return (PyLong_FromLong(0));
}
