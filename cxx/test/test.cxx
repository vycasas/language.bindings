#include <cxxlib/cxxlib.hxx>

#include <iostream>
#include <memory>
#include <sstream>

class MyGenerator : public CXXLib::GeneratorBase
{
public:
    MyGenerator(void)
    { return; }

    virtual ~MyGenerator(void)
    { return; }

    virtual int generateInt(int data) const
    {
        return (data * data);
    }

    virtual std::string generateString(int data) const
    {
        std::stringstream ss;
        ss << data;
        return (ss.str());
    }
};

int main(void)
{
    try {
        CXXLib::Library::initialize();
    
        std::cout << "Library initialized... version " << CXXLib::Library::getVersionString() << std::endl;

        std::cout << "Creating a new address..." << std::endl;
        CXXLib::Address address{ 9898, "Corner St.", "Gotham", "CA", "Antartica", "4132" };
        std::cout << "New address created!" << std::endl;
        std::cout << "Address:" << std::endl;
        std::cout << address.toString() << std::endl;

        std::cout << "Creating a new person..." << std::endl;
        CXXLib::Person person{ "Wayne", "Bruce", 25, address };
        std::cout << "New person created!" << std::endl;
        std::cout << person.toString() << std::endl;

        std::cout << "Creating a new generator..." << std::endl;
        std::unique_ptr<MyGenerator> generator(new MyGenerator);
        std::cout << "New generator created!" << std::endl;
        std::cout << "Creating a new printer..." << std::endl;
        CXXLib::Printer printer(std::move(generator));
        std::cout << "New printer created!" << std::endl;
        std::cout << "Performing printer actions..." << std::endl;
        printer.printInt();
        printer.printString();

        CXXLib::Library::terminate();
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
