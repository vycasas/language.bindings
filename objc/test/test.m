#import <Foundation/Foundation.h>

#import <objc/OLApi.h>

int main(void)
{
    @try {
        [OLLibrary initialize];

        NSLog(@"Creating a new Address...");
        OLAddress* address = [
            [OLAddress alloc] initWithStreetNum:9898 street:@"Corner St."
            city:@"Gotham" province:@"CA" country:@"Antartica" zipCode:@"4132"
        ];
        NSLog(@"New address created!");
        NSLog(@"Address: %@", [address toString]);

        [OLLibrary terminate];
    }
    @catch (OLException* e) {
        NSLog(@"OLException: %@", [e getMessage]);
    }
    @catch (NSException* e) {
        NSLog(@"NSException: %@ %@", [e name], [e reason]);
    }

    return (0);
}
