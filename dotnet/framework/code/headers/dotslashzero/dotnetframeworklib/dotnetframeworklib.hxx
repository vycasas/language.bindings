#pragma once

#include <exception>
#include <memory>

namespace DotSlashZero::CxxLib
{
    class Exception;
    class Address;
    class Person;
    class Printer;
}
// namespace DotSlashZero::CxxLib

namespace DotSlashZero::DotNetFrameworkLib
{
    public ref class Exception sealed : public System::Exception
    {
    public:
        ~Exception();
        
        !Exception();

    internal:
        Exception(System::String^ message);
    };
    // class Exception

    public ref class Library abstract sealed
    {
    public:
        static System::Boolean Initialize();
        static System::Void Uninitialize();

        static property System::String^ VersionString { System::String^ get();  }
        static property System::Int32 VersionMajor { System::Int32 get(); }
        static property System::Int32 VersionMinor { System::Int32 get(); }
        static property System::Int32 VersionPatch { System::Int32 get(); }
        static property System::String^ VersionExtra { System::String^ get();  }
    };
    // class Library

    public ref class Address sealed
    {
    public:
        Address(
            System::Int32 streetNum,
            System::String^ street,
            System::String^ city,
            System::String^ province,
            System::String^ zipCode,
            System::String^ country);

        ~Address();

        !Address();
        
        property System::Int32 StreetNum
        { System::Int32 get(); }
        
        property System::String^ Street
        { System::String^ get(); }
        
        property System::String^ City
        { System::String^ get(); }
        
        property System::String^ Province
        { System::String^ get(); }

        property System::String^ ZipCode
        { System::String^ get(); }
        
        property System::String^ Country
        { System::String^ get(); }
        
        System::String^ ToString() override;

    internal:
        CxxLib::Address* GetImpl(); // this is dangerous! it will be nice if managed classes can support C++ smart pointer members and use std::weak_ptr

    private:
        CxxLib::Address* m_pImpl = nullptr;
    };
    // class Address

    public ref class Person sealed
    {
    public:
        Person(
            System::String^ lastName,
            System::String^ firstName,
            System::Int32 age,
            DotNetFrameworkLib::Address^ address);

        ~Person();

        !Person();

        property System::String^ LastName
        { System::String^ get(); }

        property System::String^ FirstName
        { System::String^ get(); }

        property System::Int32 Age
        { System::Int32 get(); }

        property DotNetFrameworkLib::Address^ Address
        { DotNetFrameworkLib::Address^ get(); }

        System::String^ ToString() override;

    private:
        CxxLib::Person* m_pImpl = nullptr;
    };
    // class Person

    
    public interface class IGenerator
    {
    public:
        System::Int32 GenerateInt(System::Int32 data);
        System::String^ GenerateString(System::Int32 data);
    };
    // interface class IGenerator

    public ref class Printer sealed
    {
    public:
        Printer(IGenerator^ generator);

        !Printer();
        
        ~Printer();

        System::Void PrintInt();

        System::Void PrintString();

    private:
        CxxLib::Printer* m_pImpl = nullptr;
    };
    // class Printer
    
}
// namespace DotSlashZero::DotNetFrameworkLib
