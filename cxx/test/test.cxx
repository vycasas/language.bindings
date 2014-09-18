#include <iostream>
#include <memory>
#include <sstream>

#include <cxx/api.hxx>

class TestGenerator : public CXXLib::GeneratorBase
{
public:
    TestGenerator(void)
    { return; }

    virtual ~TestGenerator(void)
    { return; }

    virtual int generateInt(int data) const
    {
        std::cout << "TestGenerator::generateInt(int)" << std::endl;
        return (data * data);
    }

    virtual std::string generateString(int data) const
    {
        std::cout << "TestGenerator::generateString(int)" << std::endl;
        std::stringstream ss;
        ss << data;
        return (ss.str());
    }
};

int main(void)
{
    try {
        std::cout << "Creating a new address..." << std::endl;
        CXXLib::Address address{ 9898, "Corner St.", "Gotham", "CA", "Antartica", "4132" };
        std::cout << "New address created!" << std::endl;
        std::cout << "Address:" << std::endl;
        std::cout << address.toString() << std::endl;

        std::cout << "Creating a new person..." << std::endl;
        CXXLib::Person person{ "Wayne", "Bruce", 25, address };
        std::cout << "New person created!" << std::endl;
        std::cout << person.toString() << std::endl;

        std::unique_ptr<TestGenerator> generatorPtr(new TestGenerator);
        CXXLib::GeneratorFunctions::PrintIntUsingGenerator(std::move(generatorPtr));
        CXXLib::GeneratorFunctions::PrintStringUsingGenerator(
            std::unique_ptr<CXXLib::GeneratorBase>(new TestGenerator)
        );
    }
    catch (CXXLib::Exception& e) {
        std::cerr << "An error has occurred: " << e.getMessage() << std::endl;
        return (-1);
    }
    catch (std::exception& e) {
        std::cerr << "An error has occurred: " << e.what() << std::endl;
        return (-1);
    }

    return (0);
}
