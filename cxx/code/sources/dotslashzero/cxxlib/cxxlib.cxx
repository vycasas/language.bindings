#include <dotslashzero/cxxlib/cxxlib.hxx>

#include <algorithm>
#include <functional>
#include <string_view>

#define DSZ_CXXLIB_CALLING_CONVENTION
#if defined(_MSC_VER)
    #undef DSZ_CXXLIB_CALLING_CONVENTION
    #define DSZ_CXXLIB_CALLING_CONVENTION __stdcall
#endif /* defined(_MSC_VER) */

#define DSZ_CXXLIBCORE_API_CHECK(errorNum) \
    if (errorNum != DSZ_CLIB_ERROR_NUM_NO_ERROR) \
        throw (DotSlashZero::CxxLib::Exception(errorNum));

namespace DotSlashZero::CxxLib
{
    namespace Core
    {
        static inline std::string ErrorNumToStdString(DszCLibErrorNum cLibErrorNum)
        {
            std::string::size_type constexpr ERROR_NUM_STRING_SIZE = 40;
            std::string errorNumString(ERROR_NUM_STRING_SIZE, '\0');

            DszCLibErrorNumGetMessage(
                cLibErrorNum,
                &(errorNumString.front()), errorNumString.size(),
                nullptr);

            return (errorNumString.c_str());
        }

        static DszCLibErrorNum DSZ_CXXLIB_CALLING_CONVENTION GenerateIntRedirect(
            int data,
            int* pInt,
            void* pUserData)
        {
            if (pUserData == nullptr)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            if (pInt == nullptr)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            auto pGenerator = reinterpret_cast<IGenerator*>(pUserData);

            if (!pGenerator)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            *pInt = pGenerator->GenerateInt(data);

            return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
        }

        static DszCLibErrorNum DSZ_CXXLIB_CALLING_CONVENTION GenerateStringRedirect(
            int data,
            char* pString, std::size_t stringSize,
            std::size_t* pCharsWritten,
            void* pUserData)
        {
            if (pUserData == nullptr)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            auto pGenerator = reinterpret_cast<IGenerator*>(pUserData);

            if (!pGenerator)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            auto generatedString = pGenerator->GenerateString(data);

            std::size_t numChars = 0;

            if ((pString != nullptr) && (stringSize > 0)) {
                std::fill_n(pString, stringSize, '\0');
                auto const COPY_COUNT = (std::min)(stringSize, generatedString.size());
                std::copy_n(generatedString.cbegin(), COPY_COUNT, pString);
                pString[stringSize - 1] = '\0';
                numChars = std::string_view(pString).size();
            }
            else {
                numChars = generatedString.size();
            }

            if (pCharsWritten != nullptr)
                *pCharsWritten = numChars;

            return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
        }
    }
    // namespace Core

    Exception::Exception(void) :
        std::runtime_error{""}
    { return; }

    Exception::Exception(DszCLibErrorNum errorNum) :
        std::runtime_error{ Core::ErrorNumToStdString(errorNum) }
    { return; }

    std::string Exception::GetMessage(void) const noexcept
    {
        return (what());
    }

    namespace Library
    {
        bool Initialize(void) noexcept
        {
            auto cLibErrorNum = DszCLibLibraryInitialize();

            return (cLibErrorNum == DSZ_CLIB_ERROR_NUM_NO_ERROR);
        }

        void Uninitialize(void) noexcept
        {
            DszCLibLibraryUninitialize();

            return;
        }

        std::string GetVersionString(void)
        {
            std::string::size_type constexpr VERSION_STRING_SIZE = 16;
            std::string versionString(VERSION_STRING_SIZE, '\0');

            DszCLibLibraryGetVersionString(
                versionString.data(), versionString.size(),
                nullptr);

            return (versionString.c_str());
        }

        std::size_t GetVersionMajor(void)
        {
            std::size_t versionMajor = 0;

            DszCLibLibraryGetVersionMajor(&versionMajor);

            return (versionMajor);
        }

        std::size_t GetVersionMinor(void)
        {
            std::size_t versionMinor = 0;

            DszCLibLibraryGetVersionMinor(&versionMinor);

            return (versionMinor);
        }

        std::size_t GetVersionPatch(void)
        {
            std::size_t versionPatch = 0;

            DszCLibLibraryGetVersionPatch(&versionPatch);

            return (versionPatch);
        }

        std::string GetVersionExtra(void)
        {
            std::string::size_type constexpr VERSION_EXTRA_STRING_SIZE = 16;
            std::string versionExtraString(VERSION_EXTRA_STRING_SIZE, '\0');

            DszCLibLibraryGetVersionExtra(
                versionExtraString.data(), versionExtraString.size(),
                nullptr);

            return (versionExtraString.c_str());
        }
    }
    // namespace Library

    Address::Address(void) :
        m_impl{ DSZ_CLIB_ADDRESS_INVALID }
    { return; }

    Address::Address(
        int streetNum,
        std::string const& street,
        std::string const& city,
        std::string const& province,
        std::string const& zipCode,
        std::string const& country) :
        m_impl{ DSZ_CLIB_ADDRESS_INVALID }
    {
        auto cLibErrorNum = DszCLibAddressCreate(
            streetNum,
            street.c_str(),
            city.c_str(),
            province.c_str(),
            zipCode.c_str(),
            country.c_str(),
            &m_impl);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return;
    }

    Address::Address(Address const& address)
    {
        Destroy__();

        auto cLibErrorNum = DszCLibAddressCreate(
            address.GetStreetNum(),
            address.GetStreet().c_str(),
            address.GetCity().c_str(),
            address.GetProvince().c_str(),
            address.GetZipCode().c_str(),
            address.GetCountry().c_str(),
            &m_impl);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return;
    }

    Address::~Address(void) noexcept
    {
        Destroy__();

        return;
    }

    Address& Address::operator=(Address const& address)
    {
        Destroy__();

        auto cLibErrorNum = DszCLibAddressCreate(
            address.GetStreetNum(),
            address.GetStreet().c_str(),
            address.GetCity().c_str(),
            address.GetProvince().c_str(),
            address.GetZipCode().c_str(),
            address.GetCountry().c_str(),
            &m_impl);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (*this);
    }

    int Address::GetStreetNum(void) const
    {
        int streetNum = 0;

        auto cLibErrorNum = DszCLibAddressGetStreetNum(
            m_impl,
            &streetNum);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (streetNum);
    }

    std::string Address::GetStreet(void) const
    {
        std::string::size_type constexpr STREET_SIZE = 16;
        std::string street(STREET_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetStreet(
            m_impl,
            street.data(), street.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (street.c_str());
    }

    std::string Address::GetCity(void) const
    {
        std::string::size_type constexpr CITY_SIZE = 16;
        std::string city(CITY_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetCity(
            m_impl,
            city.data(), city.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (city.c_str());
    }

    std::string Address::GetProvince(void) const
    {
        std::string::size_type constexpr PROVINCE_SIZE = 16;
        std::string province(PROVINCE_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetProvince(
            m_impl,
            province.data(), province.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (province.c_str());
    }

    std::string Address::GetZipCode(void) const
    {
        std::string::size_type constexpr ZIP_CODE_SIZE = 16;
        std::string zipCode(ZIP_CODE_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetCountry(
            m_impl,
            zipCode.data(), zipCode.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (zipCode.c_str());
    }

    std::string Address::GetCountry(void) const
    {
        std::string::size_type constexpr COUNTRY_SIZE = 16;
        std::string country(COUNTRY_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetCountry(
            m_impl,
            country.data(), country.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (country.c_str());
    }

    std::string Address::ToString(void) const
    {
        std::string::size_type constexpr ADDRESS_STRING_SIZE = 80;
        std::string addressString(ADDRESS_STRING_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressToString(
            m_impl,
            addressString.data(), addressString.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (addressString.c_str());
    }

    void Address::Destroy__() noexcept
    {
        if (m_impl != DSZ_CLIB_ADDRESS_INVALID) {
            DszCLibAddressDestroy(m_impl);
            m_impl = DSZ_CLIB_ADDRESS_INVALID;
        }

        return;
    }

    Person::Person(
        std::string const& lastName,
        std::string const& firstName,
        int age,
        Address const& address) :
        m_impl{ DSZ_CLIB_PERSON_INVALID }
    {
        // Remember: address.m_impl is copied.
        auto cLibErrorNum = DszCLibPersonCreate(
            lastName.c_str(),
            firstName.c_str(),
            age,
            address.m_impl,
            &m_impl);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return;
    }

    Person::Person(Person const& person)
    {
        Destroy__();

        auto cLibErrorNum = DszCLibPersonCreate(
            person.GetLastName().c_str(),
            person.GetFirstName().c_str(),
            person.GetAge(),
            person.GetAddress().m_impl,
            &m_impl);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return;
    }

    Person::~Person(void) noexcept
    {
        Destroy__();

        return;
    }

    Person& Person::operator=(Person const& person)
    {
        Destroy__();

        auto cLibErrorNum = DszCLibPersonCreate(
            person.GetLastName().c_str(),
            person.GetFirstName().c_str(),
            person.GetAge(),
            person.GetAddress().m_impl,
            &m_impl);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (*this);
    }

    std::string Person::GetLastName(void) const
    {
        std::string::size_type const LAST_NAME_SIZE = 16;
        std::string lastName(LAST_NAME_SIZE, '\0');

        auto cLibErrorNum = DszCLibPersonGetLastName(
            m_impl,
            lastName.data(), lastName.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (lastName.c_str());
    }

    std::string Person::GetFirstName(void) const
    {
        std::string::size_type const FIRST_NAME_SIZE = 16;
        std::string firstName(FIRST_NAME_SIZE, '\0');

        auto cLibErrorNum = DszCLibPersonGetFirstName(
            m_impl,
            firstName.data(), firstName.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (firstName.c_str());
    }

    int Person::GetAge(void) const
    {
        int age = 0;

        auto cLibErrorNum = DszCLibPersonGetAge(
            m_impl,
            &age);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (age);
    }

    Address Person::GetAddress(void) const
    {
        Address address;

        auto cLibErrorNum = DszCLibPersonGetAddress(
            m_impl,
            &(address.m_impl));

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (address);
    }

    std::string Person::ToString(void) const
    {
        std::string::size_type PERSON_STRING_SIZE = 160;
        std::string personString(PERSON_STRING_SIZE, '\0');

        auto cLibErrorNum = DszCLibPersonToString(
            m_impl,
            personString.data(), personString.size(),
            nullptr);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return (personString.c_str());
    }

    void Person::Destroy__() noexcept
    {
        if (m_impl != DSZ_CLIB_PERSON_INVALID) {
            DszCLibPersonDestroy(m_impl);
            m_impl = DSZ_CLIB_PERSON_INVALID;
        }

        return;
    }

    Printer::Printer(IGenerator* pGenerator) :
        m_impl{ DSZ_CLIB_PRINTER_INVALID },
        m_pGenerator{ pGenerator }
    {
        DszCLibGenerator cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;

        auto cLibErrorNum = DszCLibGeneratorCreate(
            (DszCLibGenerateIntFunction) &(Core::GenerateIntRedirect),
            (DszCLibGenerateStringFunction) &(Core::GenerateStringRedirect),
            &cLibGenerator);

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        cLibErrorNum = DszCLibPrinterCreate(
            cLibGenerator,
            &m_impl); // note: this takes ownership of generatorImpl, so don't destroy the instance

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return;
    }

    Printer::~Printer(void) noexcept
    {
        Destroy__();

        return;
    }

    void Printer::PrintInt(void) const
    {
        auto cLibErrorNum = DszCLibPrinterPrintIntWithUserData(
            m_impl,
            const_cast<void*>(reinterpret_cast<void const*>(m_pGenerator.get())));

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return;
    }

    void Printer::PrintString(void) const
    {
        auto cLibErrorNum = DszCLibPrinterPrintStringWithUserData(
            m_impl,
            const_cast<void*>(reinterpret_cast<void const*>(m_pGenerator.get())));

        DSZ_CXXLIBCORE_API_CHECK(cLibErrorNum);

        return;
    }

    void Printer::Destroy__() noexcept
    {
        if (m_impl != DSZ_CLIB_PRINTER_INVALID) {
            DszCLibPrinterDestroy(m_impl);
            m_impl = DSZ_CLIB_PRINTER_INVALID;
        }

        return;
    }
}
// namespace DotSlashZero::CXXLib
