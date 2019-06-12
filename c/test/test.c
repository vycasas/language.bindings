#include <dotslashzero/clib/clib.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#define snprintf sprintf_s
#endif /* defined(_WIN32) */

static DszCLibErrorNum DSZ_CLIB_CALLING_CONVENTION GenerateInt(
    int data, int* pInt)
{
    if (pInt == NULL) return (-1);

    *pInt = data * data;

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

static DszCLibErrorNum DSZ_CLIB_CALLING_CONVENTION GenerateString(
    int data, char* pString, size_t stringSize,
    size_t* pCharsWritten)
{
    size_t const BUFFER_SIZE = 64;
    char buffer[BUFFER_SIZE];
    size_t numChars = 0;

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

    return (DSZ_CLIB_ERRORNUM_NO_ERROR);
}

int main(void)
{
    DszCLibErrorNum errorNum = DSZ_CLIB_ERRORNUM_NO_ERROR;
    size_t const ERROR_MESSAGE_SIZE = 40;
    char errorMessage[ERROR_MESSAGE_SIZE];
    size_t const VERSION_STRING_SIZE = 16;
    char versionString[VERSION_STRING_SIZE];
    DszCLibAddress address = DSZ_CLIB_ADDRESS_INVALID;
    size_t const ADDRESS_STRING_SIZE = 256;
    char addressString[ADDRESS_STRING_SIZE];
    DszCLibPerson person = DSZ_CLIB_PERSON_INVALID;
    size_t const PERSON_STRING_SIZE = 512;
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

    if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR) {
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }

    return (0);
}
