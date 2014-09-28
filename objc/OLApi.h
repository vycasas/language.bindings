#if !defined(OBJCLIB_API_H)
#define OBJCLIB_API_H

#import <Foundation/Foundation.h>

#if defined(__cplusplus)
extern "C"
{
#endif /* defined(__cplusplus) */ 

@interface OLException : NSException
- (void) dealloc;
- (NSString*) getMessage;
@end /* interface OLException */

@interface OLLibrary : NSObject
+ (void) initialize;
+ (void) terminate;
@end /* interface OLLibrary */

@interface OLAddress : NSObject

- (id) initWithStreetNum:(int)streetNum street:(NSString*)street
    city:(NSString*)city province:(NSString*)province
    country:(NSString*)country zipCode:(NSString*)zipCode;
- (void) dealloc;
- (NSString*) toString;

@property (readonly) int streetNum;
@property (readonly) NSString* street;
@property (readonly) NSString* city;
@property (readonly) NSString* province;
@property (readonly) NSString* country;
@property (readonly) NSString* zipCode;

@end /* interface OLAddress */

@interface OLPerson : NSObject

- (id) initWithLastName:(NSString*)lastName firstName:(NSString*)firstName
    age:(int)age address:(OLAddress*)address;
- (void) dealloc;
- (NSString*) toString;

@property (readonly) NSString* lastName;
@property (readonly) NSString* firstName;
@property (readonly) int age;
@property (readonly) OLAddress* address;

@end /* interface OLPerson */

@protocol OLGenerator <NSObject>

@required
- (void) dealloc;
- (int) generateIntWithData:(int)data;
- (NSString*) generateStringWithData:(int)data;

@end /* protocol Generator */

@interface OLPrinter : NSObject

- (id) initWithGenerator:(id<OLGenerator>*)generator;
- (void) dealloc;
- (void) printInt;
- (void) printString;

@end /* interface OLPrinter */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */

#endif /* !defined(OBJCLIB_API_H) */
