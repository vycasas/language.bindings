#import <ObjCLib/ObjCLib.h>

#import <Foundation/Foundation.h>

#if defined(__cplusplus)
    #error Please use pure C compiler for this file.
#endif /* defined(__cplusplus) */

@interface MyGenerator : NSObject<OLGenerator>

- (instancetype) init;
- (void) dealloc;
- (int) generateIntWithData:(int)data;
- (NSString*) generateStringWithData:(int)data;

@end /* interface MyGenerator */

@implementation MyGenerator

- (instancetype) init
{
    self = [super init];
    return (self);
}

- (void) dealloc
{
    return;
}

- (int) generateIntWithData:(int)data
{
    return (data * data);
}

- (NSString*) generateStringWithData:(int)data
{
    return ([NSString stringWithFormat:@"%d", data]);
}

@end /* implementation MyGenerator */

int main(void)
{
    @try {
        /* note: OLLibrary is implicitly initialized */

        NSLog(@"Library initialized... version %@\n", [OLLibrary getVersionString]);

        NSLog(@"Creating a new Address...");
        OLAddress* address = [[OLAddress alloc]
            initWithStreetNum:9898
            street:@"Corner St."
            city:@"Gotham"
            province:@"CA"
            zipCode:@"4132"
            country:@"Antartica"];
        NSLog(@"New address created!");

        NSLog(@"\nAddress:\n%@", [address toString]);

        NSLog(@"Creating a new Person...");
        OLPerson* person = [[OLPerson alloc]
            initWithLastName:@"Wayne"
            firstName:@"Bruce"
            age:25
            address:address];
        NSLog(@"New person created!");

        NSLog(@"\nPerson:\n%@", [person toString]);

        NSLog(@"Creating a new generator...");
        MyGenerator* generator = [[MyGenerator alloc] init];
        NSLog(@"New generator created!");

        NSLog(@"Creating a new printer...");
        OLPrinter* printer = [[OLPrinter alloc] initWithGenerator: generator];
        NSLog(@"New printer created!");

        NSLog(@"Performing printer actions...");
        [printer printInt];
        [printer printString];

        [OLLibrary uninitialize];
    }
    @catch (OLException* e) {
        NSLog(@"An error has occurred: %@ %@", [e name], [e reason]);
    }
    @catch (NSException* e) {
        NSLog(@"An error has occurred: %@ %@", [e name], [e reason]);
    }

    return (0);
}
