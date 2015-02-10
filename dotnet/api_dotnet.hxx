// dotnetlib.h
#pragma once

#include <memory>

#include <cxx/api.hxx>

namespace DotNetLib
{
    public ref class Exception sealed : public System::Exception
    {
    public:
        Exception(void);

        !Exception(void);
        virtual ~Exception(void);

    internal:
        explicit Exception(const CXXLib::Exception& e);

    private:
        void CleanUp(void);

        CXXLib::Exception* _impl;
    }; // ref class Exception

	public ref class Library abstract sealed
	{
    public:
        static void Initialize(void);
        static void Terminate(void);
        static System::String^ GetVersionString(void);
		static System::UInt64 GetVersionMajor(void);
		static System::UInt64 GetVersionMinor(void);
	}; // ref class Library

    public ref class Address sealed
    {
    public:
        Address(
            int streetNum,
            System::String^ street,
            System::String^ city,
            System::String^ province,
            System::String^ country,
            System::String^ zipCode
        );

        !Address(void);
        virtual ~Address(void);

        property int StreetNum
        { int get(void); }

        property System::String^ Street
        { System::String^ get(void); }

        property System::String^ City
        { System::String^ get(void); }

        property System::String^ Province
        { System::String^ get(void); }

        property System::String^ Country
        { System::String^ get(void); }

        property System::String^ ZipCode
        { System::String^ get(void); }

        virtual System::String^ ToString(void) override;

    internal:
        CXXLib::Address* GetImpl(void)
        { return (_impl); }

    private:
        void CleanUp(void);

        CXXLib::Address* _impl;
    }; // ref class Address

    public ref class Person sealed
    {
    public:
        Person(System::String^ lastName, System::String^ firstName, int age, DotNetLib::Address^ address);

        !Person(void);
        virtual ~Person(void);

        property System::String^ LastName
        { System::String^ get(void); }

        property System::String^ FirstName
        { System::String^ get(void); }

        property int Age
        { int get(void); }

        property DotNetLib::Address^ Address
        { DotNetLib::Address^ get(void); }

        virtual System::String^ ToString(void) override;

    internal:
        CXXLib::Person* GetImpl(void)
        { return (_impl); }

    private:
        void CleanUp(void);

        CXXLib::Person* _impl;
    }; // ref class Person

    public interface class IGenerator
    {
    public:
        int GenerateInt(int data);
        System::String^ GenerateString(int data);
    }; // interface class IGenerator

    public ref class Printer
    {
    public:
        Printer(IGenerator^ generator);

        !Printer(void);
        virtual ~Printer(void);

        void PrintInt(void);
        void PrintString(void);

    internal:
        CXXLib::Printer* GetImpl(void)
        { return (_impl); }

    private:
        void CleanUp(void);

        CXXLib::Printer* _impl;
    }; // ref class Printer
} // namespace DotNetLib
