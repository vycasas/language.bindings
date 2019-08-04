#include <dotslashzero/dotnetframeworklib/dotnetframeworklib.hxx>

#include <dotslashzero/clib/clib.h>

#include <algorithm>
#include <cassert>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <vcclr.h>

// The definitions of the opaque types are necessary to work around MSVC warnings for /clr.
// These types are only used in the clib interface to hide the actual definitions of the internal types.
struct DszCLibAddress_ { };
struct DszCLibPerson_ { };
struct DszCLibGenerator_ { };
struct DszCLibPrinter_ { };

#define DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD \
    try {

#define DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD \
    } \
    catch (std::exception const& e) { \
        throw (Core::CreateNewException(e)); \
    } \
    catch (...) { \
        throw (Core::CreateNewException(L"An unknown error has occurred.")); \
    }

#define DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW \
    } \
    catch (...) { \
        bool const NO_EXCEPTION_CAUGHT = false; \
        assert(NO_EXCEPTION_CAUGHT); \
    }

#define DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum) \
    if (cLibErrorNum != DSZ_CLIB_ERROR_NUM_NO_ERROR) { \
        auto errorNumStr = DotSlashZero::DotNetFrameworkLib::Core::ErrorNumToStdString(cLibErrorNum); \
        throw (std::runtime_error(errorNumStr)); \
    }

namespace DotSlashZero::DotNetFrameworkLib
{
    namespace Core
    {
        using CliByteArray = cli::array<System::Byte>;
        using ByteVector = std::vector<std::byte>;

        static inline CliByteArray^ ByteVectorToCliByteArray(ByteVector const& byteVect)
        {
            auto cliByteArray = gcnew CliByteArray(static_cast<int>(byteVect.size()));

            for (int i = 0; i < byteVect.size(); i++)
                cliByteArray[i] = static_cast<System::Byte>(byteVect[i]);

            return (cliByteArray);
        }

        static inline ByteVector CliByteArrayToByteVector(CliByteArray^ cliByteArray)
        {
            ByteVector byteVector(
                static_cast<ByteVector::size_type>(cliByteArray->Length),
                ByteVector::value_type());

            for (int i = 0; i < cliByteArray->Length; i++)
                byteVector[i] = static_cast<std::byte>(cliByteArray[i]);

            return (byteVector);
        }

        static inline System::String^ StdStringToSystemString(std::string const& str)
        {
            return (gcnew System::String(str.c_str()));
        }

        static inline std::string SystemStringToStdString(System::String^ str)
        {
            auto utf8ByteArray = System::Text::Encoding::UTF8->GetBytes(str);
            auto utf8ByteVect = CliByteArrayToByteVector(utf8ByteArray);
            utf8ByteVect.push_back(std::byte());
            std::string result(reinterpret_cast<char const*>(utf8ByteVect.data()));
            return (result);
        }

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

        static inline Exception^ CreateNewException(System::String^ message)
        {
            return (gcnew Exception(message));
        }

        static inline Exception^ CreateNewException(std::exception const& e)
        {
            try
            {
                auto message = Core::StdStringToSystemString(e.what());
                return (CreateNewException(message));
            }
            catch (...)
            {
                bool const NO_EXCEPTION_CAUGHT = false;
                assert(NO_EXCEPTION_CAUGHT);
                return (CreateNewException(L"A severe error has occurred while throwing an exception."));
            }
        }

        using IGeneratorGcRoot = msclr::gcroot<DotNetFrameworkLib::IGenerator^>;
        using IGeneratorGcRootRawPtr = IGeneratorGcRoot*;

        static DszCLibErrorNum GenerateIntRedirect(
            int data,
            int* pInt,
            void* pUserData)
        {
            if (pUserData == nullptr)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            if (pInt == nullptr)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            auto pGenerator = reinterpret_cast<IGeneratorGcRootRawPtr>(pUserData);

            if (!pGenerator)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            *pInt = (*pGenerator)->GenerateInt(data);

            return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
        }

        static DszCLibErrorNum GenerateStringRedirect(
            int data,
            char* pString, std::size_t stringSize,
            std::size_t* pCharsWritten,
            void* pUserData)
        {
            if (pUserData == nullptr)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            auto pGenerator = reinterpret_cast<IGeneratorGcRootRawPtr>(pUserData);

            if (!pGenerator)
                return (DSZ_CLIB_ERROR_NUM_CALLBACK_ERROR);

            auto generatedString = (*pGenerator)->GenerateString(data);
            auto generatedStringStdString = Core::SystemStringToStdString(generatedString);

            std::size_t numChars = 0;

            if ((pString != nullptr) && (stringSize > 0)) {
                std::fill_n(pString, stringSize, '\0');
                auto const COPY_COUNT = (std::min)(stringSize, generatedStringStdString.size());
                std::copy_n(generatedStringStdString.cbegin(), COPY_COUNT, pString);
                pString[stringSize - 1] = '\0';
                numChars = std::string_view(pString).size();
            }
            else {
                numChars = generatedStringStdString.size();
            }

            if (pCharsWritten != nullptr)
                *pCharsWritten = numChars;

            return (DSZ_CLIB_ERROR_NUM_NO_ERROR);
        }

        class DszCLibGeneratorGuarded final
        {
        public:
            DszCLibGeneratorGuarded(DszCLibGenerator cLibGenerator)
                : m_cLibGenerator(cLibGenerator)
            { return; }

            ~DszCLibGeneratorGuarded()
            {
                if (m_cLibGenerator != DSZ_CLIB_GENERATOR_INVALID) {
                    DszCLibGeneratorDestroy(m_cLibGenerator);
                    m_cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;
                }
                return;
            }

            explicit operator bool() const
            {
                return (m_cLibGenerator != DSZ_CLIB_GENERATOR_INVALID);
            }

            operator DszCLibGenerator() const
            {
                return (m_cLibGenerator);
            }

            DszCLibGenerator Release()
            {
                auto cLibGenerator = m_cLibGenerator;
                m_cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;
                return (cLibGenerator);
            }
        private:
            DszCLibGenerator m_cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;
        };

        static DszCLibGeneratorGuarded CreateNewGenerator()
        {
            auto cLibGenerator = DSZ_CLIB_GENERATOR_INVALID;

            DszCLibGeneratorCreate(
                &Core::GenerateIntRedirect,
                &Core::GenerateStringRedirect,
                &cLibGenerator);

            return (DszCLibGeneratorGuarded(cLibGenerator));
        }
    }
    // namespace Core

    Exception::Exception(System::String^ message) :
        System::Exception(message)
    { return; }

    Exception::~Exception()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        this->!Exception();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    Exception::!Exception()
    { 
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    // static
    System::Boolean Library::Initialize()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto cLibErrorNum = DszCLibLibraryInitialize();
        return (cLibErrorNum == DSZ_CLIB_ERROR_NUM_NO_ERROR);

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static
    System::Void Library::Uninitialize()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        DszCLibLibraryUninitialize();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::String^ Library::VersionString::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        std::string::size_type constexpr VERSION_STRING_SIZE = 16;
        std::string versionString(VERSION_STRING_SIZE, '\0');

        auto cLibErrorNum = DszCLibLibraryGetVersionString(
            &(versionString.front()),
            versionString.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(versionString));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::Int32 Library::VersionMajor::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        size_t versionMajor = 0;

        auto cLibErrorNum = DszCLibLibraryGetVersionMajor(&versionMajor);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (static_cast<System::Int32>(versionMajor));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::Int32 Library::VersionMinor::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        size_t versionMinor = 0;

        auto cLibErrorNum = DszCLibLibraryGetVersionMajor(&versionMinor);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (static_cast<System::Int32>(versionMinor));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::Int32 Library::VersionPatch::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        size_t versionPatch = 0;

        auto cLibErrorNum = DszCLibLibraryGetVersionMajor(&versionPatch);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (static_cast<System::Int32>(versionPatch));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::String^ Library::VersionExtra::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        std::string::size_type constexpr VERSION_EXTRA_SIZE = 16;
        std::string versionExtra(VERSION_EXTRA_SIZE, '\0');

        auto cLibErrorNum = DszCLibLibraryGetVersionString(
            &(versionExtra.front()),
            versionExtra.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(versionExtra));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    Address::Address(
        System::Int32 streetNum,
        System::String^ street,
        System::String^ city,
        System::String^ province,
        System::String^ zipCode,
        System::String^ country)
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto streetNumInt = static_cast<int>(streetNum);
        auto streetStdStr = Core::SystemStringToStdString(street);
        auto cityStdStr = Core::SystemStringToStdString(city);
        auto provinceStdStr = Core::SystemStringToStdString(province);
        auto zipCodeStdStr = Core::SystemStringToStdString(zipCode);
        auto countryStdStr = Core::SystemStringToStdString(country);

        auto cLibAddress = DSZ_CLIB_ADDRESS_INVALID;

        auto cLibErrorNum = DszCLibAddressCreate(
            streetNumInt,
            streetStdStr.c_str(),
            cityStdStr.c_str(),
            provinceStdStr.c_str(),
            zipCodeStdStr.c_str(),
            countryStdStr.c_str(),
            &cLibAddress);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        m_pImpl = reinterpret_cast<std::intptr_t>(cLibAddress);

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    Address::Address(std::intptr_t const& pImpl)
        : m_pImpl(pImpl)
    {
        return;
    }

    Address::~Address()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        this->!Address();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    Address::!Address()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        if (m_pImpl != 0) {
            auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);
            DszCLibAddressDestroy(cLibAddress);
            m_pImpl = 0;
        }

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    // property
    System::Int32 Address::StreetNum::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);
  
        int streetNum = 0;

        auto cLibErrorNum = DszCLibAddressGetStreetNum(
            cLibAddress,
            &streetNum);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (static_cast<System::Int32>(streetNum));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::Street::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);

        std::string::size_type STREET_SIZE = 16;
        std::string street(STREET_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetStreet(
            cLibAddress,
            &(street.front()), street.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(street));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::City::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);

        std::string::size_type CITY_SIZE = 16;
        std::string city(CITY_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetCity(
            cLibAddress,
            &(city.front()), city.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(city));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::Province::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);

        std::string::size_type PROVINCE_SIZE = 16;
        std::string province(PROVINCE_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetProvince(
            cLibAddress,
            &(province.front()), province.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(province));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::ZipCode::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);

        std::string::size_type ZIP_CODE_SIZE = 16;
        std::string zipCode(ZIP_CODE_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetZipCode(
            cLibAddress,
            &(zipCode.front()), zipCode.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(zipCode));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::Country::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);

        std::string::size_type COUNTRY_SIZE = 16;
        std::string country(COUNTRY_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressGetCountry(
            cLibAddress,
            &(country.front()), country.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(country));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    System::String^ Address::ToString() /*override*/
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(m_pImpl);

        std::string::size_type ADDRESS_STRING_SIZE = 80;
        std::string addressString(ADDRESS_STRING_SIZE, '\0');

        auto cLibErrorNum = DszCLibAddressToString(
            cLibAddress,
            &(addressString.front()), addressString.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(addressString));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    std::intptr_t Address::GetImpl()
    {
        return (m_pImpl);
    }

    Person::Person(
        System::String^ lastName,
        System::String^ firstName,
        System::Int32 age,
        DotNetFrameworkLib::Address^ address)
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto lastNameStdStr = Core::SystemStringToStdString(lastName);
        auto firstNameStdStr = Core::SystemStringToStdString(firstName);
        auto ageInt = static_cast<int>(age);
        auto pAddressImpl = address->GetImpl();

        assert(pAddressImpl != 0);

        auto cLibAddress = reinterpret_cast<DszCLibAddress>(pAddressImpl);
        auto cLibPerson = DSZ_CLIB_PERSON_INVALID;

        auto cLibErrorNum = DszCLibPersonCreate(
            lastNameStdStr.c_str(),
            firstNameStdStr.c_str(),
            ageInt,
            cLibAddress,
            &cLibPerson);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        m_pImpl = reinterpret_cast<std::intptr_t>(cLibPerson);

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    Person::~Person()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        this->!Person();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    Person::!Person()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        if (m_pImpl != 0) {
            auto cLibPerson = reinterpret_cast<DszCLibPerson>(m_pImpl);
            DszCLibPersonDestroy(cLibPerson);
            m_pImpl = 0;
        }

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    // property
    System::String^ Person::LastName::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibPerson = reinterpret_cast<DszCLibPerson>(m_pImpl);

        std::string::size_type LAST_NAME_SIZE = 16;
        std::string lastName(LAST_NAME_SIZE, '\0');

        auto cLibErrorNum = DszCLibPersonGetLastName(
            cLibPerson,
            &(lastName.front()), lastName.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(lastName));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Person::FirstName::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibPerson = reinterpret_cast<DszCLibPerson>(m_pImpl);

        std::string::size_type FIRST_NAME_SIZE = 16;
        std::string firstName(FIRST_NAME_SIZE, '\0');

        auto cLibErrorNum = DszCLibPersonGetFirstName(
            cLibPerson,
            &(firstName.front()), firstName.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(firstName));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::Int32 Person::Age::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibPerson = reinterpret_cast<DszCLibPerson>(m_pImpl);

        int age = 0;

        auto cLibErrorNum = DszCLibPersonGetAge(
            cLibPerson,
            &age);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (static_cast<System::Int32>(age));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    DotNetFrameworkLib::Address^ Person::Address::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibPerson = reinterpret_cast<DszCLibPerson>(m_pImpl);
        auto cLibAddress = DSZ_CLIB_ADDRESS_INVALID;

        auto cLibErrorNum = DszCLibPersonGetAddress(
            cLibPerson,
            &cLibAddress);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        auto address = gcnew DotNetFrameworkLib::Address(reinterpret_cast<std::intptr_t>(cLibAddress));

        return (address);

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    System::String^ Person::ToString() /*override*/
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibPerson = reinterpret_cast<DszCLibPerson>(m_pImpl);

        std::string::size_type PERSON_STRING_SIZE = 160;
        std::string personString(PERSON_STRING_SIZE, '\0');

        auto cLibErrorNum = DszCLibPersonToString(
            cLibPerson,
            &(personString.front()), personString.size(),
            nullptr);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        return (Core::StdStringToSystemString(personString));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    Printer::Printer(IGenerator^ generator)
        : m_generator(generator)
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto cLibGenerator = Core::CreateNewGenerator();

        auto cLibPrinter = DSZ_CLIB_PRINTER_INVALID;

        auto cLibErrorNum = DszCLibPrinterCreate(
            cLibGenerator.Release(),
            &cLibPrinter);

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        m_pImpl = reinterpret_cast<std::intptr_t>(cLibPrinter);

        return;
        
        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    Printer::~Printer()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        this->!Printer();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    Printer::!Printer()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        if (m_pImpl != 0) {
            auto cLibPrinter = reinterpret_cast<DszCLibPrinter>(m_pImpl);
            DszCLibPrinterDestroy(cLibPrinter);
            m_pImpl = 0;
        }

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    System::Void Printer::PrintInt()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibPrinter = reinterpret_cast<DszCLibPrinter>(m_pImpl);

        std::unique_ptr<Core::IGeneratorGcRoot> pGenerator(new Core::IGeneratorGcRoot(m_generator));

        auto cLibErrorNum = DszCLibPrinterPrintIntWithUserData(
            cLibPrinter,
            reinterpret_cast<void*>(pGenerator.get()));

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    System::Void Printer::PrintString()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != 0);

        auto cLibPrinter = reinterpret_cast<DszCLibPrinter>(m_pImpl);

        std::unique_ptr<Core::IGeneratorGcRoot> pGenerator(new Core::IGeneratorGcRoot(m_generator));

        auto cLibErrorNum = DszCLibPrinterPrintStringWithUserData(
            cLibPrinter,
            reinterpret_cast<void*>(pGenerator.get()));

        DSZ_DOTNETFRAMEWORKLIBCORE_CHECK_ERROR_NUM(cLibErrorNum);

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }
}
// namespace DotSlashZero::DotNetFrameworkLib
