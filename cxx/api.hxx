#if !defined(CXXLIB_API_HXX)
#define CXXLIB_API_HXX

#include <exception>
#include <string>

#include <C/api.h>

#define CXXLIB_API __attribute__ ((visibility ("default")))

namespace CXXLib
{
    class CXXLIB_API Exception : std::exception
    {
    public:
        Exception(const Exception& src) :
            std::exception(src),
            _message(src._message),
            _impl(src._impl)
        { return; }

        Exception(const std::exception& src) :
            std::exception(src),
            _message(src.what()),
            _impl(2)
        { return; }

        std::string getMessage(void) const;

        virtual const char* what(void) const noexcept override;

    private:
        Exception(CLibErrNum errNum);

        std::string _message;
        CLibErrNum _impl;

        friend struct Library;
        friend class Address;
        friend class Person;
        friend struct GeneratorFunctions;
    }; // class Exception

    struct CXXLIB_API Library
    {
        static void initialize(void);
        static void terminate(void);

        Library(void) = delete;
        ~Library(void) = delete;
        Library(const Library&) = delete;
        Library(const Library&&) = delete;
        Library& operator=(const Library&) = delete;
        Library& operator=(const Library&&) = delete;
    }; // struct Library

    class CXXLIB_API Address
    {
    public:
        Address(
            int streetNum, const std::string& street,
            const std::string& city, const std::string& province,
            const std::string& country, const std::string& zipCode
        );

        ~Address(void) noexcept;

        int getStreetNum(void) const;
        std::string getStreet(void) const;
        std::string getCity(void) const;
        std::string getProvince(void) const;
        std::string getCountry(void) const;
        std::string getZipCode(void) const;

        std::string toString(void) const;

    private:
        Address(void) : _impl{nullptr} { return; }
        CLibAddress _impl;

        friend class Person;
    }; // class Address

    class CXXLIB_API Person
    {
    public:
        Person(
            const std::string& lastName,
            const std::string& firstName,
            int age,
            const Address& address
        );

        ~Person(void) noexcept;

        std::string getLastName(void) const;
        std::string getFirstName(void) const;
        int getAge(void) const;
        Address getAddress(void) const;

        std::string toString(void) const;

    private:
        CLibPerson _impl;
    }; // class Person

    class CXXLIB_API GeneratorBase
    {
    public:
        GeneratorBase(void);
        virtual ~GeneratorBase(void) = 0;

        virtual int generateInt(int data) const = 0;
        virtual std::string generateString(int data) const = 0;

        static CLibErrNum IntFunction(int data, int* result, void* userData);
        static CLibErrNum StringFunction(
            int data,
            char* result, size_t resultSize, size_t* charWritten,
            void* userData
        );
    }; // class GeneratorBase

    struct CXXLIB_API GeneratorFunctions
    {
        static void PrintIntUsingGenerator(std::unique_ptr<GeneratorBase> generator);
        static void PrintStringUsingGenerator(std::unique_ptr<GeneratorBase> generator);

        GeneratorFunctions(void) = delete;
        ~GeneratorFunctions(void) = delete;
        GeneratorFunctions(const GeneratorFunctions&) = delete;
        GeneratorFunctions(const GeneratorFunctions&&) = delete;
        GeneratorFunctions& operator=(const GeneratorFunctions&) = delete;
        GeneratorFunctions& operator=(const GeneratorFunctions&&) = delete;
    }; // struct GeneratorFunctions
} // namespace CXXLib

#endif // !defined(CXXLIB_API_HXX)
