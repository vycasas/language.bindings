#include <cxxlib/cxxlib.hxx>

#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <memory>

#if defined(_MSC_VER)
    #define _CRT_SECURE_NO_WARNINGS 1
    #pragma warning(disable:4996)
#endif // defined(_MSC_VER)

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
		auto someInt = static_cast<int>(std::chrono::seconds(std::time(nullptr)).count());
        std::cout << "The value of int is: " << _generator->generateInt(someInt) << std::endl;
        return;
    }

    void Printer::printString(void)
    {
		auto someInt = static_cast<int>(std::chrono::seconds(std::time(nullptr)).count());
        std::cout << "The value of string is: " << _generator->generateString(someInt) << std::endl;
        return;
    }

} // namespace CXXLibCore

// C interface for the Generator sample. Not necessary, but added for demonstration purposes.
namespace CLib
{
    typedef void* CLibCoreGenerator;
    typedef CLibErrNum (*CLibCoreGenerateIntFunction)(int data, int* result, void* userData);
    typedef CLibErrNum (*CLibCoreGenerateStringFunction)(
        int data,
        char* result, size_t resultSize, size_t* charWritten,
        void* userData
    );
    typedef CLibErrNum (*CLibCoreGeneratorDestroyFunction)(void* userData);
    typedef void* CLibCorePrinter;

    namespace CXXImplDetail
    {
        // For use with throwing exceptions within this namespace.
        class CLibException final : public CXXLib::Exception
        {
        public:
            CLibException(void) : CXXLib::Exception()
            { return; }

            CLibException(CLibErrNum errNum) : CXXLib::Exception(errNum)
            { return; }
        }; // class CLibException

        class CLibGeneratorImpl final : public CXXLibCore::Generator
        {
        public:
            CLibGeneratorImpl(
                CLibCoreGenerateIntFunction generateIntImpl,
                CLibCoreGenerateStringFunction generateStringImpl,
                CLibCoreGeneratorDestroyFunction destroyImpl,
                void* userData
            );
            virtual ~CLibGeneratorImpl(void);

            virtual int generateInt(int data) const override;
            virtual std::string generateString(int data) const override;
        private:
            CLibCoreGenerateIntFunction _generateIntImpl;
            CLibCoreGenerateStringFunction _generateStringImpl;
            CLibCoreGeneratorDestroyFunction _destroyImpl;
            void* _userData;
        }; // class CLibGeneratorImpl

        CLibGeneratorImpl::CLibGeneratorImpl(
            CLibCoreGenerateIntFunction generateIntImpl,
            CLibCoreGenerateStringFunction generateStringImpl,
            CLibCoreGeneratorDestroyFunction destroyImpl,
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
            try {
                int result;
                this->_generateIntImpl(data, &result, _userData); // note: evaluate result and handle appropriately.
                return (result);
            }
            catch (...) {
                throw (CLibException(2));
            }
        }

        std::string CLibGeneratorImpl::generateString(int data) const
        {
            try {
                char buffer[1024];
                this->_generateStringImpl(data, buffer, 1024, nullptr, _userData);
                std::string result(buffer);
                return (result);
            }
            catch (...) {
                throw (CLibException(2));
            }
        }
    } // namespace CXXImplDetail

    CLibErrNum CLibCoreGeneratorCreate(
        CLibCoreGenerateIntFunction intFunction,
        CLibCoreGenerateStringFunction stringFunction,
        CLibCoreGeneratorDestroyFunction destroyFunction,
        void* userData,
        CLibCoreGenerator* generator
    )
    {
        if (generator == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXImplDetail::CLibGeneratorImpl> result(
                new CXXImplDetail::CLibGeneratorImpl(intFunction, stringFunction, destroyFunction, userData)
            );
            *generator = static_cast<CLibCoreGenerator>(result.release());
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLibCoreGeneratorDestroy(CLibCoreGenerator generator)
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

    CLibErrNum CLibCorePrinterCreate(CLibCoreGenerator generator, CLibCorePrinter* printer)
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

            *printer = static_cast<CLibCorePrinter>(corePrinter.release());
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLibCorePrinterDestroy(CLibCorePrinter printer)
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

    CLibErrNum CLibCorePrinterPrintInt(CLibCorePrinter printer)
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

    CLibErrNum CLibCorePrinterPrintString(CLibCorePrinter printer)
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
    void Library::initialize(void)
    {
        auto c_api_call_result = CLibLibraryInitialize();
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    void Library::terminate(void)
    {
        CLibLibraryTerminate();
        return;
    }

    std::string Library::getVersionString(void)
    {
        return (std::string(CLibLibraryGetVersionString()));
    }

    size_t Library::getVersionMajor(void)
    {
        return (CLibLibraryGetVersionMajor());
    }

    size_t Library::getVersionMinor(void)
    {
        return (CLibLibraryGetVersionMinor());
    }

    Address::Address(
        int streetNum, const std::string& street,
        const std::string& city, const std::string& province,
        const std::string& country, const std::string& zipCode
    ) : _impl{nullptr}
    {
        auto c_api_call_result = CLibAddressCreate(
            streetNum, street.data(),
            city.data(), province.data(),
            country.data(), zipCode.data(),
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    Address::Address(const Address& address)
    {
        auto c_api_call_result = CLibAddressCreate(
            address.getStreetNum(), address.getStreet().data(),
            address.getCity().data(), address.getProvince().data(),
            address.getCountry().data(), address.getZipCode().data(),
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    Address::~Address(void) noexcept
    {
        CLibAddressDestroy(_impl);
        return;
    }

    Address& Address::operator=(const Address& address)
    {
        if (_impl != nullptr) {
            CLibAddressDestroy(_impl);
            _impl = nullptr;
        }

        auto c_api_call_result = CLibAddressCreate(
            address.getStreetNum(), address.getStreet().data(),
            address.getCity().data(), address.getProvince().data(),
            address.getCountry().data(), address.getZipCode().data(),
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);
        return (*this);
    }

    int Address::getStreetNum(void) const
    {
        int result = -1;
        auto c_api_call_result = CLibAddressGetStreetNumber(_impl, &result);
        CXXLIB_API_CHECK(c_api_call_result);

        return (result);
    }

    std::string Address::getStreet(void) const
    {
        std::array<char, 40> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibAddressGetStreet(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getCity(void) const
    {
        std::array<char, 16> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibAddressGetCity(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getProvince(void) const
    {
        std::array<char, 8> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibAddressGetProvince(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getCountry(void) const
    {
        std::array<char, 16> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibAddressGetCountry(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::getZipCode(void) const
    {
        std::array<char, 8> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibAddressGetZipCode(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Address::toString(void) const
    {
        std::array<char, 256> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibAddressToString(_impl, buffer.data(), buffer.size(), nullptr);
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
        // Remember: address._impl is copied.
        auto c_api_call_result = CLibPersonCreate(
            lastName.data(),
            firstName.data(),
            age,
            address._impl,
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    Person::Person(const Person& person)
    {
        auto c_api_call_result = CLibPersonCreate(
            person.getLastName().data(),
            person.getFirstName().data(),
            person.getAge(),
            person.getAddress()._impl,
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    Person::~Person(void) noexcept
    {
        CLibPersonDestroy(_impl);
        return;
    }

    Person& Person::operator=(const Person& person)
    {
        if (_impl != nullptr) {
            CLibPersonDestroy(_impl);
            _impl = nullptr;
        }
        auto c_api_call_result = CLibPersonCreate(
            person.getLastName().data(),
            person.getFirstName().data(),
            person.getAge(),
            person.getAddress()._impl,
            &_impl
        );
        CXXLIB_API_CHECK(c_api_call_result);
        return (*this);
    }

    std::string Person::getLastName(void) const
    {
        std::array<char, 24> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibPersonGetLastName(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    std::string Person::getFirstName(void) const
    {
        std::array<char, 24> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibPersonGetFirstName(_impl, buffer.data(), buffer.size(), nullptr);
        CXXLIB_API_CHECK(c_api_call_result);

        return (std::string(buffer.data()));
    }

    int Person::getAge(void) const
    {
        int result = -1;
        auto c_api_call_result = CLibPersonGetAge(_impl, &result);
        CXXLIB_API_CHECK(c_api_call_result);

        return (result);
    }

    Address Person::getAddress(void) const
    {
        // we need to make sure that we don't take ownership of this Person's Address impl...
        Address address;
        auto c_api_call_result = CLibPersonGetAddress(_impl, &(address._impl));
        CXXLIB_API_CHECK(c_api_call_result);

        Address result(address); // make a copy
        address._impl = nullptr; // remove reference
        return (result);
    }

    std::string Person::toString(void) const
    {
        std::array<char, 512> buffer;
        buffer.fill('\0');

        auto c_api_call_result = CLibPersonToString(_impl, buffer.data(), buffer.size(), nullptr);
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
        CLib::CLibCoreGenerator coreGenerator = nullptr;
        auto* generatorPtr = generator.release();
        auto c_api_call_result = CLib::CLibCoreGeneratorCreate(
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
        c_api_call_result = CLib::CLibCorePrinterCreate(coreGenerator, &_impl);
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    Printer::~Printer(void)
    {
        CLib::CLibCorePrinterDestroy(_impl);
        return;
    }

    void Printer::printInt(void)
    {
        auto c_api_call_result = CLib::CLibCorePrinterPrintInt(_impl);
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    void Printer::printString(void)
    {
        auto c_api_call_result = CLib::CLibCorePrinterPrintString(_impl);
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    void Printer::createInstance(std::unique_ptr<GeneratorBase>&& generator)
    {
        std::unique_ptr<Printer> instance(new Printer(std::move(generator)));
        _impl = instance->_impl;
        instance.release();
        return;
    }
} // namespace CXXLib
