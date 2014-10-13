#include "api_dotnet.hxx"

#include <vcclr.h>

#define BEGIN_EX_GUARD \
    try {
#define END_EX_GUARD \
    } \
    catch (const CXXLib::Exception& e) { \
        throw (gcnew DotNetLib::Exception(e)); \
    } \
    catch (std::exception& e) { \
        throw (gcnew DotNetLib::Exception(CXXLib::Exception(e))); \
    }

namespace DotNetLibCore
{
    namespace Utils
    {
        inline System::String^ CXXStdStringToSystemString(const std::string& str)
        {
            System::String^ result = gcnew System::String(str.c_str());
            return (result);
        }

        inline std::string SystemStringToCXXStdString(System::String^ str)
        {
            std::string result;
            array<unsigned char>^ byteArray = System::Text::Encoding::UTF8->GetBytes(str);
            for (int i = 0; i < byteArray->Length; i++) {
                result.push_back(static_cast<std::string::value_type>(byteArray[i]));
            }
            return (result);
        }
    } // namespace Utils

    class DotNetLibException final : public CXXLib::Exception
    {
    public:
        DotNetLibException(void) : CXXLib::Exception()
        { return; }

        DotNetLibException(CLibErrNum errNum) : CXXLib::Exception(errNum)
        { return; }
    }; // class DotNetLibException

    class DotNetLibGeneratorImpl final : public CXXLib::GeneratorBase
    {
    public:
        DotNetLibGeneratorImpl(DotNetLib::IGenerator^ generator);

        // We need to implement 

        virtual ~DotNetLibGeneratorImpl(void) override;

        virtual int generateInt(int data) const override;
        virtual std::string generateString(int data) const override;

        gcroot<DotNetLib::IGenerator^> _generatorPtr;
    };

    DotNetLibGeneratorImpl::DotNetLibGeneratorImpl(DotNetLib::IGenerator^ generator) :
        _generatorPtr(generator)
    { return; }

    DotNetLibGeneratorImpl::~DotNetLibGeneratorImpl(void)
    { return; }

    int DotNetLibGeneratorImpl::generateInt(int data) const
    {
        try {
            return (_generatorPtr->GenerateInt(data));
        }
        catch (System::Exception^) {
            throw (DotNetLibException(2));
        }
    }

    std::string DotNetLibGeneratorImpl::generateString(int data) const
    {
        try {
            return (
                DotNetLibCore::Utils::SystemStringToCXXStdString(_generatorPtr->GenerateString(data))
            );
        }
        catch (System::Exception^) {
            throw (DotNetLibException(2));
        }
    }

    // Unfortunately, due to Microsoft's error, we have to extend the CXXLib::Printer API to workaround with having
    // an std::unique_ptr as a parameter. Apparently, this issue only occurs when compiling with "/clr" flags (or any of
    // its variants).
    // See here for details:
    // https://connect.microsoft.com/VisualStudio/feedback/details/858243
    class CXXLibPrinterAPIWorkAround : public CXXLib::Printer
    {
    public:
        // The fix is to pass std::unique_ptr by r-value reference.
        CXXLibPrinterAPIWorkAround(std::unique_ptr<CXXLib::GeneratorBase>&& generator);

        virtual ~CXXLibPrinterAPIWorkAround(void) override;
    }; // class CXXLibPrinterAPIWorkAround

    CXXLibPrinterAPIWorkAround::CXXLibPrinterAPIWorkAround(std::unique_ptr<CXXLib::GeneratorBase>&& generator)
    {
        this->createInstance(std::move(generator));
        return;
    }

    CXXLibPrinterAPIWorkAround::~CXXLibPrinterAPIWorkAround(void)
    { return; }

} // namespace DotNetLibCore

namespace DotNetLib
{
    Exception::Exception(void) :
        System::Exception(),
        _impl(nullptr)
    { return; }

    Exception::!Exception(void)
    {
        this->CleanUp();
        return;
    }

    Exception::~Exception(void)
    {
        this->CleanUp();
        return;
    }

    Exception::Exception(const CXXLib::Exception& e) :
        System::Exception(DotNetLibCore::Utils::CXXStdStringToSystemString(e.getMessage()))
    {
        _impl = new CXXLib::Exception(e);
        return;
    }

    void Exception::CleanUp(void)
    {
        if (_impl != nullptr) {
            delete (_impl);
            _impl = nullptr;
        }
        return;
    }

    void Library::Initialize(void)
    {
        BEGIN_EX_GUARD;
        CXXLib::Library::initialize();
        END_EX_GUARD;
        return;
    }

    void Library::Terminate(void)
    {
        BEGIN_EX_GUARD;
        CXXLib::Library::terminate();
        END_EX_GUARD;
        return;
    }

    Address::Address(
        int streetNum,
        System::String^ street,
        System::String^ city,
        System::String^ province,
        System::String^ country,
        System::String^ zipCode
    ) : _impl(nullptr)
    {
        BEGIN_EX_GUARD;
        _impl = new CXXLib::Address(
            streetNum,
            DotNetLibCore::Utils::SystemStringToCXXStdString(street),
            DotNetLibCore::Utils::SystemStringToCXXStdString(city),
            DotNetLibCore::Utils::SystemStringToCXXStdString(province),
            DotNetLibCore::Utils::SystemStringToCXXStdString(country),
            DotNetLibCore::Utils::SystemStringToCXXStdString(zipCode)
        );
        END_EX_GUARD;
        return;
    }

    Address::!Address(void)
    {
        this->CleanUp();
        return;
    }

    Address::~Address(void)
    {
        this->CleanUp();
        return;
    }

    int Address::StreetNum::get(void)
    {
        BEGIN_EX_GUARD;
        return (_impl->getStreetNum());
        END_EX_GUARD;
    }

    System::String^ Address::Street::get(void)
    {
        BEGIN_EX_GUARD;
        return (DotNetLibCore::Utils::CXXStdStringToSystemString(_impl->getStreet()));
        END_EX_GUARD;
    }

    System::String^ Address::City::get(void)
    {
        BEGIN_EX_GUARD;
        return (DotNetLibCore::Utils::CXXStdStringToSystemString(_impl->getCity()));
        END_EX_GUARD;
    }

    System::String^ Address::Province::get(void)
    {
        BEGIN_EX_GUARD;
        return (DotNetLibCore::Utils::CXXStdStringToSystemString(_impl->getProvince()));
        END_EX_GUARD;
    }

    System::String^ Address::Country::get(void)
    {
        BEGIN_EX_GUARD;
        return (DotNetLibCore::Utils::CXXStdStringToSystemString(_impl->getCountry()));
        END_EX_GUARD;
    }

    System::String^ Address::ZipCode::get(void)
    {
        BEGIN_EX_GUARD;
        return (DotNetLibCore::Utils::CXXStdStringToSystemString(_impl->getZipCode()));
        END_EX_GUARD;
    }

    System::String^ Address::ToString(void)
    {
        System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
        sb->Append(this->StreetNum);
        sb->Append(L" ");
        sb->Append(this->Street);
        sb->Append(System::Environment::NewLine);
        sb->Append(this->City);
        sb->Append(L" ");
        sb->Append(this->Province);
        sb->Append(System::Environment::NewLine);
        sb->Append(this->Country);
        sb->Append(L" ");
        sb->Append(this->ZipCode);
        return (sb->ToString());
    }

    void Address::CleanUp(void)
    {
        if (_impl != nullptr) {
            delete (_impl);
            _impl = nullptr;
        }
        return;
    }

    Person::Person(System::String^ lastName, System::String^ firstName, int age, DotNetLib::Address^ address) :
        _impl(nullptr)
    {
        BEGIN_EX_GUARD;
        _impl = new CXXLib::Person(
            DotNetLibCore::Utils::SystemStringToCXXStdString(lastName),
            DotNetLibCore::Utils::SystemStringToCXXStdString(firstName),
            age,
            *(address->GetImpl())
        );
        END_EX_GUARD;
        return;
    }

    Person::!Person(void)
    {
        this->CleanUp();
        return;
    }

    Person::~Person(void)
    {
        this->CleanUp();
        return;
    }

    System::String^ Person::LastName::get(void)
    {
        BEGIN_EX_GUARD;
        return (DotNetLibCore::Utils::CXXStdStringToSystemString(_impl->getLastName()));
        END_EX_GUARD;
    }

    System::String^ Person::FirstName::get(void)
    {
        BEGIN_EX_GUARD;
        return (DotNetLibCore::Utils::CXXStdStringToSystemString(_impl->getFirstName()));
        END_EX_GUARD;
    }

    int Person::Age::get(void)
    {
        BEGIN_EX_GUARD;
        return (_impl->getAge());
        END_EX_GUARD;
    }

    DotNetLib::Address^ Person::Address::get(void)
    {
        BEGIN_EX_GUARD;
        auto implAddr = _impl->getAddress();
        DotNetLib::Address^ result = gcnew DotNetLib::Address(
            implAddr.getStreetNum(),
            DotNetLibCore::Utils::CXXStdStringToSystemString(implAddr.getStreet()),
            DotNetLibCore::Utils::CXXStdStringToSystemString(implAddr.getCity()),
            DotNetLibCore::Utils::CXXStdStringToSystemString(implAddr.getProvince()),
            DotNetLibCore::Utils::CXXStdStringToSystemString(implAddr.getCountry()),
            DotNetLibCore::Utils::CXXStdStringToSystemString(implAddr.getZipCode())
        );
        return (result);
        END_EX_GUARD;
    }

    System::String^ Person::ToString(void)
    {
        System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
        sb->Append(this->LastName);
        sb->Append(L", ");
        sb->Append(this->FirstName);
        sb->Append(System::Environment::NewLine);
        sb->Append(this->Age);
        sb->Append(L" years old");
        sb->Append(System::Environment::NewLine);
        sb->Append(this->Address->ToString());
        return (sb->ToString());
    }

    void Person::CleanUp(void)
    {
        if (_impl != nullptr) {
            delete (_impl);
            _impl = nullptr;
        }
        return;
    }

    Printer::Printer(IGenerator^ generator) :
        _impl(nullptr)
    {
        BEGIN_EX_GUARD;
        _impl = new DotNetLibCore::CXXLibPrinterAPIWorkAround(
            std::unique_ptr<CXXLib::GeneratorBase>(std::move(new DotNetLibCore::DotNetLibGeneratorImpl(generator)))
        );
        END_EX_GUARD;
        return;
    }

    Printer::!Printer(void)
    {
        this->CleanUp();
        return;
    }

    Printer::~Printer(void)
    {
        this->CleanUp();
        return;
    }

    void Printer::PrintInt(void)
    {
        BEGIN_EX_GUARD;
        _impl->printInt();
        END_EX_GUARD;
        return;
    }

    void Printer::PrintString(void)
    {
        BEGIN_EX_GUARD;
        _impl->printString();
        END_EX_GUARD;
        return;
    }

    void Printer::CleanUp(void)
    {
        if (_impl != nullptr) {
            delete (_impl);
            _impl = nullptr;
        }
        return;
    }

} // namespace DotNetLib
