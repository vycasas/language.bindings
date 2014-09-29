#import "OLApi.h"

#include <iostream>
#include <mutex>
#include <cxx/api.hxx>

#define BEGIN_EX_GUARD \
    try {
#define END_EX_GUARD \
    } \
    catch (CXXLib::Exception& e) { \
        @throw ([OLException createFromCXXLibException: &e]); \
    } \
    catch (std::exception& e) { \
        CXXLib::Exception coreEx(e); \
        @throw ([OLException createFromCXXLibException: &coreEx]); \
    }

namespace OBJCLibCore
{
    std::mutex G_InitializedMutex;
    BOOL G_Initialized = NO;

    namespace Utils
    {
        inline NSString* CXXStdStringToNSString(const std::string& str)
        {
            // Assume that we are using UTF-8 throughout. Otherwise, take note of the locale.
            NSString* result = [[NSString alloc] initWithUTF8String:str.data()];
            return (result);
        }

        inline std::string NSStringToCXXStdString(NSString* str)
        {
            std::string result([str UTF8String]);
            return (result);
        }
    } // namespace Utils


    class OBJCLibException : public CXXLib::Exception
    {
    public:
        OBJCLibException(void) : CXXLib::Exception()
        { return; }

        OBJCLibException(CLibErrNum errNum) : CXXLib::Exception(errNum)
        { return; }
    }; // class
} // namespace OBJCLibCore

extern "C"
{

@implementation OLException
{
    CXXLib::Exception* _impl;
}

- (id) initWithCXXLibException:(CXXLib::Exception*)exception
{
    std::unique_ptr<CXXLib::Exception> coreExPtr(new CXXLib::Exception(*exception));
    self = [super initWithName:@"CXXLib::Exception"
        reason:OBJCLibCore::Utils::CXXStdStringToNSString(coreExPtr->getMessage())
        userInfo:nil
    ];
    if (self != nil) {
        _impl = coreExPtr.release();
    }
    else {
        // do some error checks...
    }
    return (self);
}

- (void) dealloc
{
    if (_impl != nullptr) {
        std::unique_ptr<CXXLib::Exception> ownedPtr(_impl);
    }
    [super dealloc];
    return;
}

- (NSString*) getMessage
{
    if (_impl != nullptr) {
        NSString* message = OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getMessage());
        return (message);
    }
    return (nil);
}

+ (OLException*) createFromCXXLibException:(CXXLib::Exception*)exception
{
    OLException* ex = [[OLException alloc] initWithCXXLibException:exception];
    return (ex);
}
@end // implementation OLException

@implementation OLLibrary
+ (void) initialize
{
    if (self == [OLLibrary class]) {
        std::unique_lock<std::mutex> lock(OBJCLibCore::G_InitializedMutex);
        if (OBJCLibCore::G_Initialized == NO) {
            CXXLib::Library::initialize();
            OBJCLibCore::G_Initialized = YES;
        }
    }
    return;
}

+ (void) terminate
{
    std::unique_lock<std::mutex> lock(OBJCLibCore::G_InitializedMutex);
    if (OBJCLibCore::G_Initialized == YES) {
        CXXLib::Library::terminate();
        OBJCLibCore::G_Initialized = NO;
    }
    return;
}
@end // implementation OLLibrary

@implementation OLAddress
{
    CXXLib::Address* _impl;
}

- (id) initWithStreetNum:(int)streetNum street:(NSString*)street
    city:(NSString*)city province:(NSString*)province
    country:(NSString*)country zipCode:(NSString*)zipCode
{
    self = [super init];
    if (self != nil) {
        _impl = new CXXLib::Address(
            streetNum,
            OBJCLibCore::Utils::NSStringToCXXStdString(street),
            OBJCLibCore::Utils::NSStringToCXXStdString(city),
            OBJCLibCore::Utils::NSStringToCXXStdString(province),
            OBJCLibCore::Utils::NSStringToCXXStdString(country),
            OBJCLibCore::Utils::NSStringToCXXStdString(zipCode)
        );
    }
    else {
        OBJCLibCore::OBJCLibException ex(2);
        @throw ([OLException createFromCXXLibException: &ex]);
    }
    return (self);
}

- (void) dealloc
{
    if (self != nil) {
        if (_impl != nullptr) {
            delete (_impl);
        }
        [super dealloc];
    }
    return;
}

- (NSString*) toString
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->toString()));
    END_EX_GUARD;
}

- (int) streetNum
{
    BEGIN_EX_GUARD;
    return (_impl->getStreetNum());
    END_EX_GUARD;
}

- (NSString*) street
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getStreet()));
    END_EX_GUARD;
}

- (NSString*) city
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getCity()));
    END_EX_GUARD;
}

- (NSString*) province
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getProvince()));
    END_EX_GUARD;
}

- (NSString*) country
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getCountry()));
    END_EX_GUARD; 
}

- (NSString*) zipCode
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getZipCode()));
    END_EX_GUARD;
}

@end // implementation OLAddress

} // extern "C"