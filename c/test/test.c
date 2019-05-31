#include <clib/clib.h>

#include <stdio.h>
#include <string.h>

void CLIB_CALLING_CONVENTION MyGetNumberCallbackFunction(int result)
{
    fprintf(stdout, "MyGetNumberCallbackFunction called with: %d\n", result);
    return;
}

int main(void)
{
    CLibErrNum error;
    char errorMessage[40];
    CLibAddress address = NULL;
    char addressString[256];
    CLibPerson person = NULL;
    char personString[512];

    CLibLibraryInitialize();

    fprintf(stdout, "Library initialized... version %s\n", CLibLibraryGetVersionString());

    fprintf(stdout, "Creating a new address...\n");
    error = CLibAddressCreate(9898, "Corner St.", "Gotham", "CA", "Antartica", "4132", &address);
    if (error != 0) {
        memset(errorMessage, 0, 40);
        CLibErrNumGetMessage(error, errorMessage, 40);
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }
    fprintf(stdout, "New address created!\n");

    memset(addressString, 0, 256);
    CLibAddressToString(address, addressString, 256, NULL);
    fprintf(stdout, "Address:\n%s\n", addressString);

    fprintf(stdout, "Creating a new person...\n");
    error = CLibPersonCreate("Wayne", "Bruce", 25, address, &person);
    if (error != 0) {
        memset(errorMessage, 0, 40);
        CLibErrNumGetMessage(error, errorMessage, 40);
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }
    fprintf(stdout, "New person created!\n");

    memset(personString, 0, 512);
    CLibPersonToString(person, personString, 512, NULL);
    fprintf(stdout, "Person:\n%s\n", personString);

    CLibPersonDestroy(person);
    CLibAddressDestroy(address);

    CLibGetNumber((CLibGetNumberCallbackFunction*) &MyGetNumberCallbackFunction);

    CLibLibraryTerminate();

    return (0);
}
