#include <dotslashzero/clib/clib.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
    #define snprintf sprintf_s
    #undef strncpy
    #define strncpy(buffer, source, bufferSize) strncpy_s(buffer, bufferSize, source, _TRUNCATE)
#endif /* defined(_WIN32) */

static DszCLibErrorNum DSZ_CLIB_CALLING_CONVENTION GenerateInt(
    int data, int* pInt,
    void* pUserData)
{
    (void) pUserData;

    if (pInt == NULL)
        return ((DszCLibErrorNum) -1);

    *pInt = data * data;

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

static DszCLibErrorNum DSZ_CLIB_CALLING_CONVENTION GenerateString(
    int data, char* pString, size_t stringSize,
    size_t* pCharsWritten,
    void* pUserData)
{
#if defined(_MSC_VER)
    #define BUFFER_SIZE 64
#else /* defined(_MSC_VER) */
    size_t const BUFFER_SIZE = 64;
#endif /* defined(_MSC_VER) */

    char buffer[BUFFER_SIZE];
    size_t numChars = 0;

    (void) pUserData;

    memset(buffer, 0, BUFFER_SIZE);
    snprintf(buffer, BUFFER_SIZE, "%d", data);

    if ((pString != NULL) && (stringSize > 0)) {
        strncpy(pString, buffer, stringSize);
        pString[stringSize - 1] = '\0';
        numChars = strlen(pString);
    }
    else {
        numChars = strlen(buffer);
    }

    if (pCharsWritten != NULL)
        *pCharsWritten = numChars;

#if defined(_MSC_VER)
    #undef BUFFER_SIZE
#endif /* defined(_MSC_VER) */

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

int main(void)
{
#if defined(_MSC_VER)
    #define ERROR_MESSAGE_SIZE 40
    #define VERSION_STRING_SIZE 16
    #define ADDRESS_STRING_SIZE 256
    #define PERSON_STRING_SIZE 512
#else /* defined(_MSC_VER) */
    size_t const ERROR_MESSAGE_SIZE = 40;
    size_t const VERSION_STRING_SIZE = 16;
    size_t const ADDRESS_STRING_SIZE = 256;
    size_t const PERSON_STRING_SIZE = 512;
#endif /* defined(_MSC_VER) */

    DszCLibErrorNum errorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    char errorMessage[ERROR_MESSAGE_SIZE];
    char versionString[VERSION_STRING_SIZE];
    DszCLibAddress address = DSZ_CLIB_ADDRESS_INVALID;
    char addressString[ADDRESS_STRING_SIZE];
    DszCLibPerson person = DSZ_CLIB_PERSON_INVALID;
    char personString[PERSON_STRING_SIZE];
    DszCLibGenerator generator = DSZ_CLIB_GENERATOR_INVALID; /* note that this will be owned by printer */
    DszCLibPrinter printer = DSZ_CLIB_PRINTER_INVALID;

    DszCLibLibraryInitialize();

    memset(versionString, 0, VERSION_STRING_SIZE);
    DszCLibLibraryGetVersionString(versionString, VERSION_STRING_SIZE, NULL);

    fprintf(stdout, "Library initialized... version %s\n", versionString);

    do {
        fprintf(stdout, "Creating a new address...\n");
        
        errorNum = DszCLibAddressCreate(9898, "Corner St.", "Gotham", "CA", "4132", "Antartica", &address);
        if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
            break;

        fprintf(stdout, "New address created!\n");

        memset(addressString, 0, ADDRESS_STRING_SIZE);
        errorNum = DszCLibAddressToString(address, addressString, ADDRESS_STRING_SIZE, NULL);
        if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
            break;

        fprintf(stdout, "Address:\n%s\n", addressString);

        fprintf(stdout, "Creating a new person...\n");

        errorNum = DszCLibPersonCreate("Wayne", "Bruce", 25, address, &person);
        if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
            break;

        fprintf(stdout, "New person created!\n");

        memset(personString, 0, PERSON_STRING_SIZE);
        errorNum = DszCLibPersonToString(person, personString, 512, NULL);
        if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
            break;

        fprintf(stdout, "Person:\n%s\n", personString);

        fprintf(stdout, "Creating a new generator...\n");

        errorNum = DszCLibGeneratorCreate(
            &GenerateInt,
            &GenerateString,
            &generator);

        if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
            break;

        fprintf(stdout, "New generator created!\n");

        fprintf(stdout, "Creating a new printer...\n");

        errorNum = DszCLibPrinterCreate(
            generator,
            &printer);

        if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
            break;

        /* generator is now owned by printer */
        generator = DSZ_CLIB_GENERATOR_INVALID;
        fprintf(stdout, "New printer created!\n");

        fprintf(stdout, "Performing printer actions...\n");
        DszCLibPrinterPrintInt(printer);
        DszCLibPrinterPrintString(printer);
    }
    while (false);

    if (printer != DSZ_CLIB_PRINTER_INVALID)
        DszCLibPrinterDestroy(printer);

    if (generator != DSZ_CLIB_GENERATOR_INVALID)
        DszCLibGeneratorDestroy(generator);

    if (person != DSZ_CLIB_PERSON_INVALID)
        DszCLibPersonDestroy(person);

    if (address != DSZ_CLIB_ADDRESS_INVALID)
        DszCLibAddressDestroy(address);

    if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        memset(errorMessage, 0, ERROR_MESSAGE_SIZE);
        DszCLibErrorNumGetMessage(errorNum, errorMessage, ERROR_MESSAGE_SIZE, NULL);
    }

    DszCLibLibraryUninitialize();

#if defined(_MSC_VER)
    #undef ERROR_MESSAGE_SIZE
    #undef VERSION_STRING_SIZE
    #undef ADDRESS_STRING_SIZE
    #undef PERSON_STRING_SIZE
#endif /* defined(_MSC_VER) */

    if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }

    return (0);
}
