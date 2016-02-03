#include "api_nodejs.hxx"

#include <node_object_wrap.h>

#include <iostream>
#include <memory>
#include <stdexcept>

#include <cxx/api.hxx>

using namespace v8; // needed because some macros (in Node.js) expand to unqualified v8 namespace.

#define CREATE_ERROR(isolate, cxxLibException) \
    isolate->ThrowException(\
        v8::Exception::Error(\
            v8::String::NewFromUtf8(isolate, cxxLibException.getMessage().c_str())\
        )\
    );

#define BEGIN_EX_GUARD(isolate) \
    try {

#define END_EX_GUARD(isolate) \
    }\
    catch (const CXXLib::Exception& e) { \
        CREATE_ERROR(isolate, e);\
    } \
    catch (std::exception& e) { \
        CREATE_ERROR(isolate, CXXLib::Exception(e));\
    }

void Test(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::String> str = v8::String::NewFromUtf8(isolate, "OK!");
    args.GetReturnValue().Set(str);
    return;
}

namespace NJSLibCore
{
    namespace Utils
    {
        std::string V8StringToCXXString(const v8::Local<v8::String> v8Str)
        {
            auto strLen = v8Str->Utf8Length();
            std::string result(strLen, '\0');
            v8Str->WriteUtf8(&(result.front()));
            return (result);
        }
    }

    void LibraryInitialize(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        CXXLib::Library::initialize();
        END_EX_GUARD(isolate);

        return;
    }

    void LibraryTerminate(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        CXXLib::Library::terminate();
        END_EX_GUARD(isolate);

        return;
    }

    void LibraryGetVersionString(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto versionStr = CXXLib::Library::getVersionString();
        auto versionV8Str = v8::String::NewFromUtf8(isolate, versionStr.c_str());
        args.GetReturnValue().Set(versionV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void LibraryGetVersionMajor(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto versionV8Number = v8::Number::New(isolate, static_cast<double>(CXXLib::Library::getVersionMajor()));
        args.GetReturnValue().Set(versionV8Number);
        END_EX_GUARD(isolate);

        return;
    }

    void LibraryGetVersionMinor(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto versionV8Number = v8::Number::New(isolate, static_cast<double>(CXXLib::Library::getVersionMinor()));
        args.GetReturnValue().Set(versionV8Number);
        END_EX_GUARD(isolate);

        return;
    }

    class Address final : public node::ObjectWrap
    {
    public:
        Address(
            int streetNum, const std::string& street,
            const std::string& city, const std::string& province,
            const std::string& country, const std::string& zipCode
        )
        {
            _impl.reset(new CXXLib::Address(streetNum, street, city, province, country, zipCode));
            return;
        }

        ~Address(void) noexcept
        {
            // std::cout << __FILE__ << ":" << __LINE__ << " - " << __FUNCTION__ << std::endl;
            return;
        }

        int getStreetNum(void) const
        {
            return (_impl->getStreetNum());
        }

        std::string getStreet(void) const
        {
            return (_impl->getStreet());
        }

        std::string getCity(void) const
        {
            return (_impl->getCity());
        }

        std::string getProvince(void) const
        {
            return (_impl->getProvince());
        }

        std::string getCountry(void) const
        {
            return (_impl->getCountry());
        }

        std::string getZipCode(void) const
        {
            return (_impl->getZipCode());
        }

        std::string toString(void) const
        {
            return (_impl->toString());
        }

    private:
        Address()
        {
            return;
        }

        Address(Address&& oldAddress)
        {
            _impl.reset(oldAddress._impl.release());
            return;
        }

        std::unique_ptr<CXXLib::Address> _impl;

        friend class Person;

    public:
        // Node.js definitions for class wrap
        static void InitializeClass(v8::Local<v8::Object> exports);
        static void NewAddress(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetStreetNum(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetStreet(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetCity(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetProvince(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetCountry(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetZipCode(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void ToString(const v8::FunctionCallbackInfo<v8::Value>& args);

        static v8::Persistent<v8::Function> constructor;
    }; // class Address

    void Address::InitializeClass(v8::Local<v8::Object> exports)
    {
        auto* isolate = exports->GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        v8::Local<v8::FunctionTemplate> addressTemplate = v8::FunctionTemplate::New(isolate, NewAddress);
        addressTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Address"));
        addressTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(addressTemplate, "getStreetNum", Address::GetStreetNum);
        NODE_SET_PROTOTYPE_METHOD(addressTemplate, "getStreet", Address::GetStreet);
        NODE_SET_PROTOTYPE_METHOD(addressTemplate, "getCity", Address::GetCity);
        NODE_SET_PROTOTYPE_METHOD(addressTemplate, "getProvince", Address::GetProvince);
        NODE_SET_PROTOTYPE_METHOD(addressTemplate, "getCountry", Address::GetCountry);
        NODE_SET_PROTOTYPE_METHOD(addressTemplate, "getZipCode", Address::GetZipCode);
        NODE_SET_PROTOTYPE_METHOD(addressTemplate, "toString", Address::ToString);

        NJSLibCore::Address::constructor.Reset(isolate, addressTemplate->GetFunction());
        exports->Set(v8::String::NewFromUtf8(isolate, "Address"), addressTemplate->GetFunction());
        END_EX_GUARD(isolate);

        return;
    }

    void Address::NewAddress(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);

        // verify arguments
        if (args.Length() < 6) {
            throw (std::invalid_argument("Missing number of arguments."));
        } 

        if (!(args[0]->IsNumber() && args[1]->IsString() && args[2]->IsString() &&
            args[3]->IsString() && args[4]->IsString() && args[5]->IsString())) {
            throw (std::invalid_argument("Invalid argument(s)."));
        }
        
        if (args.IsConstructCall()) {
            auto streetNum = args[0]->ToInt32()->Value();
            auto street = Utils::V8StringToCXXString(args[1]->ToString());
            auto city = Utils::V8StringToCXXString(args[2]->ToString());
            auto province = Utils::V8StringToCXXString(args[3]->ToString());
            auto country = Utils::V8StringToCXXString(args[4]->ToString());
            auto zipCode = Utils::V8StringToCXXString(args[5]->ToString());
            std::unique_ptr<NJSLibCore::Address> address(
                new NJSLibCore::Address(
                    streetNum, street,
                    city, province,
                    country, zipCode
                )
            );

            address->Wrap(args.This());
            address.release();

            args.GetReturnValue().Set(args.This());
        }
        else {
            const int argc = 6;
            v8::Local<v8::Value> argv[argc] = {
                args[0], args[1],
                args[2], args[3],
                args[4], args[5]
            };
            auto addressCtor = v8::Local<v8::Function>::New(isolate, NJSLibCore::Address::constructor);
            args.GetReturnValue().Set(addressCtor->NewInstance(argc, argv));
        }
        END_EX_GUARD(isolate);

        return;
    }

    void Address::GetStreetNum(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);
        
        BEGIN_EX_GUARD(isolate);
        auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(args.This());
        auto streetNum = address->getStreetNum();
        args.GetReturnValue().Set(v8::Number::New(isolate, static_cast<double>(streetNum)));
        END_EX_GUARD(isolate);

        return;
    }

    void Address::GetStreet(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);
        
        BEGIN_EX_GUARD(isolate);
        auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(args.This());
        auto streetStr = address->getStreet();
        auto streetV8Str = v8::String::NewFromUtf8(isolate, streetStr.c_str());
        args.GetReturnValue().Set(streetV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void Address::GetCity(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);
        
        BEGIN_EX_GUARD(isolate);
        auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(args.This());
        auto cityStr = address->getCity();
        auto cityV8Str = v8::String::NewFromUtf8(isolate, cityStr.c_str());
        args.GetReturnValue().Set(cityV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void Address::GetProvince(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);
        
        BEGIN_EX_GUARD(isolate);
        auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(args.This());
        auto provinceStr = address->getProvince();
        auto provinceV8Str = v8::String::NewFromUtf8(isolate, provinceStr.c_str());
        args.GetReturnValue().Set(provinceV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void Address::GetCountry(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);
        
        BEGIN_EX_GUARD(isolate);
        auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(args.This());
        auto countryStr = address->getCountry();
        auto countryV8Str = v8::String::NewFromUtf8(isolate, countryStr.c_str());
        args.GetReturnValue().Set(countryV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void Address::GetZipCode(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(args.This());
        auto zipCodeStr = address->getZipCode();
        auto zipCodeV8Str = v8::String::NewFromUtf8(isolate, zipCodeStr.c_str());
        args.GetReturnValue().Set(zipCodeV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void Address::ToString(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(args.This());
        auto addressStr = address->toString();
        auto addressV8Str = v8::String::NewFromUtf8(isolate, addressStr.c_str());
        args.GetReturnValue().Set(addressV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    v8::Persistent<v8::Function> Address::constructor;

    class Person final : public node::ObjectWrap
    {
    public:
        Person(
            const std::string& lastName,
            const std::string& firstName,
            int age,
            const CXXLib::Address& address
        )
        {
            _impl.reset(new CXXLib::Person(lastName, firstName, age, address));
            return;
        }

        ~Person(void)
        {
            // std::cout << __FILE__ << ":" << __LINE__ << " - " << __FUNCTION__ << std::endl;
            return;
        }

        std::string getLastName(void) const
        {
            return (_impl->getLastName());
        }

        std::string getFirstName(void) const
        {
            return (_impl->getFirstName());
        }

        int getAge(void) const
        {
            return (_impl->getAge());
        }

        CXXLib::Address getAddress(void) const
        {
            return (_impl->getAddress());
        }

        std::string toString(void) const
        {
            return (_impl->toString());
        }

    private:
        Person();

        std::unique_ptr<CXXLib::Person> _impl;

    public:
        // Node.js definitions for class wrap
        static void InitializeClass(v8::Local<v8::Object> exports);
        static void NewPerson(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetLastName(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetFirstName(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetAge(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetAddress(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void ToString(const v8::FunctionCallbackInfo<v8::Value>& args);

        static v8::Persistent<v8::Function> constructor;
    }; // class Person

    void Person::InitializeClass(v8::Local<v8::Object> exports)
    {
        auto* isolate = exports->GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        v8::Local<v8::FunctionTemplate> personTemplate = v8::FunctionTemplate::New(isolate, NewPerson);
        personTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Person"));
        personTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(personTemplate, "getLastName", Person::GetLastName);
        NODE_SET_PROTOTYPE_METHOD(personTemplate, "getFirstName", Person::GetFirstName);
        NODE_SET_PROTOTYPE_METHOD(personTemplate, "getAge", Person::GetAge);
        NODE_SET_PROTOTYPE_METHOD(personTemplate, "getAddress", Person::GetAddress);
        NODE_SET_PROTOTYPE_METHOD(personTemplate, "toString", Person::ToString);

        NJSLibCore::Person::constructor.Reset(isolate, personTemplate->GetFunction());
        exports->Set(v8::String::NewFromUtf8(isolate, "Person"), personTemplate->GetFunction());
        END_EX_GUARD(isolate);

        return;
    }

    void Person::NewPerson(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);

        // verify arguments
        if (args.Length() < 4) {
            throw (std::invalid_argument("Missing number of arguments."));
        } 

        if (!(args[0]->IsString() && args[1]->IsString() && args[2]->IsNumber() &&
            args[3]->IsObject())) {
            throw (std::invalid_argument("Invalid argument(s)."));
        }
        
        if (args.IsConstructCall()) {
            auto lastName = Utils::V8StringToCXXString(args[0]->ToString());
            auto firstName = Utils::V8StringToCXXString(args[1]->ToString());
            auto age = args[2]->ToInt32()->Value();
            auto addressObject = args[3]->ToObject();

            auto addressCtorName = addressObject->GetConstructorName();
            std::cout << Utils::V8StringToCXXString(addressCtorName) << std::endl;

            auto* address = node::ObjectWrap::Unwrap<NJSLibCore::Address>(addressObject);
            
            if (address == nullptr) {
                throw (std::invalid_argument("Invalid address."));
            }

            std::unique_ptr<NJSLibCore::Person> person(
                new NJSLibCore::Person(
                    lastName, firstName,
                    age,
                    *(address->_impl)
                )
            );

            person->Wrap(args.This());
            person.release();

            args.GetReturnValue().Set(args.This());
        }
        else {
            const int argc = 4;
            v8::Local<v8::Value> argv[argc] = {
                args[0], args[1],
                args[2], args[3]
            };
            auto personCtor = v8::Local<v8::Function>::New(isolate, NJSLibCore::Person::constructor);
            args.GetReturnValue().Set(personCtor->NewInstance(argc, argv));
        }
        END_EX_GUARD(isolate);

        return;
    }

    void Person::GetLastName(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto* person = node::ObjectWrap::Unwrap<NJSLibCore::Person>(args.This());
        auto lastNameStr = person->getLastName();
        auto lastNameV8Str = v8::String::NewFromUtf8(isolate, lastNameStr.c_str());
        args.GetReturnValue().Set(lastNameV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void Person::GetFirstName(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto* person = node::ObjectWrap::Unwrap<NJSLibCore::Person>(args.This());
        auto firstNameStr = person->getFirstName();
        auto firstNameV8Str = v8::String::NewFromUtf8(isolate, firstNameStr.c_str());
        args.GetReturnValue().Set(firstNameV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    void Person::GetAge(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto* person = node::ObjectWrap::Unwrap<NJSLibCore::Person>(args.This());
        auto age = person->getAge();
        args.GetReturnValue().Set(v8::Number::New(isolate, static_cast<double>(age)));
        END_EX_GUARD(isolate);

        return;
    }

    void Person::GetAddress(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto* person = node::ObjectWrap::Unwrap<NJSLibCore::Person>(args.This());
        auto address = person->getAddress(); // gets CXXLib::Address
        // move to heap
        // std::unique_ptr<NJSLibCore::Address> addressResult(new NJSLibCore::Address(std::move(address)));
        
        END_EX_GUARD(isolate);

        return;
    }

    void Person::ToString(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        auto* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        BEGIN_EX_GUARD(isolate);
        auto* person = node::ObjectWrap::Unwrap<NJSLibCore::Person>(args.This());
        auto personStr = person->toString();
        auto personV8Str = v8::String::NewFromUtf8(isolate, personStr.c_str());
        args.GetReturnValue().Set(personV8Str);
        END_EX_GUARD(isolate);

        return;
    }

    v8::Persistent<v8::Function> Person::constructor;
}

void NJLInitialize(v8::Local<v8::Object> exports)
{
    NODE_SET_METHOD(exports, "LibraryInitialize", NJSLibCore::LibraryInitialize);
    NODE_SET_METHOD(exports, "LibraryTerminate", NJSLibCore::LibraryTerminate);
    NODE_SET_METHOD(exports, "LibraryGetVersionString", NJSLibCore::LibraryGetVersionString);
    NODE_SET_METHOD(exports, "LibraryGetVersionMajor", NJSLibCore::LibraryGetVersionMajor);
    NODE_SET_METHOD(exports, "LibraryGetVersionMinor", NJSLibCore::LibraryGetVersionMinor);

    // Initialize classes.
    NJSLibCore::Address::InitializeClass(exports);
    NJSLibCore::Person::InitializeClass(exports);

    return;
}
