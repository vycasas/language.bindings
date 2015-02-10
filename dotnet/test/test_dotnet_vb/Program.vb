Imports DotNetLib

Module Program

    Class MyGenerator : Implements IGenerator

        Public Function GenerateInt(data As Integer) As Integer Implements IGenerator.GenerateInt
            Return (data * data)
        End Function

        Public Function GenerateString(data As Integer) As String Implements IGenerator.GenerateString
            Return (String.Format("{0}", data))
        End Function
    End Class

    Function Main() As Integer
        Try
            Library.Initialize()

            Console.Out.WriteLine("Library initialized... version {0}", Library.GetVersionString())

            Console.Out.WriteLine("Creating a new address...")
            Dim address As Address = New Address(
                9898, "Corner St.",
                "Gotham City", "CA",
                "Antartica", "6758"
            )
            Console.Out.WriteLine("New address created!")
            Console.Out.WriteLine("Address:")
            Console.Out.WriteLine(address.ToString())

            Console.Out.WriteLine("Creating a new person...")
            Dim person As Person = New Person(
                "Wyane", "Bruce", 25, address
            )
            Console.Out.WriteLine("New person created!")
            Console.Out.WriteLine("Person:")
            Console.Out.WriteLine(Person.ToString())

            Console.Out.WriteLine("Creating a new generator...")
            Dim generator As MyGenerator = New MyGenerator()
            Console.Out.WriteLine("New generator created!")
            Console.Out.WriteLine("Creating a new printer...")
            Dim printer As Printer = New Printer(generator)
            Console.Out.WriteLine("New printer created!")
            Console.Out.WriteLine("Performing printer actions...")
            Printer.PrintInt()
            printer.PrintString()

            Library.Terminate()
        Catch e As DotNetLib.Exception
            Console.Error.WriteLine("An error has occurred: {0}", e.Message)
            Return (-1)
        Catch e As System.Exception
            Console.Error.WriteLine("An error has occurred: {0}", e.Message)
            Return (-1)
        End Try
        Return (0)
    End Function

End Module
