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
        print("Version: \(versionString)")

        let address = try Address(
            streetNum: 9898,
            street: "Corner St.",
            city: "Gotham",
            province: "CA",
            zipCode: "4321",
            country: "Antartica")
        print("Address:")
        print("\(address)")

        let person = try Person(
            lastName: "Wayne",
            firstName: "Bruce",
            age: 25,
            address: address)
        print("Person:")
        print("\(person)")

        let myGenerator = MyGenerator()
        let printer = try Printer(generator: myGenerator)
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
