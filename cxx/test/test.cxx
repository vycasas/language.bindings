#include <dotslashzero/cxxlib/cxxlib.hxx>

#include <iostream>
#include <memory>
#include <sstream>

class MyGenerator final : public DotSlashZero::CxxLib::IGenerator
{
public:
    MyGenerator(void) = default;

    ~MyGenerator(void) noexcept = default;

    int GenerateInt(int data) const override
    {
        return (data * data);
    }

    std::string GenerateString(int data) const override
    {
        std::stringstream ss;
        ss << data;
        return (ss.str());
    }
};

int main(void)
{
    auto initOk = DotSlashZero::CxxLib::Library::Initialize();
    if (!initOk) {
        std::cerr << "Failed to initialize library" << std::endl;
        return (-1);
    }

    try {

        std::cout << "Library initialized... version " << DotSlashZero::CxxLib::Library::GetVersionString() << std::endl;

        std::cout << "Creating a new address..." << std::endl;
        DotSlashZero::CxxLib::Address address{ 9898, "Corner St.", "Gotham", "CA", "Antartica", "4132" };
        std::cout << "New address created!" << std::endl;
        std::cout << "Address:" << std::endl;
        std::cout << address.ToString() << std::endl;

        std::cout << "Creating a new person..." << std::endl;
        DotSlashZero::CxxLib::Person person{ "Wayne", "Bruce", 25, address };
        std::cout << "New person created!" << std::endl;
        std::cout << "Person:" << std::endl;
        std::cout << person.ToString() << std::endl;

        std::cout << "Creating a new generator..." << std::endl;
        auto pGenerator = std::make_unique<MyGenerator>();
        std::cout << "New generator created!" << std::endl;
        std::cout << "Creating a new printer..." << std::endl;
        DotSlashZero::CxxLib::Printer printer(pGenerator.release());
        std::cout << "New printer created!" << std::endl;
        std::cout << "Performing printer actions..." << std::endl;
        printer.PrintInt();
        printer.PrintString();
    }
    catch (DotSlashZero::CxxLib::Exception& e) {
        std::cerr << "An error has occurred: " << e.GetMessage() << std::endl;
        return (-1);
    }
    catch (std::exception& e) {
        std::cerr << "An error has occurred: " << e.what() << std::endl;
        return (-1);
    }

    DotSlashZero::CxxLib::Library::Uninitialize();

    return (0);
}
