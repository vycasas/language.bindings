fileprivate func callCLibApiFunctionWithStringReturn(
    _ cLibApiFunction: (
        _ pString: UnsafeMutablePointer<CChar>?,
        _ stringSize: Int,
        _ pCharsWritten: UnsafeMutablePointer<Int>?) -> DszCLibErrorNum,
    _ stringResult: inout String) -> DszCLibErrorNum
{
    let stringSize: Int = 128
    let pString = UnsafeMutablePointer<CChar>.allocate(capacity: stringSize)
    pString.assign(repeating: ("\0".utf8CString[0]), count: stringSize)

    let cLibErrorNum = cLibApiFunction(
        pString,
        stringSize,
        nil)

    stringResult = String(cString: pString)

    pString.deallocate()

    return cLibErrorNum
}

public struct LibraryError : Error {
    private(set) var message: String

    internal init(errorNum: Int) {
        self.message = ""

        _ = callCLibApiFunctionWithStringReturn({
            (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                DszCLibErrorNumGetMessage(errorNum, pString, stringSize, pCharsWritten)
                return (0)
        },
        &self.message)
    }

    internal init(message: String) {
        self.message = message
    }
}

extension LibraryError : CustomStringConvertible {
    public var description: String {
        return message
    }
}

fileprivate func callCLibApiAndCheckErrorNum(_ cLibApiFunction: () -> DszCLibErrorNum) throws {
    let cLibErrorNum = cLibApiFunction()

    if (cLibErrorNum != 0) {
        // swift doesn't unwind stack during exception throwing so no information is lost
        throw (LibraryError(errorNum: cLibErrorNum))
    }
}

public struct Library {
    private init() {
    }

    public static func initialize() throws {
        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in DszCLibLibraryInitialize()
        })
    }

    public static func uninitialize() {
        DszCLibLibraryUninitialize()
    }

    public static func getVersionString() throws -> String {
        var versionString = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibLibraryGetVersionString(pString, stringSize, pCharsWritten)
                }, &versionString)
        })

        return versionString
    }

    public static var versionString: String {
        return (try? getVersionString()) ?? String()
    }
}

public final class Address {
    // https://stackoverflow.com/a/44894681
    private var pImpl: UnsafeMutablePointer<DszCLibAddress?>?

    public init(
        streetNum: Int,
        street: String,
        city: String,
        province: String,
        zipCode: String,
        country: String) throws {

        self.pImpl = UnsafeMutablePointer<DszCLibAddress?>.allocate(capacity: 1)

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibAddressCreate(
                    CInt(truncatingIfNeeded: streetNum),
                    street,
                    city,
                    province,
                    zipCode,
                    country,
                    self.pImpl) // ?? how does this work?
        })
    }

    internal init(addressImpl: DszCLibAddress?) {
        self.pImpl = UnsafeMutablePointer<DszCLibAddress?>.allocate(capacity: 1)
        self.pImpl!.pointee = addressImpl
    }

    deinit {
        if (self.pImpl != nil) {
            let pAddressImpl = self.pImpl!
            DszCLibAddressDestroy(pAddressImpl.pointee)
            pAddressImpl.deallocate()
            self.pImpl = nil
        }
    }

    internal func getAddressImpl() throws -> DszCLibAddress? {
        assert(self.pImpl != nil)
        let pAddressImpl = self.pImpl!
        return pAddressImpl.pointee
    }

    public func getStreetNum() throws -> Int {
        let addressImpl = try getAddressImpl()
        let pStreetNum = UnsafeMutablePointer<CInt>.allocate(capacity: 1)

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibAddressGetStreetNum(addressImpl, pStreetNum)
        })

        let streetNum = pStreetNum.pointee
        pStreetNum.deallocate()
        return Int(streetNum)
    }

    public func getStreet() throws -> String {
        let addressImpl = try getAddressImpl()
        var street = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibAddressGetStreet(addressImpl, pString, stringSize, pCharsWritten)
                }, &street)
        })

        return street
    }

    public func getCity() throws -> String {
        let addressImpl = try getAddressImpl()
        var city = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibAddressGetCity(addressImpl, pString, stringSize, pCharsWritten)
                }, &city)
        })

        return city
    }

    public func getProvince() throws -> String {
        let addressImpl = try getAddressImpl()
        var province = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibAddressGetProvince(addressImpl, pString, stringSize, pCharsWritten)
                }, &province)
        })

        return province
    }

    public func getZipCode() throws -> String {
        let addressImpl = try getAddressImpl()
        var zipCode = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibAddressGetZipCode(addressImpl, pString, stringSize, pCharsWritten)
                }, &zipCode)
        })

        return zipCode
    }

    public func getCountry() throws -> String {
        let addressImpl = try getAddressImpl()
        var country = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibAddressGetCountry(addressImpl, pString, stringSize, pCharsWritten)
                }, &country)
        })

        return country
    }

    public var streetNum: Int {
        return (try? getStreetNum()) ?? Int()
    }

    public var street: String {
        return (try? getStreet()) ?? String()
    }

    public var city: String {
        return (try? getCity()) ?? String()
    }

    public var province: String {
        return (try? getProvince()) ?? String()
    }

    public var zipCode: String {
        return (try? getZipCode()) ?? String()
    }

    public var country: String {
        return (try? getCountry()) ?? String()
    }

    public func toString() throws -> String {
        let addressImpl = try getAddressImpl()
        var addressString = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibAddressToString(addressImpl, pString, stringSize, pCharsWritten)
                }, &addressString)
        })

        return addressString
    }
}

extension Address : CustomStringConvertible {
    public var description: String {
        return (try? toString()) ?? String()
    }
}

public final class Person {
    private var pImpl: UnsafeMutablePointer<DszCLibPerson?>?

    public init(
        lastName: String,
        firstName: String,
        age: Int,
        address: Address) throws {

        self.pImpl = UnsafeMutablePointer<DszCLibPerson?>.allocate(capacity: 1)

        let addressImpl = try address.getAddressImpl()

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibPersonCreate(
                    lastName,
                    firstName,
                    CInt(truncatingIfNeeded: age),
                    addressImpl,
                    self.pImpl) // ?? how does this work?
        })
    }

    internal init(personImpl: DszCLibPerson?) {
        self.pImpl = UnsafeMutablePointer<DszCLibPerson?>.allocate(capacity: 1)
        self.pImpl!.pointee = personImpl
    }

    deinit {
        if (self.pImpl != nil) {
            let pPersonImpl = self.pImpl!
            DszCLibPersonDestroy(pPersonImpl.pointee)
            pPersonImpl.deallocate()
            self.pImpl = nil
        }
    }

    internal func getPersonImpl() throws -> DszCLibPerson? {
        assert(self.pImpl != nil)
        let pPersonImpl = self.pImpl!
        return pPersonImpl.pointee
    }

    public func getLastName() throws -> String {
        let personImpl = try getPersonImpl()
        var lastName = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibPersonGetLastName(personImpl, pString, stringSize, pCharsWritten)
                }, &lastName)
        })

        return lastName
    }

    public func getFirstName() throws -> String {
        let personImpl = try getPersonImpl()
        var firstName = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibPersonGetFirstName(personImpl, pString, stringSize, pCharsWritten)
                }, &firstName)
        })

        return firstName
    }

    public func getAge() throws -> Int {
        let personImpl = try getPersonImpl()
        let pAge = UnsafeMutablePointer<CInt>.allocate(capacity: 1)

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibPersonGetAge(personImpl, pAge)
        })

        let age = pAge.pointee
        pAge.deallocate()
        return Int(age)
    }

    public func getAddress() throws -> Address? {
        let personImpl = try getPersonImpl()
        let pAddressImpl = UnsafeMutablePointer<DszCLibAddress?>.allocate(capacity: 1)

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibPersonGetAddress(personImpl, pAddressImpl)
        })

        let addressImpl = pAddressImpl.pointee
        if (addressImpl == nil) {
            return nil
        }

        let address = Address(addressImpl: addressImpl)
        pAddressImpl.deallocate()
        return address
    }

    public var lastName: String {
        return (try? getLastName()) ?? String()
    }

    public var firstName: String {
        return (try? getFirstName()) ?? String()
    }

    public var age: Int {
        return (try? getAge()) ?? Int()
    }

    public var address: Address? {
        return try? getAddress()
    }

    public func toString() throws -> String {
        let personImpl = try getPersonImpl()
        var personString = String();

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                callCLibApiFunctionWithStringReturn({
                    (pString, stringSize, pCharsWritten) -> DszCLibErrorNum in
                        DszCLibPersonToString(personImpl, pString, stringSize, pCharsWritten)
                }, &personString)
        })

        return personString
    }
}

extension Person : CustomStringConvertible {
    public var description: String {
        return (try? toString()) ?? String()
    }
}

public protocol Generator {
    func generateInt(data: Int) -> Int
    func generateString(data: Int) -> String
}

fileprivate func generateIntRedirect(
    _ data: CInt,
    _ pInt: UnsafeMutablePointer<CInt>?,
    _ pUserData: UnsafeMutableRawPointer?) -> DszCLibErrorNum {

    if (pInt == nil) {
        return DszCLibErrorNum(-2)
    }

    if (pUserData == nil) {
        return DszCLibErrorNum(-2)
    }

    let generator = pUserData!.load(as: Generator.self)
    let generatedInt = generator.generateInt(data: Int(data))
    pInt!.pointee = CInt(truncatingIfNeeded: generatedInt)

    return DszCLibErrorNum(0)
}

fileprivate func generateStringRedirect(
    _ data: CInt,
    _ pString: UnsafeMutablePointer<CChar>?,
    _ stringSize: Int,
    _ pCharsWritten: UnsafeMutablePointer<Int>?,
    _ pUserData: UnsafeMutableRawPointer?) -> DszCLibErrorNum {

    if (pUserData == nil) {
        return DszCLibErrorNum(-2)
    }

    let generator = pUserData!.load(as: Generator.self)

    let generatedString = generator.generateString(data: Int(data))

    var numChars = generatedString.utf8.count;

    if ((pString != nil) && (stringSize > 0)) {
        numChars = numChars < stringSize ? numChars : stringSize
        let utf8Array = Array(generatedString.utf8)
        for i in 1...numChars {
            pString![i - 1] = CChar(utf8Array[i - 1])
        }
    }

    if (pCharsWritten != nil) {
        pCharsWritten!.pointee = numChars
    }

    return DszCLibErrorNum(0)
}

public final class Printer {
    private var pImpl: UnsafeMutablePointer<DszCLibPrinter?>?
    private var generator: Generator

    // needed to keep the closures alive as long as Printer is allive
    private var fnGenerateInt: DszCLibGenerateIntFunction?
    private var fnGenerateString: DszCLibGenerateStringFunction?

    public init(generator: Generator) throws {
        self.pImpl = UnsafeMutablePointer<DszCLibPerson?>.allocate(capacity: 1)
        self.generator = generator

        fnGenerateInt = {
            (data, pInt, pUserData) -> DszCLibErrorNum in
                generateIntRedirect(data, pInt, pUserData)
        }
        fnGenerateString = {
            (data, pString, stringSize, pCharsWritten, pUserData) -> DszCLibErrorNum in
                generateStringRedirect(data, pString, stringSize, pCharsWritten, pUserData)
        }

        let pGeneratorImpl = try createGeneratorImpl()
        if (pGeneratorImpl == nil) {
            throw LibraryError(message: "Failed to create generator")
        }

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibPrinterCreate(
                    pGeneratorImpl!.pointee,
                    self.pImpl)
        })

        pGeneratorImpl!.deallocate()
    }

    deinit {
        if (self.pImpl != nil) {
            let pPrinterImpl = self.pImpl!
            DszCLibPrinterDestroy(pPrinterImpl.pointee)
            pPrinterImpl.deallocate()
            self.pImpl = nil
        }
    }

    internal func getPrinterImpl() throws -> DszCLibPrinter? {
        assert(self.pImpl != nil)
        let pPrinterImpl = self.pImpl!
        return pPrinterImpl.pointee
    }

    private func createGeneratorImpl() throws -> UnsafeMutablePointer<DszCLibGenerator?>? {
        let pGeneratorImpl = UnsafeMutablePointer<DszCLibGenerator?>.allocate(capacity: 1)

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibGeneratorCreate(
                    fnGenerateInt,
                    fnGenerateString,
                    pGeneratorImpl)
        })

        return pGeneratorImpl
    }

    public func printInt() throws {
        let printerImpl = try getPrinterImpl()

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibPrinterPrintIntWithUserData(
                    printerImpl,
                    &self.generator)
        })
    }

    public func printString() throws {
        let printerImpl = try getPrinterImpl()

        try callCLibApiAndCheckErrorNum({
            () -> DszCLibErrorNum in
                DszCLibPrinterPrintStringWithUserData(
                    printerImpl,
                    &self.generator)
        })
    }
}
