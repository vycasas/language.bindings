using System;
using System.Threading.Tasks;
using DotSlashZero;
using DotSlashZero.DotNetFrameworkLib;

namespace DotSlashZero.DotNetFrameworkLib.Test
{
    internal static class Program
    {
        internal sealed class MyGenerator : IGenerator
        {
            public MyGenerator()
            {
                return;
            }

            public int GenerateInt(int data)
            {
                return (data * data);
            }

            public string GenerateString(int data)
            {
                return ($"{data}");
            }
        }

        public static async Task<int> Main()
        {
            try
            {
                var initOk = Library.Initialize();
                if (!initOk)
                {
                    await Console.Error.WriteLineAsync("Failed to initialize library");
                    return (1);
                }

                await Console.Out.WriteLineAsync($"Library initialized... version {Library.VersionString}");

                await Console.Out.WriteLineAsync("Creating a new address...");

                var address = new Address(
                    9898,
                    "Corner St.",
                    "Gotham",
                    "CA",
                    "4132",
                    "Antartica");

                await Console.Out.WriteLineAsync("New address created!");
                await Console.Out.WriteLineAsync("Address:");

                await Console.Out.WriteLineAsync($"{address}");

                await Console.Out.WriteLineAsync("Creating a new person...");

                var person = new Person(
                    "Wayne",
                    "Bruce",
                    25,
                    address);

                await Console.Out.WriteLineAsync("New person created!");

                await Console.Out.WriteLineAsync("Person:");
                await Console.Out.WriteLineAsync($"{person}");

                await Console.Out.WriteLineAsync("Creating a new generator...");
                var generator = new MyGenerator();
                await Console.Out.WriteLineAsync("New generator created!");

                await Console.Out.WriteLineAsync("Creating a new printer...");
                var printer = new Printer(generator);
                await Console.Out.WriteLineAsync("New printer created!");

                await Console.Out.WriteLineAsync("Performing printer actions...");
                printer.PrintInt();
                printer.PrintString();

                return (0);
            }
            catch (DotNetFrameworkLib.Exception e)
            {
                await Console.Error.WriteLineAsync($"An error has occurred: {e}");
                return (-1);
            }
            catch (System.Exception e)
            {
                await Console.Error.WriteLineAsync($"An error has occurred: {e}");
                return (-1);
            }
            finally
            {
                Library.Uninitialize();
            }
        }
    }
}
