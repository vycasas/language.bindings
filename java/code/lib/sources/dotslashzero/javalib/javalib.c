#include <dotslashzero/javalib/javalib.h>

#include <dotslashzero/clib/clib.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
#error Please use pure C compiler for this file.
#endif /* defined(__cplusplus) */

#define DSZ_JAVALIBCORE_INLINE inline

#if defined(_MSC_VER)
#undef strncpy
#define strncpy(buffer, source, bufferSize) strncpy_s(buffer, bufferSize, source, _TRUNCATE)
#undef DSZ_JAVALIBCORE_INLINE
#define DSZ_JAVALIBCORE_INLINE __inline
#endif /* defined(_MSC_VER) */

#define DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum) ((jlong) errorNum)

typedef jlong DszJavaLibCoreErrorNum;
static DszJavaLibCoreErrorNum const DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR               = 0x00000000;
static DszJavaLibCoreErrorNum const DSZ_JAVALIBCORE_ERRORNUM_CLIB_ERROR             = 0x00010000;
static DszJavaLibCoreErrorNum const DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR = 0x00020000;
static DszJavaLibCoreErrorNum const DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR      = 0x00040000;

#if defined(_MSC_VER)
#define DSZ_JAVALIBCORE_ERRORNUM_MESSAGE_SIZE 16
#define DSZ_JAVALIBCORE_LIBRARY_VERSION_STRING_SIZE 16
#define DSZ_JAVALIBCORE_LIBRARY_VERSION_EXTRA_SIZE 16
#define DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE 40
#define DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE 16
#define DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE 16
#define DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE 8
#define DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE 16
#define DSZ_JAVALIBCORE_ADDRESS_STRING_SIZE 128
#define DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE 24
#define DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE 24
#define DSZ_JAVALIBCORE_PERSON_STRING_SIZE 256
#else /* defined(_MSC_VER) */
static size_t const DSZ_JAVALIBCORE_ERRORNUM_MESSAGE_SIZE = 16;
static size_t const DSZ_JAVALIBCORE_LIBRARY_VERSION_STRING_SIZE = 16;
static size_t const DSZ_JAVALIBCORE_LIBRARY_VERSION_EXTRA_SIZE = 16;
static size_t const DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE = 40;
static size_t const DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE = 16;
static size_t const DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE = 16;
static size_t const DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE = 8;
static size_t const DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE = 16;
static size_t const DSZ_JAVALIBCORE_ADDRESS_STRING_SIZE = 128;
static size_t const DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE = 24;
static size_t const DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE = 24;
static size_t const DSZ_JAVALIBCORE_PERSON_STRING_SIZE = 256;
#endif /* defined(_MSC_VER) */

/* ------- */
/* internal core types */

typedef struct DszJavaLibCorePrinterUserData_
{
    JNIEnv* JavaEnv;
    jobject Generator;
}
DszJavaLibCorePrinterUserData;

typedef DszJavaLibCorePrinterUserData* DszJavaLibCorePrinterUserDataPtr;

/* ------- */

/* ------- */
/* internal core functions */

static DSZ_JAVALIBCORE_INLINE bool DszJavaLibCoreIsCLibError(
    DszJavaLibCoreErrorNum errorNum)
{
    DszJavaLibCoreErrorNum test = (errorNum & DSZ_JAVALIBCORE_ERRORNUM_CLIB_ERROR);
    return (test != 0);
}

static DSZ_JAVALIBCORE_INLINE DszJavaLibCoreErrorNum DszJavaLibCoreConvertCLibErrorNum(
    DszCLibErrorNum cLibErrorNum)
{
    return ((DszJavaLibCoreErrorNum) (DSZ_JAVALIBCORE_ERRORNUM_CLIB_ERROR | cLibErrorNum));
}

static DSZ_JAVALIBCORE_INLINE DszCLibErrorNum DszJavaLibCoreConvertJavaLibCoreErrorNum(
    DszJavaLibCoreErrorNum javaLibCoreErrorNum)
{
    return ((DszCLibErrorNum) (javaLibCoreErrorNum ^ DSZ_JAVALIBCORE_ERRORNUM_CLIB_ERROR));
}

static DSZ_JAVALIBCORE_INLINE char const* DszJavaLibCoreGetErrorMessage(
    DszJavaLibCoreErrorNum errorNum)
{
    if (errorNum == DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR) {
        return ("No error.");
    }
    if (errorNum == DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR) {
        return ("Internal JNI error.");
    }
    if (errorNum == DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR) {
        return ("JNI error.");
    }
    return ("Unknown error.");
}

static DszJavaLibCoreErrorNum DszJavaLibCoreNativeTypeSetAddress(
    JNIEnv* pEnv,
    jobject nativeType,
    uintptr_t value)
{
    JNIEnv pEnvActual = NULL;
    jclass nativeTypeClass = NULL; /* net.dotslashzero.javalib.Core.NativeType */
    jmethodID nativeTypeSetAddressId = NULL;

    assert(pEnv != NULL);
    assert(nativeType != NULL);

    pEnvActual = *pEnv;

    if (pEnvActual == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    nativeTypeClass = pEnvActual->FindClass(pEnv, "net/dotslashzero/javalib/Core$NativeType");
    if (nativeTypeClass == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    nativeTypeSetAddressId = pEnvActual->GetMethodID(pEnv, nativeTypeClass, "setAddress", "(J)V");

    if (nativeTypeSetAddressId == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    pEnvActual->CallVoidMethod(pEnv, nativeType, nativeTypeSetAddressId, (jlong) value);

    return (DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR);
}

static DszJavaLibCoreErrorNum DszJavaLibCoreWrappedStringSetString(
    JNIEnv* pEnv,
    jobject wrappedString,
    char const* value)
{
    JNIEnv pEnvActual = NULL;
    jclass wrappedStringClass = NULL; /* net.dotslashzero.javalib.Core.WrappedString */
    jmethodID wrappedStringSetStringId = NULL;
    jstring stringValue = NULL;

    assert(pEnv != NULL);
    assert(wrappedString != NULL);

    pEnvActual = *pEnv;

    if (pEnvActual == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    wrappedStringClass = pEnvActual->FindClass(pEnv, "net/dotslashzero/javalib/Core$WrappedString");

    if (wrappedStringClass == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    wrappedStringSetStringId = pEnvActual->GetMethodID(pEnv, wrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (wrappedStringSetStringId == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    stringValue = pEnvActual->NewStringUTF(pEnv, value);

    pEnvActual->CallVoidMethod(pEnv, wrappedString, wrappedStringSetStringId, stringValue);

    pEnvActual->DeleteLocalRef(pEnv, stringValue);

    return (DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR);
}

static DszJavaLibCoreErrorNum DszJavaLibCoreWrappedLongSetLong(
    JNIEnv* pEnv,
    jobject wrappedLong,
    long value)
{
    JNIEnv pEnvActual = NULL;
    jclass wrappedLongClass = NULL; /* net.dotslashzero.javalib.Core.WrappedLong */
    jmethodID wrappedLongSetLongId = NULL;

    assert(pEnv != NULL);
    assert(wrappedLong != NULL);

    pEnvActual = *pEnv;

    if (pEnvActual == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    wrappedLongClass = pEnvActual->FindClass(pEnv, "net/dotslashzero/javalib/Core$WrappedLong");

    if (wrappedLongClass == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    wrappedLongSetLongId = pEnvActual->GetMethodID(pEnv, wrappedLongClass, "setLong", "(J)V");

    if (wrappedLongSetLongId == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    pEnvActual->CallVoidMethod(pEnv, wrappedLong, wrappedLongSetLongId, (jlong) value);

    return (DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR);
}

static DszJavaLibCoreErrorNum DszJavaLibCoreWrappedIntegerSetInt(
    JNIEnv* pEnv,
    jobject wrappedInteger,
    int value)
{
    JNIEnv pEnvActual = NULL;
    jclass wrappedIntegerClass = NULL; /* net.dotslashzero.javalib.Core.WrappedInteger */
    jmethodID wrappedIntegerSetIntId = NULL;

    assert(pEnv != NULL);
    assert(wrappedInteger != NULL);

    pEnvActual = *pEnv;

    if (pEnvActual == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    wrappedIntegerClass = pEnvActual->FindClass(pEnv, "net/dotslashzero/javalib/Core$WrappedInteger");

    if (wrappedIntegerClass == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    wrappedIntegerSetIntId = pEnvActual->GetMethodID(pEnv, wrappedIntegerClass, "setInt", "(J)V");

    if (wrappedIntegerSetIntId == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    pEnvActual->CallVoidMethod(pEnv, wrappedInteger, wrappedIntegerSetIntId, (jint) value);

    return (DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR);
}

static DszJavaLibCoreErrorNum DszJavaLibCoreCopyJStringToCString(
    JNIEnv* pEnv,
    jstring jString,
    char* pCString,
    size_t cStringSize,
    size_t* pCharsWritten)
{
    JNIEnv pEnvActual = NULL;
    char const* pJString = NULL;
    jsize jStringSize = 0;
    size_t numChars = 0;

    assert(pEnv != NULL);
    assert(jString != NULL);

    pEnvActual = *pEnv;

    if (pEnvActual == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    jStringSize = pEnvActual->GetStringUTFLength(pEnv, jString);

    if (jStringSize == 0)
        return (DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR);

    pJString = pEnvActual->GetStringUTFChars(pEnv, jString, NULL);

    if (pJString == NULL)
        return (DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_INTERNAL_ERROR);

    numChars = (size_t) jStringSize;

    if ((pCString != NULL) && (cStringSize > 0)) {
        strncpy(pCString, pJString, cStringSize);
        pCString[cStringSize - 1] = '\0';
        numChars = strlen(pCString);
    }

    pEnvActual->ReleaseStringUTFChars(pEnv, jString, pJString);

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

    return (DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR);
}

static DszCLibErrorNum DszJavaLibCorePrinterGenerateIntRedirect(
    int data,
    int* pInt,
    void* pUserData)
{
    /* note: do not assert here because this function will be passed to the C library */
    DszJavaLibCorePrinterUserDataPtr pPrinterUserData = (DszJavaLibCorePrinterUserDataPtr) pUserData;
    JNIEnv* pEnv = NULL;
    jobject generator = NULL;
    JNIEnv pEnvActual = NULL;
    jclass iGeneratorInterface = NULL;
    jmethodID iGeneratorGenerateIntId = NULL;
    jint generatedInt = 0;

    if (pPrinterUserData == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    pEnv = pPrinterUserData->JavaEnv;
    generator = pPrinterUserData->Generator;

    if ((pEnv == NULL) || (generator == NULL))
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    if (pInt == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    pEnvActual = *pEnv;

    iGeneratorInterface = pEnvActual->FindClass(
        pEnv,
        "net/dotslashzero/javalib/IGenerator");

    if (iGeneratorInterface == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    iGeneratorGenerateIntId = pEnvActual->GetMethodID(
        pEnv,
        iGeneratorInterface,
        "generateInt",
        "(I)I");

    if (iGeneratorGenerateIntId == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    generatedInt = pEnvActual->CallIntMethod(
        pEnv,
        generator,
        iGeneratorGenerateIntId,
        (jint) data);

    *pInt = (int) generatedInt;

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

static DszCLibErrorNum DszJavaLibCorePrinterGenerateStringRedirect(
    int data,
    char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData)
{
    /* note: do not assert here because this function will be passed to the C library */
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszJavaLibCorePrinterUserDataPtr pPrinterUserData = (DszJavaLibCorePrinterUserDataPtr) pUserData;
    JNIEnv* pEnv = NULL;
    jobject generator = NULL;
    JNIEnv pEnvActual = NULL;
    jclass iGeneratorInterface = NULL;
    jmethodID iGeneratorGenerateStringId = NULL;
    jstring generatedString = NULL;

    if (pPrinterUserData == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    pEnv = pPrinterUserData->JavaEnv;
    generator = pPrinterUserData->Generator;

    if ((pEnv == NULL) || (generator == NULL))
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    pEnvActual = *pEnv;

    iGeneratorInterface = pEnvActual->FindClass(
        pEnv,
        "net/dotslashzero/javalib/IGenerator");

    if (iGeneratorInterface == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    iGeneratorGenerateStringId = pEnvActual->GetMethodID(
        pEnv,
        iGeneratorInterface,
        "generateString",
        "(I)Ljava/lang/String;");

    if (iGeneratorGenerateStringId == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    /* note: the casting to jstring below may not work in C++ */
    generatedString = (jstring) pEnvActual->CallObjectMethod(
        pEnv,
        generator,
        iGeneratorGenerateStringId,
        (jint) data);

    if (generatedString == NULL)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        generatedString,
        pString,
        stringSize,
        pCharsWritten);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_CLIB_ERRORNUM_CALLBACK_ERROR);

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

/* ------- */

/* ------- */
/* JNIEXPORT functions */

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_JavaLibException_nativeGetErrorMessage(
    JNIEnv* pEnv,
    jclass javaLibExceptionClass,
    jlong errorNum,
    jobject errorMessageWrappedString)
{
    char errorNumString[DSZ_JAVALIBCORE_ERRORNUM_MESSAGE_SIZE];
    char const* pErrorMessage = NULL;

    (void) javaLibExceptionClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (errorMessageWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if (DszJavaLibCoreIsCLibError(errorNum)) {
        DszCLibErrorNum cLibErrorNum = DszJavaLibCoreConvertJavaLibCoreErrorNum(errorNum);
        DszCLibErrorNumGetMessage(
            cLibErrorNum,
            errorNumString, DSZ_JAVALIBCORE_ERRORNUM_MESSAGE_SIZE,
            NULL);

        errorNumString[DSZ_JAVALIBCORE_ERRORNUM_MESSAGE_SIZE - 1] = '\0';

        pErrorMessage = errorNumString;
    }
    else {
        pErrorMessage = DszJavaLibCoreGetErrorMessage(errorNum);
    }

    DszJavaLibCoreWrappedStringSetString(
        pEnv,
        errorMessageWrappedString,
        pErrorMessage);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeInitialize(
    JNIEnv* pEnv,
    jclass libraryClass)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) pEnv;
    (void) libraryClass;

    cLibErrorNum = DszCLibLibraryInitialize();

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeUninitialize(
    JNIEnv* pEnv,
    jclass libraryClass)
{
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) pEnv;
    (void) libraryClass;

    cLibErrorNum = DszCLibLibraryUninitialize();

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionString(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionStringWrappedString)
{
    char versionString[DSZ_JAVALIBCORE_LIBRARY_VERSION_STRING_SIZE];
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) libraryClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (versionStringWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(versionString, 0, DSZ_JAVALIBCORE_LIBRARY_VERSION_STRING_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionString(
        versionString, DSZ_JAVALIBCORE_LIBRARY_VERSION_STRING_SIZE,
        NULL);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    versionString[DSZ_JAVALIBCORE_LIBRARY_VERSION_STRING_SIZE - 1] = '\0';

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        versionStringWrappedString,
        versionString);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionMajor(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionMajorWrappedLong)
{
    size_t versionMajor = 0;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) libraryClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (versionMajorWrappedLong == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibLibraryGetVersionMajor(&versionMajor);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedLongSetLong(
        pEnv,
        versionMajorWrappedLong,
        (long) versionMajor);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionMinor(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionMinorWrappedLong)
{
    size_t versionMinor = 0;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) libraryClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (versionMinorWrappedLong == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibLibraryGetVersionMinor(&versionMinor);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedLongSetLong(
        pEnv,
        versionMinorWrappedLong,
        (long) versionMinor);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionPatch(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionPatchWrappedLong)
{
    size_t versionPatch = 0;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) libraryClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (versionPatchWrappedLong == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibLibraryGetVersionPatch(&versionPatch);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedLongSetLong(
        pEnv,
        versionPatchWrappedLong,
        (long) versionPatch);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionExtra(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionExtraWrappedString)
{
    char versionExtraString[DSZ_JAVALIBCORE_LIBRARY_VERSION_EXTRA_SIZE];
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) libraryClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (versionExtraWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(versionExtraString, 0, DSZ_JAVALIBCORE_LIBRARY_VERSION_EXTRA_SIZE);

    cLibErrorNum = DszCLibLibraryGetVersionExtra(
        versionExtraString, DSZ_JAVALIBCORE_LIBRARY_VERSION_EXTRA_SIZE,
        NULL);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    versionExtraString[DSZ_JAVALIBCORE_LIBRARY_VERSION_EXTRA_SIZE - 1] = '\0';

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        versionExtraWrappedString,
        versionExtraString);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeCreateAddress(
    JNIEnv* pEnv,
    jclass addressClass,
    jint streetNum,
    jstring street,
    jstring city,
    jstring province,
    jstring zipCode,
    jstring country,
    jobject addressAddressType)
{
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszCLibAddress address = DSZ_CLIB_ADDRESS_INVALID;
    int cStreetNum = (int) streetNum;
    char cStreet[DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE];
    char cCity[DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE];
    char cProvince[DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE];
    char cZipCode[DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE];
    char cCountry[DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE];

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (street == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (city == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (province == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (zipCode == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (country == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (addressAddressType == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(cStreet, 0, DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE);
    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        street,
        cStreet, DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE,
        NULL);
    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return(DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    memset(cCity, 0, DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE);
    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        city,
        cCity, DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE,
        NULL);
    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    memset(cProvince, 0, DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE);
    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        province,
        cProvince, DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE,
        NULL);
    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    memset(cZipCode, 0, DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE);
    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        zipCode,
        cZipCode, DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE,
        NULL);
    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    memset(cCountry, 0, DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE);
    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        country,
        cCountry, DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE,
        NULL);
    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    cLibErrorNum = DszCLibAddressCreate(
        cStreetNum,
        cStreet,
        cCity,
        cProvince,
        cZipCode,
        cCountry,
        &address);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    errorNum = DszJavaLibCoreNativeTypeSetAddress(
        pEnv,
        addressAddressType,
        (uintptr_t) address);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        DszCLibAddressDestroy(address); /* this is important! */

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeDestroyAddress(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if ((address == NULL) || (address == DSZ_CLIB_ADDRESS_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibAddressDestroy(address);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreetNum(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject streetNumWrappedInteger)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    int streetNum = 0;

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((address == NULL) || (address == DSZ_CLIB_ADDRESS_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (streetNumWrappedInteger == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibAddressGetStreetNum(address, &streetNum);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedIntegerSetInt(
        pEnv,
        streetNumWrappedInteger,
        streetNum);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreet(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject streetWrappedString)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    char street[DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE];

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((address == NULL) || (address == DSZ_CLIB_ADDRESS_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (streetWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(street, 0, DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE);

    cLibErrorNum = DszCLibAddressGetStreet(
        address,
        street, DSZ_JAVALIBCORE_ADDRESS_STREET_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        streetWrappedString,
        street);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCity(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject cityWrappedString)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    char city[DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE];

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (address == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (cityWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(city, 0, DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE);

    cLibErrorNum = DszCLibAddressGetCity(
        address,
        city, DSZ_JAVALIBCORE_ADDRESS_CITY_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        cityWrappedString,
        city);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetProvince(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject provinceWrappedString)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    char province[DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE];

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (address == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (provinceWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(province, 0, DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE);

    cLibErrorNum = DszCLibAddressGetProvince(
        address,
        province, DSZ_JAVALIBCORE_ADDRESS_PROVINCE_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        provinceWrappedString,
        province);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetZipCode(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject zipCodeWrappedString)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    char zipCode[DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE];

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (address == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (zipCodeWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(zipCode, 0, DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE);

    cLibErrorNum = DszCLibAddressGetZipCode(
        address,
        zipCode, DSZ_JAVALIBCORE_ADDRESS_ZIPCODE_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        zipCodeWrappedString,
        zipCode);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCountry(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject countryWrappedString)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    char country[DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE];

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (address == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (countryWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(country, 0, DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE);

    cLibErrorNum = DszCLibAddressGetCountry(
        address,
        country, DSZ_JAVALIBCORE_ADDRESS_COUNTRY_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        countryWrappedString,
        country);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeToString(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject addressStringWrappedString)
{
    DszCLibAddress address = (DszCLibAddress) addressImpl;
    char addressString[DSZ_JAVALIBCORE_ADDRESS_STRING_SIZE];
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) addressClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((address == NULL) || (address == DSZ_CLIB_ADDRESS_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (addressStringWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(addressString, 0, DSZ_JAVALIBCORE_ADDRESS_STRING_SIZE);

    cLibErrorNum = DszCLibAddressToString(
        address,
        addressString, DSZ_JAVALIBCORE_ADDRESS_STRING_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    addressString[DSZ_JAVALIBCORE_ADDRESS_STRING_SIZE - 1] = '\0';

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        addressStringWrappedString,
        addressString);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeCreatePerson(
    JNIEnv* pEnv,
    jclass personClass,
    jstring lastName,
    jstring firstName,
    jint age,
    jlong addressImpl,
    jobject personPersonType)
{
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszCLibPerson person = DSZ_CLIB_PERSON_INVALID;
    char cLastName[DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE];
    char cFirstName[DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE];
    int cAge = (int) age;
    DszCLibAddress address = (DszCLibAddress) addressImpl;

    (void) personClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (lastName == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (firstName == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((address == NULL) || (address == DSZ_CLIB_ADDRESS_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (personPersonType == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(cLastName, 0, DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE);
    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        lastName,
        cLastName, DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE,
        NULL);
    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return(DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    memset(cFirstName, 0, DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE);
    errorNum = DszJavaLibCoreCopyJStringToCString(
        pEnv,
        firstName,
        cFirstName, DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE,
        NULL);
    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    cLibErrorNum = DszCLibPersonCreate(
        cLastName,
        cFirstName,
        cAge,
        address,
        &person);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    errorNum = DszJavaLibCoreNativeTypeSetAddress(
        pEnv,
        personPersonType,
        (uintptr_t) person);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        DszCLibPersonDestroy(person); /* this is important! */

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeDestroyPerson(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl)
{
    DszCLibPerson person = (DszCLibPerson) personImpl;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) personClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if ((person == NULL) || (person == DSZ_CLIB_PERSON_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibPersonDestroy(person);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetLastName(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject lastNameWrappedString)
{
    DszCLibPerson person = (DszCLibPerson) personImpl;
    char lastName[DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE];
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) personClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((person == NULL) || (person == DSZ_CLIB_PERSON_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (lastNameWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(lastName, 0, DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE);

    cLibErrorNum = DszCLibPersonGetLastName(
        person,
        lastName, DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    lastName[DSZ_JAVALIBCORE_PERSON_LASTNAME_SIZE - 1] = '\0';

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        lastNameWrappedString,
        lastName);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetFirstName(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject firstNameWrappedString)
{
    DszCLibPerson person = (DszCLibPerson) personImpl;
    char firstName[DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE];
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) personClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((person == NULL) || (person == DSZ_CLIB_PERSON_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (firstNameWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(firstName, 0, DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE);

    cLibErrorNum = DszCLibPersonGetFirstName(
        person,
        firstName, DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    firstName[DSZ_JAVALIBCORE_PERSON_FIRSTNAME_SIZE - 1] = '\0';

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        firstNameWrappedString,
        firstName);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAge(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject ageWrappedInteger)
{
    DszCLibPerson person = (DszCLibPerson) personImpl;
    int age = 0;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) personClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((person == NULL) || (person == DSZ_CLIB_PERSON_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (ageWrappedInteger == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibPersonGetAge(
        person,
        &age);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreWrappedIntegerSetInt(
        pEnv,
        ageWrappedInteger,
        age);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAddress(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject addressAddressType)
{
    DszCLibPerson person = (DszCLibPerson) personImpl;
    DszCLibAddress address = DSZ_CLIB_ADDRESS_INVALID;
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) personClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((person == NULL) || (person == DSZ_CLIB_PERSON_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (addressAddressType == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibPersonGetAddress(
        person,
        &address);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    errorNum = DszJavaLibCoreNativeTypeSetAddress(
        pEnv,
        addressAddressType,
        (uintptr_t) address);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        DszCLibAddressDestroy(address);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeToString(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject personStringWrappedString)
{
    DszCLibPerson person = (DszCLibPerson) personImpl;
    char personString[DSZ_JAVALIBCORE_PERSON_STRING_SIZE];
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) personClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((person == NULL) || (person == DSZ_CLIB_PERSON_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (personStringWrappedString == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    memset(personString, 0, DSZ_JAVALIBCORE_PERSON_STRING_SIZE);

    cLibErrorNum = DszCLibPersonToString(
        person,
        personString, DSZ_JAVALIBCORE_PERSON_STRING_SIZE,
        NULL);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    personString[DSZ_JAVALIBCORE_ADDRESS_STRING_SIZE - 1] = '\0';

    errorNum = DszJavaLibCoreWrappedStringSetString(
        pEnv,
        personStringWrappedString,
        personString);

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativeCreateGenerator(
    JNIEnv* pEnv,
    jclass printerClass,
    jobject generatorGeneratorType)
{
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszCLibGenerator generator = DSZ_CLIB_GENERATOR_INVALID;

    (void) printerClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (generatorGeneratorType == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibGeneratorCreate(
        (DszCLibGenerateIntFunction) &(DszJavaLibCorePrinterGenerateIntRedirect),
        (DszCLibGenerateStringFunction) &(DszJavaLibCorePrinterGenerateStringRedirect),
        &generator);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    errorNum = DszJavaLibCoreNativeTypeSetAddress(
        pEnv,
        generatorGeneratorType,
        (uintptr_t) generator);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        DszCLibGeneratorDestroy(generator); /* this is important! */

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativeDestroyGenerator(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong generatorImpl)
{
    DszCLibGenerator generator = (DszCLibGenerator) generatorImpl;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) printerClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if ((generator == NULL) || (generator == DSZ_CLIB_GENERATOR_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibGeneratorDestroy(generator);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativeCreatePrinter(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong generatorImpl,
    jobject printerPrinterType)
{
    DszJavaLibCoreErrorNum errorNum = DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszCLibGenerator generator = (DszCLibGenerator) generatorImpl;
    DszCLibPrinter printer = DSZ_CLIB_PRINTER_INVALID;

    (void) printerClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if (printerPrinterType == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));
    if ((generator == NULL) || (generator == DSZ_CLIB_GENERATOR_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    /* note: printer takes ownership of generator after this call is successful */
    cLibErrorNum = DszCLibPrinterCreate(
        generator,
        &printer);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
        errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));

    errorNum = DszJavaLibCoreNativeTypeSetAddress(
        pEnv,
        printerPrinterType,
        (uintptr_t) printer);

    if (errorNum != DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR)
        DszCLibPrinterDestroy(printer); /* this is important! */

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativeDestroyPrinter(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong printerImpl)
{
    DszCLibPrinter printer = (DszCLibPrinter) printerImpl;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;

    (void) printerClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if ((printer == NULL) || (printer == DSZ_CLIB_PRINTER_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    cLibErrorNum = DszCLibPrinterDestroy(printer);
    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintInt(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong printerImpl,
    jobject generatorInstance)
{
    DszCLibPrinter printer = (DszCLibPrinter) printerImpl;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszJavaLibCorePrinterUserData userData = {
        .JavaEnv = NULL,
        .Generator = NULL
    };

    (void) printerClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if ((printer == NULL) || (printer == DSZ_CLIB_PRINTER_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if (generatorInstance == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    /* note: if design requires executing printInt in a different thread/process space,
        userData must be kept alive longer than the call*/

    userData.JavaEnv = pEnv;
    userData.Generator = generatorInstance;

    cLibErrorNum = DszCLibPrinterPrintIntWithUserData(
        printer,
        (void*) &userData);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}

JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintString(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong printerImpl,
    jobject generatorInstance)
{
    DszCLibPrinter printer = (DszCLibPrinter) printerImpl;
    DszCLibErrorNum cLibErrorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    DszJavaLibCorePrinterUserData userData = {
        .JavaEnv = NULL,
        .Generator = NULL
    };

    (void) printerClass;

    if (pEnv == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if ((printer == NULL) || (printer == DSZ_CLIB_PRINTER_INVALID))
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    if (generatorInstance == NULL)
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_JAVALIB_JNI_ERROR));

    /* note: if design requires executing printInt in a different thread/process space,
        userData must be kept alive longer than the call*/

    userData.JavaEnv = pEnv;
    userData.Generator = generatorInstance;

    cLibErrorNum = DszCLibPrinterPrintStringWithUserData(
        printer,
        (void*) &userData);

    if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        DszJavaLibCoreErrorNum errorNum = DszJavaLibCoreConvertCLibErrorNum(cLibErrorNum);
        return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(errorNum));
    }

    return (DSZ_JAVALIBCORE_RETURN_ERRORNUM(DSZ_JAVALIBCORE_ERRORNUM_NO_ERROR));
}
