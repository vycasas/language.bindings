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

    int GetIntUsingGenerator(const Generator& generator)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);
        return (generator.generateInt(dist(rd)));
    }

    std::string GetStringUsingGenerator(const Generator& generator)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, 999);
        return (generator.generateString(dist(rd)));
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

    namespace CXXImplDetail
    {
        class CLibGeneratorImpl : public CXXLibCore::Generator
        {
        public:
            CLibGeneratorImpl(
                CLCoreGenerateIntFunction generateIntImpl,
                CLCoreGenerateStringFunction generateStringImpl,
                void* userData
            );
            virtual ~CLibGeneratorImpl(void);

            virtual int generateInt(int data) const override;
            virtual std::string generateString(int data) const override;
        private:
            CLCoreGenerateIntFunction _generateIntImpl;
            CLCoreGenerateStringFunction _generateStringImpl;
            void* _userData;
        }; // class CLibGeneratorImpl

        CLibGeneratorImpl::CLibGeneratorImpl(
            CLCoreGenerateIntFunction generateIntImpl,
            CLCoreGenerateStringFunction generateStringImpl,
            void* userData
        ) : _generateIntImpl(generateIntImpl), _generateStringImpl(generateStringImpl), _userData(userData)
        { return; }

        CLibGeneratorImpl::~CLibGeneratorImpl(void)
        { return; }

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
        void* userData,
        CLCoreGenerator* generator
    )
    {
        if (generator == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXImplDetail::CLibGeneratorImpl> result(
                new CXXImplDetail::CLibGeneratorImpl(intFunction, stringFunction, userData)
            );
            *generator = reinterpret_cast<CLCoreGenerator>(result.release());
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
                reinterpret_cast<CXXImplDetail::CLibGeneratorImpl*>(generator)
            );
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLCorePrintIntUsingGenerator(CLCoreGenerator generator)
    {
        if (generator == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXImplDetail::CLibGeneratorImpl> ownedPtr(
                reinterpret_cast<CXXImplDetail::CLibGeneratorImpl*>(generator)
            );

            auto result = CXXLibCore::GetIntUsingGenerator(*ownedPtr);
            std::cout << "The value of int is: " << result << std::endl;
        }
        catch (...) {
            return (1);
        }

        return (0);
    }

    CLibErrNum CLCorePrintStringUsingGenerator(CLCoreGenerator generator)
    {
        if (generator == nullptr)
            return (2);

        try {
            std::unique_ptr<CXXImplDetail::CLibGeneratorImpl> ownedPtr(
                reinterpret_cast<CXXImplDetail::CLibGeneratorImpl*>(generator)
            );

            auto result = CXXLibCore::GetStringUsingGenerator(*ownedPtr);
            std::cout << "The value of string is: " << result << std::endl;
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

    Generator::Generator(void)
    { return; }

    Generator::~Generator(void)
    { return; }

    CLibErrNum Generator::IntFunction(int data, int* result, void* userData)
    {
        if (result == nullptr || userData == nullptr)
            return (2);

        Generator* generatorPtr = reinterpret_cast<Generator*>(userData);
        *result = generatorPtr->generateInt(data);
        return (0);
    }

    CLibErrNum Generator::StringFunction(
        int data,
        char* result, size_t resultSize, size_t* charWritten,
        void* userData
    )
    {
        // Optionally, we can allow result to be nullptr...
        if (result == nullptr || resultSize == 0 || userData == nullptr)
            return (2);

        Generator* generatorPtr = reinterpret_cast<Generator*>(userData);
        auto implResult = generatorPtr->generateString(data);
        std::strncpy(result, implResult.data(), resultSize);

        if (charWritten != nullptr)
            *charWritten = std::strlen(result);

        return (0);
    }

    void GeneratorFunctions::PrintIntUsingGenerator(std::unique_ptr<Generator> generator)
    {
        // This is easily achievable if we are not going through a C ABI, but our sample requires doing so.
        auto c_api_call_result = CLib::CLCorePrintIntUsingGenerator(generator.release());
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }

    void GeneratorFunctions::PrintStringUsingGenerator(std::unique_ptr<Generator> generator)
    {
        auto c_api_call_result = CLib::CLCorePrintStringUsingGenerator(generator.release());
        CXXLIB_API_CHECK(c_api_call_result);
        return;
    }
} // namespace CXXLib
