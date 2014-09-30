#import <Foundation/Foundation.h>

#import <objc/OLApi.h>

@interface MyGenerator : OLGenerator
- (id) init;
- (void) dealloc;
- (int) generateIntWithData:(int)data;
- (NSString*) generateStringWithData:(int)data;
@end

@implementation MyGenerator
- (id) init
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
@end

int main(void)
{
    @try {
        [OLLibrary initialize];

        NSLog(@"Creating a new Address...");
        OLAddress* address = [[OLAddress alloc]
            initWithStreetNum:9898
            street:@"Corner St."
            city:@"Gotham"
            province:@"CA"
            country:@"Antartica"
            zipCode:@"4132"
        ];
        NSLog(@"New address created!");
        NSLog(@"\nAddress:\n%@", [address toString]);

        NSLog(@"Creating a new Person...");
        OLPerson* person = [[OLPerson alloc]
            initWithLastName:@"Wayne"
            firstName:@"Bruce"
            age:25
            address:address
        ];
        NSLog(@"New person created!");
        NSLog(@"\nPerson:\n%@", [person toString]);

        NSLog(@"Creating a new generator...");
        MyGenerator* generator = [[MyGenerator alloc] init];
        NSLog(@"New generator created!");
        NSLog(@"Creating a new printer...");
        OLPrinter* printer = [[OLPrinter alloc] initWithGenerator: generator];
        NSLog(@"New printer created!");
        [printer printInt];
        [printer printString];

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
