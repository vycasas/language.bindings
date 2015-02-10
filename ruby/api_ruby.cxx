#include "api_ruby.hxx"

#include <iostream>
#include <memory>

#include <cxx/api.hxx>

#define BEGIN_EX_GUARD \
    try {
#define END_EX_GUARD \
    } \
    catch (CXXLib::Exception& e) { \
        return (INT2NUM(static_cast<int>(RubyLibCore::RubyLibException(e).getErrorNum()))); \
    } \
    catch (std::exception& e) { \
        return (INT2NUM(2)); \
    } \
    catch (...) { \
        return (INT2NUM(2)); \
    }

namespace RubyLibCore
{
    namespace Utils
    {
        template<typename T>
        inline T* GetCorePtr(VALUE value)
        {
            auto address = NUM2SIZET(value);
            return (reinterpret_cast<T*>(address));
        }
    } // namespace Utils

    // Internal references
    namespace Definitions
    {
        static VALUE RubyLibNativeModule = Qnil;
        static VALUE RubyLibErrorModule = Qnil;
        static VALUE LibraryModule = Qnil;
        static VALUE AddressModule = Qnil;
        static VALUE PersonModule = Qnil;
        static VALUE PrinterModule = Qnil;
    } // namespace Definitions

    class RubyLibException : public CXXLib::Exception
    {
    public:
        RubyLibException(void) : CXXLib::Exception() { return; }
        RubyLibException(CLibErrNum errNum) : CXXLib::Exception(errNum) { return; }
        RubyLibException(CXXLib::Exception e) : CXXLib::Exception(e) { return; }
        CLibErrNum getErrorNum(void) { return (_impl); }
    }; // class RubyLibException

    class RubyLibGeneratorImpl final : public CXXLib::GeneratorBase
    {
    public:
        RubyLibGeneratorImpl(VALUE impl);

        virtual ~RubyLibGeneratorImpl(void) override;

        virtual int generateInt(int data) const override;
        virtual std::string generateString(int data) const override;

        VALUE _impl;
    }; // class RubyLibGeneratorImpl

    RubyLibGeneratorImpl::RubyLibGeneratorImpl(VALUE impl) : _impl(Qnil)
    {
        _impl = impl;
        return;
    }

    RubyLibGeneratorImpl::~RubyLibGeneratorImpl(void)
    {
        if (_impl != Qnil) {
            // delete impl
            _impl = Qnil;
        }
        return;
    }

    int RubyLibGeneratorImpl::generateInt(int data) const
    {
        VALUE result = rb_funcall(_impl, rb_intern("generate_int"), 1, INT2NUM(data));
        return (NUM2INT(result));
    }

    std::string RubyLibGeneratorImpl::generateString(int data) const
    {
        VALUE result = rb_funcall(_impl, rb_intern("generate_string"), 1, INT2NUM(data));
        return (std::string(StringValueCStr(result)));
    }

} // namespace RubyLibCore

extern "C"
{

RUBYLIB_API
void Init_rubylibnative(void)
{
    using namespace RubyLibCore::Definitions;

    // Initialize module and classes    
    RubyLibNativeModule = rb_define_module("RubyLibNative");

    // RubyLibError
    RubyLibErrorModule = rb_define_module_under(RubyLibNativeModule, "RubyLibError");
    rb_define_module_function(RubyLibErrorModule, "get_message", RUBY_METHOD_FUNC(RLRubyLibErrorGetMessage), 1);

    // Library
    LibraryModule = rb_define_module_under(RubyLibNativeModule, "Library");
    rb_define_module_function(LibraryModule, "initialize", RUBY_METHOD_FUNC(RLLibraryInitialize), 0);
    rb_define_module_function(LibraryModule, "terminate", RUBY_METHOD_FUNC(RLLibraryTerminate), 0);
    rb_define_module_function(LibraryModule, "get_version_string", RUBY_METHOD_FUNC(RLLibraryGetVersionString), 1);
    rb_define_module_function(LibraryModule, "get_version_major", RUBY_METHOD_FUNC(RLLibraryGetVersionMajor), 1);
    rb_define_module_function(LibraryModule, "get_version_minor", RUBY_METHOD_FUNC(RLLibraryGetVersionMinor), 1);

    // Address
    AddressModule = rb_define_module_under(RubyLibNativeModule, "Address");
    rb_define_module_function(AddressModule, "create", RUBY_METHOD_FUNC(RLAddressCreate), 1);
    rb_define_module_function(AddressModule, "destroy", RUBY_METHOD_FUNC(RLAddressDestroy), 1);
    rb_define_module_function(AddressModule, "get_street_num", RUBY_METHOD_FUNC(RLAddressGetStreetNum), 1);
    rb_define_module_function(AddressModule, "get_street", RUBY_METHOD_FUNC(RLAddressGetStreet), 1);
    rb_define_module_function(AddressModule, "get_city", RUBY_METHOD_FUNC(RLAddressGetCity), 1);
    rb_define_module_function(AddressModule, "get_province", RUBY_METHOD_FUNC(RLAddressGetProvince), 1);
    rb_define_module_function(AddressModule, "get_country", RUBY_METHOD_FUNC(RLAddressGetCountry), 1);
    rb_define_module_function(AddressModule, "get_zip_code", RUBY_METHOD_FUNC(RLAddressGetZipCode), 1);

    // Person
    PersonModule = rb_define_module_under(RubyLibNativeModule, "Person");
    rb_define_module_function(PersonModule, "create", RUBY_METHOD_FUNC(RLPersonCreate), 1);
    rb_define_module_function(PersonModule, "destroy", RUBY_METHOD_FUNC(RLPersonDestroy), 1);
    rb_define_module_function(PersonModule, "get_last_name", RUBY_METHOD_FUNC(RLPersonGetLastName), 1);
    rb_define_module_function(PersonModule, "get_first_name", RUBY_METHOD_FUNC(RLPersonGetFirstName), 1);
    rb_define_module_function(PersonModule, "get_age", RUBY_METHOD_FUNC(RLPersonGetAge), 1);
    rb_define_module_function(PersonModule, "get_address_impl", RUBY_METHOD_FUNC(RLPersonGetAddressImpl), 1);

    // Printer
    PrinterModule = rb_define_module_under(RubyLibNativeModule, "Printer");
    rb_define_module_function(PrinterModule, "create", RUBY_METHOD_FUNC(RLPrinterCreate), 1);
    rb_define_module_function(PrinterModule, "destroy", RUBY_METHOD_FUNC(RLPrinterDestroy), 1);
    rb_define_module_function(PrinterModule, "print_int", RUBY_METHOD_FUNC(RLPrinterPrintInt), 1);
    rb_define_module_function(PrinterModule, "print_string", RUBY_METHOD_FUNC(RLPrinterPrintString), 1);

    return;
}

RUBYLIB_API
VALUE RLRubyLibErrorGetMessage(VALUE, VALUE)
{
    BEGIN_EX_GUARD;
    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLLibraryInitialize(VALUE)
{
    BEGIN_EX_GUARD;
    CXXLib::Library::initialize();
    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLLibraryTerminate(VALUE)
{
    try {
        CXXLib::Library::terminate();
    }
    catch (...) { }
    return (INT2NUM(0));
}

RUBYLIB_API
VALUE RLLibraryGetVersionString(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;

    rb_hash_aset(
        args,
        rb_str_new_cstr("version_string"),
        rb_str_new_cstr(CXXLib::Library::getVersionString().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLLibraryGetVersionMajor(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;

    rb_hash_aset(
        args,
        rb_str_new_cstr("version_major"),
        SIZET2NUM(CXXLib::Library::getVersionMajor())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLLibraryGetVersionMinor(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;

    rb_hash_aset(
        args,
        rb_str_new_cstr("version_minor"),
        SIZET2NUM(CXXLib::Library::getVersionMinor())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressCreate(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;
    int streetNum;
    std::string street, city, province, country, zipCode;

    // street_num
    result = rb_hash_aref(args, rb_str_new_cstr("street_num"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    streetNum = NUM2INT(result);

    // street
    result = rb_hash_aref(args, rb_str_new_cstr("street"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    street = std::string(StringValueCStr(result));

    // city
    result = rb_hash_aref(args, rb_str_new_cstr("city"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    city = std::string(StringValueCStr(result));

    // province
    result = rb_hash_aref(args, rb_str_new_cstr("province"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    province = std::string(StringValueCStr(result));

    // country
    result = rb_hash_aref(args, rb_str_new_cstr("country"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    country = std::string(StringValueCStr(result));

    // zip_code
    result = rb_hash_aref(args, rb_str_new_cstr("zip_code"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    zipCode = std::string(StringValueCStr(result));

    std::unique_ptr<CXXLib::Address> coreAddressPtr(
        new CXXLib::Address(
            streetNum, street, city, province, country, zipCode
        )
    );
    rb_hash_aset(args, rb_str_new_cstr("impl"), SIZET2NUM(reinterpret_cast<std::size_t>(coreAddressPtr.get())));
    coreAddressPtr.release();

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressDestroy(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result != Qnil) {
        std::unique_ptr<CXXLib::Address> ownedPtr(
            RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result)
        );
    }

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressGetStreetNum(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* coreAddressPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("street_num"),
        INT2NUM(coreAddressPtr->getStreetNum())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressGetStreet(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* coreAddressPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("street"),
        rb_str_new_cstr(coreAddressPtr->getStreet().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressGetCity(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* coreAddressPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("city"),
        rb_str_new_cstr(coreAddressPtr->getCity().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressGetProvince(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* coreAddressPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("province"),
        rb_str_new_cstr(coreAddressPtr->getProvince().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressGetCountry(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* coreAddressPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("country"),
        rb_str_new_cstr(coreAddressPtr->getCountry().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLAddressGetZipCode(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* coreAddressPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("zip_code"),
        rb_str_new_cstr(coreAddressPtr->getZipCode().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPersonCreate(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;
    std::string lastName, firstName;
    int age;
    CXXLib::Address* coreAddressPtr;

    result = rb_hash_aref(args, rb_str_new_cstr("last_name"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    lastName = std::string(StringValueCStr(result));

    result = rb_hash_aref(args, rb_str_new_cstr("first_name"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    firstName = std::string(StringValueCStr(result));

    result = rb_hash_aref(args, rb_str_new_cstr("age"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    age = NUM2INT(result);

    result = rb_hash_aref(args, rb_str_new_cstr("address_impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }
    coreAddressPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Address>(result);

    std::unique_ptr<CXXLib::Person> corePersonPtr(
        new CXXLib::Person(lastName, firstName, age, *coreAddressPtr)
    );
    rb_hash_aset(args, rb_str_new_cstr("impl"), SIZET2NUM(reinterpret_cast<std::size_t>(corePersonPtr.get())));
    corePersonPtr.release();

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPersonDestroy(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result != Qnil) {
        std::unique_ptr<CXXLib::Person> ownedPtr(
            RubyLibCore::Utils::GetCorePtr<CXXLib::Person>(result)
        );
    }

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPersonGetLastName(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* corePersonPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Person>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("last_name"),
        rb_str_new_cstr(corePersonPtr->getLastName().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPersonGetFirstName(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* corePersonPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Person>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("first_name"),
        rb_str_new_cstr(corePersonPtr->getFirstName().c_str())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPersonGetAge(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* corePersonPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Person>(result);
    rb_hash_aset(
        args,
        rb_str_new_cstr("age"),
        INT2NUM(corePersonPtr->getAge())
    );

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPersonGetAddressImpl(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* corePersonPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Person>(result);

    std::unique_ptr<CXXLib::Address> coreAddressPtr(
        new CXXLib::Address(corePersonPtr->getAddress())
    );
    rb_hash_aset(
        args,
        rb_str_new_cstr("address_impl"),
        SIZET2NUM(reinterpret_cast<std::size_t>(coreAddressPtr.get()))
    );
    coreAddressPtr.release();
    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPrinterCreate(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("generator"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    std::unique_ptr<RubyLibCore::RubyLibGeneratorImpl> generator(new RubyLibCore::RubyLibGeneratorImpl(result));
    std::unique_ptr<CXXLib::Printer> corePrinterPtr(new CXXLib::Printer(std::move(generator)));

    rb_hash_aset(args, rb_str_new_cstr("impl"), SIZET2NUM(reinterpret_cast<std::size_t>(corePrinterPtr.get())));
    corePrinterPtr.release();

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPrinterDestroy(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result != Qnil) {
        std::unique_ptr<CXXLib::Printer> ownedPtr(
            RubyLibCore::Utils::GetCorePtr<CXXLib::Printer>(result)
        );
    }

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPrinterPrintInt(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* corePrinterPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Printer>(result);
    corePrinterPtr->printInt();

    return (INT2NUM(0));
    END_EX_GUARD;
}

RUBYLIB_API
VALUE RLPrinterPrintString(VALUE, VALUE args)
{
    BEGIN_EX_GUARD;
    VALUE result = Qnil;

    result = rb_hash_aref(args, rb_str_new_cstr("impl"));
    if (result == Qnil) {
        throw (RubyLibCore::RubyLibException(1));
    }

    auto* corePrinterPtr = RubyLibCore::Utils::GetCorePtr<CXXLib::Printer>(result);
    corePrinterPtr->printString();

    return (INT2NUM(0));
    END_EX_GUARD;
}

} // extern "C"
