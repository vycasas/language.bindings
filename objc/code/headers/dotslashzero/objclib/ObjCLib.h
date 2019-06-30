#import <Foundation/Foundation.h>

#if defined(__cplusplus)
extern "C"
{
#endif /* defined(__cplusplus) */

@interface OLException : NSException
@end /* interface OLException */

@interface OLLibrary : NSObject

+ (BOOL) initialize;
+ (void) uninitialize;
+ (NSString*) getVersionString;
+ (NSUInteger) getVersionMajor;
+ (NSUInteger) getVersionMinor;
+ (NSUInteger) getVersionPatch;
+ (NSString*) getVersionExtra;

@end /* interface OLLibrary */

@interface OLAddress : NSObject

- (instancetype) initWithStreetNum: (int) streetNum
    street: (NSString*) street
    city: (NSString*) city
    province: (NSString* )province
    zipCode: (NSString*) zipCode
    country: (NSString*) country;
- (void) dealloc;

@property (readonly) int streetNum;
@property (readonly) NSString* street;
@property (readonly) NSString* city;
@property (readonly) NSString* province;
@property (readonly) NSString* zipCode;
@property (readonly) NSString* country;

- (NSString*) toString;

@end /* interface OLAddress */

@interface OLPerson : NSObject

- (instancetype) initWithLastName: (NSString*) lastName
    firstName: (NSString*) firstName
    age: (int) age
    address: (OLAddress*) address;
- (void) dealloc;

@property (readonly) NSString* lastName;
@property (readonly) NSString* firstName;
@property (readonly) int age;
@property (readonly) OLAddress* address;

- (NSString*) toString;

@end /* interface OLPerson */

@protocol OLGenerator <NSObject>

@required
- (void) dealloc;

- (int) generateIntWithData: (int) data;
- (NSString*) generateStringWithData: (int) data;

@end /* protocol OLGenerator */

@interface OLPrinter : NSObject

- (instancetype) initWithGenerator: (id<OLGenerator>) generator;
- (void) dealloc;

- (void) printInt;
- (void) printString;

@end /* interface OLPrinter */

#if defined(__cplusplus)
}
/* extern "C" */
#endif /* defined(__cplusplus) */
