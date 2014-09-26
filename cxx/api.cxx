#include "api.hxx"

#include <array>
#include <cstring>
#include <iostream>
#include <memory>
#include <random>

#define CXXLIB_API_CHECK(result) \
    if (result != 0) \
        throw (CXXLib::Exception(result));

namespace CXXLibCore
{
    // These are classes/functions that are not part of API.
    class Generator
    {
    public:
        virtual ~Generator(void)
        { return; }

        virtual int generateInt(int data) const = 0;
        virtual std::string generateString(int data) const = 0;
    }; // class Generator

    class Printer
    {
    public:
        Printer(std::unique_ptr<Generator> generator) : 
            _generator(std::move(generator))
        { return; }

        void printInt(void);
        void printString(void);

    private:
        std::unique_ptr<Generator> _generator;
    }; // class Printer

    void Printer::printInt(void)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);
        std::cout << "The value of int is: " << _generator->generateInt(dist(rd)) << std::endl;
        return;
    }

    void Printer::printString(void)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);
        std::cout << "The value of string is: " << _generator->generateString(dist(rd)) << std::endl;
        return;
    }

} // namespace CXXLibCore

// C interface for the Generator sample. Not necessary, but added for demonstration purposes.
namespace CLib
{
    typedef void* CLCoreGenerator;
    typedef CLibErrNum (*CLCoreGenerateIntFunction)(int data, int* result, void* userData);
    typedef CLibErrNum (*CLCoreGenerateStringFunction)(
        int data,
        char* result, size_t resultSize, size_t* charWritten,
        void* userData
    );
    typedef CLibErrNum (*CLCoreGeneratorDestroyFunction)(void* userData);
    typedef void* CLCorePrinter;

    namespace CXXImplDetail
    {
        class CLibGeneratorImpl : public CXXLibCore::Generator
        {
        public:
            CLibGeneratorImpl(
                CLCoreGenerateIntFunction generateIntImpl,
                CLCoreGenerateStringFunction generateStringImpl,
                CLCoreGeneratorDestroyFunction destroyImpl,
                void* userData
            );
            virtual ~CLibGeneratorImpl(void);

            virtual int generateInt(int data) const override;
            virtual std::string generateString(int data) const override;
        private:
            CLCoreGenerateIntFunction _generateIntImpl;
            CLCoreGenerateStringFunction _generateStringImpl;
            CLCoreGeneratorDestroyFunction _destroyImpl;
            void* _userData;
        }; // class CLibGeneratorImpl

        CLibGeneratorImpl::CLibGeneratorImpl(
            CLCoreGenerateIntFunction generateIntImpl,
            CLCoreGenerateStringFunction generateStringImpl,
            CLCoreGeneratorDestroyFunction destroyImpl,
            void* userData
        ) :
            _generateIntImpl(generateIntImpl),
            _generateStringImpl(generateStringImpl),
            _destroyImpl(destroyImpl),
            _userData(userData)
        { return; }

        CLibGeneratorImpl::~CLibGeneratorImpl(void)
        {
            this->_destroyImpl(_userData);
            return;
        }

        int CLibGeneratorImpl::generateInt(int data) const
        {
            int result;
            this->_generateIntImpl(data, &result, _userData); // note: evaluate result and handle appropriately.
            return (result);
        }

        std::string CLibGeneratorImpl::generateString(int data) const
        {
            char buffer[1024];
            this->_generateStringImpl(data, buffer, 1024, nullptr, _userData);
            std::string result(buffer);
            return (result);
        }
    } // namespace CXXImplDetail

    CLibErrNum CLCoreGeneratorCreate(
        CLCoreGenerateIntFunction intFunction,
        CLCoreGenerateStringFunction stringFunction,
        CLCoreGeneratorDestroyFunction destroyFunction,
        void* userData,
        CLCoreGenerator* generator
    )
    {
        if (generator == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXImplDetail::CLibGeneratorImpl> result(
                new CXXImplDetail::CLibGeneratorImpl(intFunction, stringFunction, destroyFunction, userData)
            );
            *generator = static_cast<CLCoreGenerator>(result.release());
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLCoreGeneratorDestroy(CLCoreGenerator generator)
    {
        if (generator == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXImplDetail::CLibGeneratorImpl> ownedPtr(
                static_cast<CXXImplDetail::CLibGeneratorImpl*>(generator)
            );
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLCorePrinterCreate(CLCoreGenerator generator, CLCorePrinter* printer)
    {
        if (generator == nullptr || printer == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXLibCore::Generator> coreGenerator(
                std::move(static_cast<CXXImplDetail::CLibGeneratorImpl*>(generator))
            );

            std::unique_ptr<CXXLibCore::Printer> corePrinter(
                new CXXLibCore::Printer(std::move(coreGenerator))
            );

            *printer = static_cast<CLCorePrinter>(corePrinter.release());
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLCorePrinterDestroy(CLCorePrinter printer)
    {
        if (printer == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXLibCore::Printer> ownedPtr(
                static_cast<CXXLibCore::Printer*>(printer)
            );
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLCorePrinterPrintInt(CLCorePrinter printer)
    {
        if (printer == nullptr)
            return (2);

        try {
            CXXLibCore::Printer* corePrinter = static_cast<CXXLibCore::Printer*>(printer);
            corePrinter->printInt();
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLCorePrinterPrintString(CLCorePrinter printer)
    {
        if (printer == nullptr)
            return (2);

        try {
            CXXLibCore::Printer* corePrinter = static_cast<CXXLibCore::Printer*>(printer);
            corePrinter->printString();
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

} // namespace CLib

namespace CXXLib
{
    Exception::Exception(CLibErrNum errNum) : _impl{errNum}
    {
        std::array<char, 40> buffer;
        buffer.fill('\0');
        CLErrNumGetMessage(_impl, buffer.data(), buffer.size());

        _message = std::string(buffer.data());
        return;
    }

    std::string Exception::getMessage(void) const
    {
        return (_message);
    }

    const char* Exception::what(void) const noexcept
    {
        return (getMessage().data());
    }

    void Library::initialize(void)
    {
        auto c_api_call_result = CLLibraryInitialize();
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    void Library::terminate(void)
    {
        CLLibraryTerminate();
        return;
    }

    Address::Address(
        int streetNum, const std::string& street,
        const std::string& city, const std::string& province,
        const std::string& country, const std::string& zipCode
    ) : _impl{nullptr}
    {
        auto c_api_call_result = CLAddressCreate(

            streetNum, street.data(),
            city.data(), province.data(),
            country.data(), zipCode.data(),
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    Address::~Address(void) noexcept
    {
        CLAddressDestroy(_impl);
        return;
    }

    int Address::getStreetNum(void) const
    {
        int result = -1;
        auto c_api_call_result = CLAddressGetStreetNumber(_impl, &result);
        CXXLIB_API_CHECK(c_api_call_result);

        return (result);
    }

    std::string Address::getStreet(void) const
    {
        std::array<char, 40> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLAddressGetStreet(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getCity(void) const
    {
        std::array<char, 16> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLAddressGetCity(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getProvince(void) const
    {
        std::array<char, 8> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLAddressGetProvince(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getCountry(void) const
    {
        std::array<char, 16> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLAddressGetCountry(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getZipCode(void) const
    {
        std::array<char, 8> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLAddressGetZipCode(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::toString(void) const
    {
        std::array<char, 256> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLAddressToString(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    Person::Person(
            const std::string& lastName,
            const std::string& firstName,
            int age,
            const Address& address
    ) : _impl{nullptr}
    {
        auto c_api_call_result = CLPersonCreate(
            lastName.data(),
            firstName.data(),
            age,
            address._impl,
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);

        return;
    }

    Person::~Person(void) noexcept
    {
        CLPersonDestroy(_impl);
        return;
    }

    std::string Person::getLastName(void) const
    {
        std::array<char, 24> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLPersonGetLastName(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Person::getFirstName(void) const
    {
        std::array<char, 24> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLPersonGetFirstName(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    int Person::getAge(void) const
    {
        int result = -1;
        auto c_api_call_result = CLPersonGetAge(_impl, &result);
        CXXLIB_API_CHECK(c_api_call_result);

        return (result);
    }

    Address Person::getAddress(void) const
    {
        Address address;
        auto c_api_call_result = CLPersonGetAddress(_impl, &(address._impl));
        CXXLIB_API_CHECK(c_api_call_result);

        return (address);
    }

    std::string Person::toString(void) const
    {
        std::array<char, 512> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLPersonToString(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    GeneratorBase::GeneratorBase(void)
    { return; }

    GeneratorBase::~GeneratorBase(void)
    { return; }

    CLibErrNum GeneratorBase::IntFunction(int data, int* result, void* userData)
    {
        if (result == nullptr || userData == nullptr)
            return (2);

        auto generatorPtr = reinterpret_cast<GeneratorBase*>(userData);
        *result = generatorPtr->generateInt(data);
        return (0);
    }

    CLibErrNum GeneratorBase::StringFunction(
        int data,
        char* result, size_t resultSize, size_t* charWritten,
        void* userData
    )
    {
        // Optionally, we can allow result to be nullptr...
        if (result == nullptr || resultSize == 0 || userData == nullptr)
            return (2);

        auto generatorPtr = reinterpret_cast<GeneratorBase*>(userData);
        auto implResult = generatorPtr->generateString(data);
        std::strncpy(result, implResult.data(), resultSize);

        if (charWritten != nullptr)
            *charWritten = std::strlen(result);

        return (0);
    }

    CLibErrNum GeneratorBase::DestroyFunction(void* userData)
    {
        if (userData == nullptr)
            return (2);

        try {
            std::unique_ptr<GeneratorBase> ownedPtr(
                static_cast<GeneratorBase*>(userData)
            );
        }
        catch (...) {
            return (1);
        }
        return (0);
    }

    Printer::Printer(std::unique_ptr<GeneratorBase> generator)
    {
        CLib::CLCoreGenerator coreGenerator = nullptr;
        auto* generatorPtr = generator.release();
        auto c_api_call_result = CLib::CLCoreGeneratorCreate(
            &GeneratorBase::IntFunction,
            &GeneratorBase::StringFunction,
            &GeneratorBase::DestroyFunction,
            generatorPtr,
            &coreGenerator
        );
        if (c_api_call_result != 0) {
            generator.reset(generatorPtr);
        }
        CXXLIB_API_CHECK(c_api_call_result);
        c_api_call_result = CLib::CLCorePrinterCreate(coreGenerator, &_impl);
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    Printer::~Printer(void)
    {
        CLib::CLCorePrinterDestroy(_impl);
        return;
    }

    void Printer::printInt(void)
    {
        auto c_api_call_result = CLib::CLCorePrinterPrintInt(_impl);
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    void Printer::printString(void)
    {
        auto c_api_call_result = CLib::CLCorePrinterPrintString(_impl);
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }
} // namespace CXXLib
