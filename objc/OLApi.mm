#import "OLApi.h"

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

    class OBJCLibException final : public CXXLib::Exception
    {
    public:
        OBJCLibException(void) : CXXLib::Exception()
        { return; }

        OBJCLibException(CLibErrNum errNum) : CXXLib::Exception(errNum)
        { return; }
    }; // class OBJCLibException

    class OBJCLibGeneratorImpl final : public CXXLib::GeneratorBase
    {
    public:
        OBJCLibGeneratorImpl(OLGenerator* impl);

        virtual ~OBJCLibGeneratorImpl(void) override;

        virtual int generateInt(int data) const override;
        virtual std::string generateString(int data) const override;
    private:
        void* _impl;
    }; // class OBJCLibGeneratorImpl

    OBJCLibGeneratorImpl::OBJCLibGeneratorImpl(OLGenerator* impl) :
        _impl(nullptr)
    {
        _impl = (__bridge_retained void*) impl;
        return;
    }

    OBJCLibGeneratorImpl::~OBJCLibGeneratorImpl(void)
    {
        // give _impl back to ARC so that it can be collected
        (void)((__bridge_transfer OLGenerator*) _impl);
        return;
    }

    int OBJCLibGeneratorImpl::generateInt(int data) const
    {
        if (_impl == nil) {
            throw (OBJCLibCore::OBJCLibException(2));
        }
        @try {
            return ([(__bridge OLGenerator*) _impl generateIntWithData: data]);
        }
        @catch (NSException*) {
            throw (OBJCLibCore::OBJCLibException(2));
        }
    }

    std::string OBJCLibGeneratorImpl::generateString(int data) const
    {
        if (_impl == nil) {
            throw (OBJCLibCore::OBJCLibException(2));
        }
        @try {
            // note: perhaps it is a good idea to make this part synchronized
            NSString* callbackResult = [(__bridge OLGenerator*) _impl generateStringWithData: data];
            return (OBJCLibCore::Utils::NSStringToCXXStdString(callbackResult));
        }
        @catch (NSException*) {
            throw (OBJCLibCore::OBJCLibException(2));
        }
    }
} // namespace OBJCLibCore

extern "C"
{

@interface OLException()
{
    CXXLib::Exception* _impl;
}
- (CXXLib::Exception*) getImpl;
@end // interface OLException

@interface OLAddress()
{
    CXXLib::Address* _impl;
}
- (CXXLib::Address*) getImpl;
@end // interface OLAddress

@interface OLPerson()
{
    CXXLib::Person* _impl;
}
- (CXXLib::Person*) getImpl;
@end // interface OLPerson

@interface OLPrinter()
{
    CXXLib::Printer* _impl;
}
- (CXXLib::Printer*) getImpl;
@end // interface OLPrinter

@implementation OLException

- (CXXLib::Exception*) getImpl
{
    return (_impl);
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

- (CXXLib::Address*) getImpl
{
    return (_impl);
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

@implementation OLPerson

- (CXXLib::Person*) getImpl
{
    return (_impl);
}

- (id) initWithLastName:(NSString*)lastName firstName:(NSString*)firstName
    age:(int)age address:(OLAddress*)address
{
    self = [super init];
    if (self != nil) {
        _impl = new CXXLib::Person(
            OBJCLibCore::Utils::NSStringToCXXStdString(lastName),
            OBJCLibCore::Utils::NSStringToCXXStdString(firstName),
            age,
            *[address getImpl]
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
    }
    return;
}

- (NSString*) toString
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->toString()));
    END_EX_GUARD;
}

- (NSString*) lastName
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getLastName()));
    END_EX_GUARD;
}

- (NSString*) firstName
{
    BEGIN_EX_GUARD;
    return (OBJCLibCore::Utils::CXXStdStringToNSString(_impl->getFirstName()));
    END_EX_GUARD;   
}

- (int) age
{
    BEGIN_EX_GUARD;
    return (_impl->getAge());
    END_EX_GUARD;
}

- (OLAddress*) address
{
    BEGIN_EX_GUARD;
    auto coreAddr = _impl->getAddress();
    OLAddress* result = [[OLAddress alloc]
        initWithStreetNum: coreAddr.getStreetNum()
        street: OBJCLibCore::Utils::CXXStdStringToNSString(coreAddr.getStreet())
        city: OBJCLibCore::Utils::CXXStdStringToNSString(coreAddr.getCity())
        province: OBJCLibCore::Utils::CXXStdStringToNSString(coreAddr.getProvince())
        country: OBJCLibCore::Utils::CXXStdStringToNSString(coreAddr.getCountry())
        zipCode: OBJCLibCore::Utils::CXXStdStringToNSString(coreAddr.getZipCode())
    ];
    return (result);
    END_EX_GUARD;
}

@end // implementation OLPerson

@implementation OLGenerator
- (void) dealloc
{
    return;
}

- (int) generateIntWithData:(int)data
{
    (void)data;
    OBJCLibCore::OBJCLibException ex(2);
    @throw ([OLException createFromCXXLibException: &ex]);
}

- (NSString*) generateStringWithData:(int)data
{
    (void)data;
    OBJCLibCore::OBJCLibException ex(2);
    @throw ([OLException createFromCXXLibException: &ex]);
}

@end // implementation OLGenerator

@implementation OLPrinter

- (CXXLib::Printer*) getImpl
{
    return (_impl);
}

- (id) initWithGenerator:(OLGenerator*)generator
{
    self = [super init];
    if (self != nil) {
        std::unique_ptr<OBJCLibCore::OBJCLibGeneratorImpl> coreGenerator(
            new OBJCLibCore::OBJCLibGeneratorImpl(generator)
        );
        _impl = new CXXLib::Printer(std::move(coreGenerator));
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
    }
    return;
}

- (void) printInt
{
    BEGIN_EX_GUARD;
    _impl->printInt();
    END_EX_GUARD;
    return;
}

- (void) printString
{
    BEGIN_EX_GUARD;
    _impl->printString();
    END_EX_GUARD;
    return;
}
@end // implementation OLPrinter

} // extern "C"
