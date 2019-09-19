#pragma once

#include <dotslashzero/clib/clib.h>

#include <memory>
#include <stdexcept>
#include <string>

#define DSZ_CXXLIB_API __attribute__ ((visibility("default")))
#if defined(_MSC_VER)
#undef DSZ_CXXLIB_API
#define DSZ_CXXLIB_API __declspec(dllexport)
#endif // defined(_MSC_VER)

// In production code, it will always be ideal to inline the definitions of C++ class functions especially if they use
// C++ standard library template classes. ABI is always a challenge with C++ so it is recommend to use a simple ABI in
// the C interface, then let C++ APIs just consume the C APIs. This way the consumer code will be the one compiling C++
// classes using their compilers (guarantees ABI compatibility). This method will also prevent implementation details
// from being revealed because the C++ APIs will simply act as a forwarder calls to the C APIs.

namespace DotSlashZero::CxxLib
{
    class Exception final :
        public std::runtime_error
    {
    public:
        DSZ_CXXLIB_API Exception(void);

        DSZ_CXXLIB_API std::string GetMessage(void) const noexcept;

    private:
        Exception(DszCLibErrorNum errorNum);

        friend class Address;
        friend class Person;
        friend class Printer;
    };
    // class Exception

    namespace Library
    {
        DSZ_CXXLIB_API bool Initialize(void) noexcept;
        DSZ_CXXLIB_API void Uninitialize(void) noexcept;

        DSZ_CXXLIB_API std::string GetVersionString(void);
        DSZ_CXXLIB_API std::size_t GetVersionMajor(void);
        DSZ_CXXLIB_API std::size_t GetVersionMinor(void);
        DSZ_CXXLIB_API std::size_t GetVersionPatch(void);
        DSZ_CXXLIB_API std::string GetVersionExtra(void);
    }
    // namespace Library

    class Address final
    {
    public:
        DSZ_CXXLIB_API Address(
            int streetNum,
            std::string const& street,
            std::string const& city,
            std::string const& province,
            std::string const& zipCode,
            std::string const& country);

        DSZ_CXXLIB_API Address(Address const& address);

        DSZ_CXXLIB_API ~Address(void) noexcept;

        DSZ_CXXLIB_API Address& operator=(Address const& address);

        DSZ_CXXLIB_API int GetStreetNum(void) const;
        DSZ_CXXLIB_API std::string GetStreet(void) const;
        DSZ_CXXLIB_API std::string GetCity(void) const;
        DSZ_CXXLIB_API std::string GetProvince(void) const;
        DSZ_CXXLIB_API std::string GetZipCode(void) const;
        DSZ_CXXLIB_API std::string GetCountry(void) const;

        DSZ_CXXLIB_API std::string ToString(void) const;

    private:
        Address(void);
        void Destroy__() noexcept;

        DszCLibAddress m_impl;

        friend class Person;
    };
    // class Address

    class Person final
    {
    public:
        // Note: The address is copied and need not to be persisted after Person is created.
        DSZ_CXXLIB_API Person(
            std::string const& lastName,
            std::string const& firstName,
            int age,
            Address const& address);

        DSZ_CXXLIB_API Person(Person const& person);

        DSZ_CXXLIB_API ~Person(void) noexcept;

        DSZ_CXXLIB_API Person& operator=(Person const& person);

        DSZ_CXXLIB_API std::string GetLastName(void) const;
        DSZ_CXXLIB_API std::string GetFirstName(void) const;
        DSZ_CXXLIB_API int GetAge(void) const;
        DSZ_CXXLIB_API Address GetAddress(void) const;

        DSZ_CXXLIB_API std::string ToString(void) const;

    private:
        void Destroy__() noexcept;

        DszCLibPerson m_impl;
    };
    // class Person

    class IGenerator
    {
    public:
        DSZ_CXXLIB_API IGenerator(void) = default;
        DSZ_CXXLIB_API virtual ~IGenerator(void) noexcept = default;

        DSZ_CXXLIB_API virtual int GenerateInt(int data) const = 0;
        DSZ_CXXLIB_API virtual std::string GenerateString(int data) const = 0;
    };
    // class IGenerator

    class Printer final
    {
    public:
        // Printer will take ownership of IGenerator instance.
        DSZ_CXXLIB_API Printer(IGenerator* pGenerator);

        Printer(Printer const&) = delete;

        DSZ_CXXLIB_API virtual ~Printer(void) noexcept;

        Printer& operator=(Printer const&) = delete;

        DSZ_CXXLIB_API void PrintInt(void) const;
        DSZ_CXXLIB_API void PrintString(void) const;

    private:
        void Destroy__() noexcept;

        DszCLibPrinter m_impl;
        std::unique_ptr<IGenerator> m_pGenerator;
    };
    // class Printer
}
// namespace DotSlashZero::CxxLib
