#include <dotslashzero/dotnetframeworklib/dotnetframeworklib.hxx>

#include <dotslashzero/cxxlib/cxxlib.hxx>

#include <cassert>
#include <vector>

#include <vcclr.h>

#define DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD \
    try {

#define DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD \
    } \
    catch (DotSlashZero::CxxLib::Exception const& e) { \
        throw (Core::CreateNewException(e)); \
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

        static inline Exception^ CreateNewException(System::String^ message)
        {
            return (gcnew Exception(message));
        }

        static inline Exception^ CreateNewException(CxxLib::Exception const& e)
        {
            try
            {
                auto message = Core::StdStringToSystemString(e.GetMessage());
                return (CreateNewException(message));
            }
            catch (...)
            {
                bool const NO_EXCEPTION_CAUGHT= false;
                assert(NO_EXCEPTION_CAUGHT);
                return (CreateNewException(L"A severe error has occurred while throwing an exception."));
            }
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

        class GeneratorImpl final : public CxxLib::IGenerator
        {
        public:
            GeneratorImpl(DotNetFrameworkLib::IGenerator^ generator) :
                m_rGenerator(generator)
            { return; }

            ~GeneratorImpl(void) noexcept
            {
                m_rGenerator = nullptr;
                return;
            }

            int GenerateInt(int data) const override
            {
                try
                {
                    auto result = m_rGenerator->GenerateInt(static_cast<System::Int32>(data));

                    return (static_cast<int>(result));
                }
                catch (System::Exception^ e)
                {
                    throw (std::runtime_error(Core::SystemStringToStdString(e->Message)));
                }
            }

            std::string GenerateString(int data) const override
            {
                try
                {
                    auto result = m_rGenerator->GenerateString(static_cast<System::Int32>(data));

                    return (Core::SystemStringToStdString(result));
                }
                catch (System::Exception^ e)
                {
                    throw (std::runtime_error(Core::SystemStringToStdString(e->Message)));
                }
            }

        private:
            msclr::gcroot<DotNetFrameworkLib::IGenerator^> m_rGenerator;
        };
        // class GeneratorImpl
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

        auto initOk = CxxLib::Library::Initialize();

        return (static_cast<System::Boolean>(initOk));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static
    System::Void Library::Uninitialize()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        CxxLib::Library::Uninitialize();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::String^ Library::VersionString::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto const& versionString = CxxLib::Library::GetVersionString();

        return (Core::StdStringToSystemString(versionString));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::Int32 Library::VersionMajor::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto const& versionMajor = CxxLib::Library::GetVersionMajor();

        return (static_cast<System::Int32>(versionMajor));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::Int32 Library::VersionMinor::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto const& versionMinor = CxxLib::Library::GetVersionMinor();

        return (static_cast<System::Int32>(versionMinor));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::Int32 Library::VersionPatch::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto const& versionPatch = CxxLib::Library::GetVersionPatch();

        return (static_cast<System::Int32>(versionPatch));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // static property
    System::String^ Library::VersionExtra::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto const& versionString = CxxLib::Library::GetVersionExtra();

        return (Core::StdStringToSystemString(versionString));

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

        auto pAddress = std::make_unique<CxxLib::Address>(
            streetNumInt,
            streetStdStr,
            cityStdStr,
            provinceStdStr,
            zipCodeStdStr,
            countryStdStr);

        m_pImpl = pAddress.release();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
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

        if (m_pImpl != nullptr) {
            std::unique_ptr<CxxLib::Address> pAddress(m_pImpl); // a pattern to free objects without using delete explicitly
            m_pImpl = nullptr;
        }

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    // property
    System::Int32 Address::StreetNum::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& streetNum = m_pImpl->GetStreetNum();

        return (static_cast<System::Int32>(streetNum));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::Street::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& street = m_pImpl->GetStreet();

        return (Core::StdStringToSystemString(street));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::City::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& city = m_pImpl->GetCity();

        return (Core::StdStringToSystemString(city));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::Province::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& province = m_pImpl->GetProvince();

        return (Core::StdStringToSystemString(province));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::ZipCode::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& zipCode = m_pImpl->GetZipCode();

        return (Core::StdStringToSystemString(zipCode));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Address::Country::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& country = m_pImpl->GetCountry();

        return (Core::StdStringToSystemString(country));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    System::String^ Address::ToString() /*override*/
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& addressString = m_pImpl->ToString();

        return (Core::StdStringToSystemString(addressString));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    CxxLib::Address* Address::GetImpl()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        return (m_pImpl);

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
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

        assert(pAddressImpl != nullptr);

        auto pPerson = std::make_unique<CxxLib::Person>(
            lastNameStdStr,
            firstNameStdStr,
            ageInt,
            *pAddressImpl);

        m_pImpl = pPerson.release();

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

        if (m_pImpl != nullptr) {
            std::unique_ptr<CxxLib::Person> pPerson(m_pImpl);
            m_pImpl = nullptr;
        }

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    // property
    System::String^ Person::LastName::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& lastName = m_pImpl->GetLastName();

        return (Core::StdStringToSystemString(lastName));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::String^ Person::FirstName::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& firstName = m_pImpl->GetFirstName();

        return (Core::StdStringToSystemString(firstName));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    System::Int32 Person::Age::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& age = m_pImpl->GetAge();

        return (static_cast<System::Int32>(age));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    // property
    DotNetFrameworkLib::Address^ Person::Address::get()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& address = m_pImpl->GetAddress();

        auto streetNum = static_cast<System::Int32>(address.GetStreetNum());
        auto street = Core::StdStringToSystemString(address.GetStreet());
        auto city = Core::StdStringToSystemString(address.GetCity());
        auto province = Core::StdStringToSystemString(address.GetProvince());
        auto zipCode = Core::StdStringToSystemString(address.GetZipCode());
        auto country = Core::StdStringToSystemString(address.GetCountry());

        return (gcnew DotNetFrameworkLib::Address(
            streetNum,
            street,
            city,
            province,
            zipCode,
            country));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    System::String^ Person::ToString() /*override*/
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        auto const& personString = m_pImpl->ToString();

        return (Core::StdStringToSystemString(personString));

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    Printer::Printer(IGenerator^ generator)
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        auto pGenerator = std::make_unique<Core::GeneratorImpl>(generator);

        auto pPrinter = std::make_unique<CxxLib::Printer>(pGenerator.release());

        m_pImpl = pPrinter.release();

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

        if (m_pImpl != nullptr) {
            std::unique_ptr<CxxLib::Printer> pPrinter(m_pImpl);
            m_pImpl = nullptr;
        }

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD_NO_THROW;
    }

    System::Void Printer::PrintInt()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        m_pImpl->PrintInt();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }

    System::Void Printer::PrintString()
    {
        DSZ_DOTNETFRAMEWORKLIBCORE_BEGIN_EX_GUARD;

        assert(m_pImpl != nullptr);

        m_pImpl->PrintString();

        return;

        DSZ_DOTNETFRAMEWORKLIBCORE_END_EX_GUARD;
    }
}
// namespace DotSlashZero::DotNetFrameworkLib
