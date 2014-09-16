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

        friend class Address;
        friend class Person;
    };

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
} // namespace CXXLib

#endif // !defined(CXXLIB_API_HXX)
