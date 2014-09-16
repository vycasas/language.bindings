#include "api.hxx"

#include <array>

#define CXXLIB_API_CHECK(result) \
    if (result != 0) \
        throw (CXXLib::Exception(result));

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
} // namespace CXXLib
