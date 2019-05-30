#include <clib/clib.h>

#include <stdio.h>
#include <string.h>

int main(void)
{
    CLibErrNum error;
    char errorMessage[40];
    CLibAddress address = NULL;
    char addressString[256];
    CLibPerson person = NULL;
    char personString[512];

    CLLibraryInitialize();

    fprintf(stdout, "Library initialized... version %s\n", CLLibraryGetVersionString());

    fprintf(stdout, "Creating a new address...\n");
    error = CLAddressCreate(9898, "Corner St.", "Gotham", "CA", "Antartica", "4132", &address);
    if (error != 0) {
        memset(errorMessage, 0, 40);
        CLErrNumGetMessage(error, errorMessage, 40);
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }
    fprintf(stdout, "New address created!\n");

    memset(addressString, 0, 256);
    CLAddressToString(address, addressString, 256, NULL);
    fprintf(stdout, "Address:\n%s\n", addressString);

    fprintf(stdout, "Creating a new person...\n");
    error = CLPersonCreate("Wayne", "Bruce", 25, address, &person);
    if (error != 0) {
        memset(errorMessage, 0, 40);
        CLErrNumGetMessage(error, errorMessage, 40);
        fprintf(stderr, "An error has occurred: %s\n", errorMessage);
        return (-1);
    }
    fprintf(stdout, "New person created!\n");

    memset(personString, 0, 512);
    CLPersonToString(person, personString, 512, NULL);
    fprintf(stdout, "Person:\n%s\n", personString);

    CLPersonDestroy(person);
    CLAddressDestroy(address);

    CLLibraryTerminate();

    return (0);
}
