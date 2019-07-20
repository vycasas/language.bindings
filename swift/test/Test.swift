import Darwin
import SwiftLib

fileprivate final class MyGenerator : Generator {
    public func generateInt(data: Int) -> Int {
        return data * data
    }
    public func generateString(data: Int) -> String {
        return "\(data)"
    }
}

fileprivate func main() -> Int32 {
    do {
        try Library.initialize()
        let versionString = try Library.getVersionString()
        print("Library initialized... version \(versionString)")

        print("Creating a new Address...")
        let address = try Address(
            streetNum: 9898,
            street: "Corner St.",
            city: "Gotham",
            province: "CA",
            zipCode: "4321",
            country: "Antartica")
        print("New address created!")
        print("Address:")
        print("\(address)")

        print("Creating a new Person...")
        let person = try Person(
            lastName: "Wayne",
            firstName: "Bruce",
            age: 25,
            address: address)
        print("New person created!")
        print("Person:")
        print("\(person)")

        print("Creating a new generator...")
        let myGenerator = MyGenerator()
        print("New generator created!")
        print("Creating a new printer...")
        let printer = try Printer(generator: myGenerator)
        print("New printer created!")
        try printer.printInt()
        try printer.printString()
    }
    catch {
        print("Error: \(error)")
    }
    return 0
}

let exitCode = main()
exit(exitCode)
