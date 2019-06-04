#include <dotslashzero/clib/clib.h>

#include <stdio.h>
#include <string.h>

void DSZ_CLIB_CALLING_CONVENTION MyGetNumberCallbackFunction(int result)
{
    fprintf(stdout, "MyGetNumberCallbackFunction called with: %d\n", result);
    return;
}

int main(void)
{
    DszCLibErrNum error;
    char errorMessage[40];
    DszCLibAddress address = NULL;
    char addressString[256];
    DszCLibPerson person = NULL;
    char personString[512];

    DszCLibLibraryInitialize();

    fprintf(stdout, "Library initialized... version %s\n", DszCLibLibraryGetVersionString());

    fprintf(stdout, "Creating a new address...\n");
    error = DszCLibAddressCreate(9898, "Corner St.", "Gotham", "CA", "Antartica", "4132", &address);
    if (error != 0) {
        memset(errorMessage, 0, 40);
        DszCLibErrNumGetMessage(error, errorMessage, 40);
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }
    fprintf(stdout, "New address created!\n");

    memset(addressString, 0, 256);
    DszCLibAddressToString(address, addressString, 256, NULL);
    fprintf(stdout, "Address:\n%s\n", addressString);

    fprintf(stdout, "Creating a new person...\n");
    error = DszCLibPersonCreate("Wayne", "Bruce", 25, address, &person);
    if (error != 0) {
        memset(errorMessage, 0, 40);
        DszCLibErrNumGetMessage(error, errorMessage, 40);
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }
    fprintf(stdout, "New person created!\n");

    memset(personString, 0, 512);
    DszCLibPersonToString(person, personString, 512, NULL);
    fprintf(stdout, "Person:\n%s\n", personString);

    DszCLibPersonDestroy(person);
    DszCLibAddressDestroy(address);

    DszCLibGetNumber((DszCLibGetNumberCallbackFunction*) &MyGetNumberCallbackFunction);

    DszCLibLibraryTerminate();

    return (0);
}
