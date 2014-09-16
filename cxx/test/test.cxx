#include <iostream>

#include <cxx/api.hxx>

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
