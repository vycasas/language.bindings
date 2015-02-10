using System;

using DotNetLib;

namespace test_dotnet_cs
{
    class MyGenerator : IGenerator
    {
        public int GenerateInt(int data)
        {
            return (data * data);
        }

        public String GenerateString(int data)
        {
            return (String.Format("{0}", data));
        }
    }

    class Program
    {
        static int Main(string[] args)
        {
            try {
                Library.Initialize();

                Console.Out.WriteLine("Library initialized... version {0}", Library.GetVersionString());

                Console.Out.WriteLine("Creating a new address...");
                Address address = new Address(
                    9898, "Corner St.",
                    "Gotham City", "CA",
                    "Antartica", "6758"
                );
                Console.Out.WriteLine("New address created!");
                Console.Out.WriteLine("Address:");
                Console.Out.WriteLine(address.ToString());

                Console.Out.WriteLine("Creating a new person...");
                Person person = new Person(
                    "Wyane", "Bruce", 25, address
                );
                Console.Out.WriteLine("New person created!");
                Console.Out.WriteLine("Person:");
                Console.Out.WriteLine(person.ToString());

                Console.Out.WriteLine("Creating a new generator...");
                MyGenerator generator = new MyGenerator();
                Console.Out.WriteLine("New generator created!");
                Console.Out.WriteLine("Creating a new printer...");
                Printer printer = new Printer(generator);
                Console.Out.WriteLine("New printer created!");
                Console.Out.WriteLine("Performing printer actions...");
                printer.PrintInt();
                printer.PrintString();

                Library.Terminate();
            }
            catch (DotNetLib.Exception e) {
                Console.Error.WriteLine("An error has occurred: {0}", e.Message);
                return (-1);
            }
            catch (System.Exception e) {
                Console.Error.WriteLine("An error has occurred: {0}", e.Message);
                return (-1);
            }
            return (0);
        }
    }
}
