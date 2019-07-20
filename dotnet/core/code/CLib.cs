using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace DotSlashZero.DotNetCoreLib
{
    using CSizeT = IntPtr;
    using CCharPtr = IntPtr;
    using CVoidPtr = IntPtr;
    using CSizeTPtr = IntPtr;

    using DszCLibErrorNum = IntPtr;
    using DszCLibAddress = IntPtr;
    using DszCLibPerson = IntPtr;
    using DszCLibGenerator = IntPtr;
    using DszCLibPrinter = IntPtr;

    internal static class CLib
    {
        private static readonly DszCLibErrorNum DSZ_CLIB_ERRORNUM_NO_ERROR = DszCLibErrorNum.Zero;
        private static readonly DszCLibAddress DSZ_CLIB_ADDRESS_INVALID = DszCLibAddress.Zero;
        private static readonly DszCLibPerson DSZ_CLIB_PERSON_INVALID = DszCLibAddress.Zero;
        private static readonly DszCLibGenerator DSZ_CLIB_GENERATOR_INVALID = DszCLibAddress.Zero;
        private static readonly DszCLibPrinter DSZ_CLIB_PRINTER_INVALID = DszCLibAddress.Zero;

        private static readonly DszCLibErrorNum DSZ_CLIB_ERRORNUM_CALLBACK_ERROR = new DszCLibErrorNum(-1);

        #region caller interface

        private static string ErrorNumGetMessage(DszCLibErrorNum errorNum) // this method is special and will only be used within this class
        {
            var message = string.Empty;

            GetStringHelperNoReturn<DszCLibErrorNum>(DszCLibErrorNumGetMessage, errorNum, ref message);

            return (message);
        }

        #region Library

        internal static bool LibraryInitialize()
        {
            var cLibErrorNum = DszCLibLibraryInitialize();
            return (cLibErrorNum == DSZ_CLIB_ERRORNUM_NO_ERROR);
        }

        internal static void LibraryUninitialize()
        {
            DszCLibLibraryUninitialize();
            return;
        }

        internal static string LibraryGetVersionString()
        {
            var versionString = string.Empty;

            var errorNum = GetStringHelperNoInput(DszCLibLibraryGetVersionString, ref versionString);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (versionString);
        }

        internal static int LibraryGetVersionMajor()
        {
            CSizeT versionMajor = CSizeT.Zero;

            var errorNum = DszCLibLibraryGetVersionMajor(ref versionMajor);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (versionMajor.ToInt32());
        }

        internal static int LibraryGetVersionMinor()
        {
            CSizeT versionMinor = CSizeT.Zero;

            var errorNum = DszCLibLibraryGetVersionMinor(ref versionMinor);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (versionMinor.ToInt32());
        }

        internal static int LibraryGetVersionPatch()
        {
            CSizeT versionPatch = CSizeT.Zero;

            var errorNum = DszCLibLibraryGetVersionPatch(ref versionPatch);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (versionPatch.ToInt32());
        }

        internal static string LibraryGetVersionExtra()
        {
            var versionExtra = string.Empty;

            var errorNum = GetStringHelperNoInput(DszCLibLibraryGetVersionExtra, ref versionExtra);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (versionExtra);
        }

        #endregion

        #region Address

        internal static DszCLibAddress AddressCreate(
            int streetNum,
            string street,
            string city,
            string province,
            string zipCode,
            string country)
        {
            var address = DSZ_CLIB_ADDRESS_INVALID;

            var errorNum = DszCLibAddressCreate(
                streetNum,
                street,
                city,
                province,
                zipCode,
                country,
                ref address);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (address);
        }

        internal static void AddressDestroy(DszCLibAddress address)
        {
            DszCLibAddressDestroy(address);
            return;
        }

        internal static int AddressGetStreetNum(DszCLibAddress address)
        {
            int streetNum = 0;

            var errorNum = DszCLibAddressGetStreetNum(
                address,
                ref streetNum);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (streetNum);
        }

        internal static string AddressGetStreet(DszCLibAddress address)
        {
            string street = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibAddressGetStreet, address, ref street);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (street);
        }

        internal static string AddressGetCity(DszCLibAddress address)
        {
            string city = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibAddressGetCity, address, ref city);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (city);
        }

        internal static string AddressGetProvince(DszCLibAddress address)
        {
            string province = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibAddressGetProvince, address, ref province);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (province);
        }

        internal static string AddressGetZipCode(DszCLibAddress address)
        {
            string zipCode = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibAddressGetZipCode, address, ref zipCode);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (zipCode);
        }

        internal static string AddressGetCountry(DszCLibAddress address)
        {
            string country = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibAddressGetCountry, address, ref country);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (country);
        }

        internal static string AddressToString(DszCLibAddress address)
        {
            var addressString = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibAddressToString, address, ref addressString);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (addressString);
        }

        #endregion

        #region Person

        internal static DszCLibPerson PersonCreate(
            string lastName,
            string firstName,
            int age,
            DszCLibAddress address)
        {
            var person = DSZ_CLIB_PERSON_INVALID;

            var errorNum = DszCLibPersonCreate(
                lastName,
                firstName,
                age,
                address,
                ref person);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (person);
        }

        internal static void PersonDestroy(DszCLibPerson person)
        {
            DszCLibPersonDestroy(person);
            return;
        }

        internal static string PersonGetLastName(DszCLibPerson person)
        {
            string lastName = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibPersonGetLastName, person, ref lastName);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (lastName);
        }

        internal static string PersonGetFirstName(DszCLibPerson person)
        {
            string firstName = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibPersonGetFirstName, person, ref firstName);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (firstName);
        }

        internal static int PersonGetAge(DszCLibPerson person)
        {
            int age = 0;

            var errorNum = DszCLibPersonGetAge(
                person,
                ref age);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (age);
        }

        internal static DszCLibAddress PersonGetAddress(DszCLibPerson person)
        {
            DszCLibAddress address = DSZ_CLIB_ADDRESS_INVALID;

            var errorNum = DszCLibPersonGetAddress(
                person,
                ref address);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (address);
        }

        internal static string PersonToString(DszCLibPerson person)
        {
            string personString = string.Empty;

            var errorNum = GetStringHelper<DszCLibAddress>(DszCLibPersonToString, person, ref personString);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (personString);
        }

        #endregion

        #region Generator

        // a class that implements the generator delegates to be passed to native code
        // and provides an abstraction for the API IGenerator interface
        internal sealed class GeneratorWrapper
        {
            public delegate int GenerateInt(int data);

            public delegate string GenerateString(int data);

            private GenerateInt m_generateInt;

            private GenerateString m_generateString;

            public GeneratorWrapper(GenerateInt generateInt, GenerateString generateString)
            {
                m_generateInt = generateInt;
                m_generateString = generateString;
            }

            public DszCLibErrorNum GenerateIntWrapped(
                int data,
                ref int pInt,
                CVoidPtr _)
            {
                var generatedInt = m_generateInt(data);
                pInt = generatedInt;

                return (DSZ_CLIB_ERRORNUM_NO_ERROR);
            }

            public DszCLibErrorNum GenerateStringWrapped(
                int data,
                CCharPtr pString, CSizeT stringSize,
                CSizeTPtr pCharsWritten,
                CVoidPtr _)
            {
                // note: pString is unmanaged memory created by native code

                var generatedStr = m_generateString(data);
                var generatedStrUtf8Bytes = Encoding.UTF8.GetBytes(generatedStr + '\0');

                var numBytesToCopy = generatedStrUtf8Bytes.Length;

                if ((pString != CCharPtr.Zero) && (stringSize.ToInt32() > 0))
                {
                    numBytesToCopy = Math.Min(
                        numBytesToCopy,
                        stringSize.ToInt32());

                    Marshal.Copy(
                        generatedStrUtf8Bytes,
                        0,
                        pString,
                        numBytesToCopy);
                }

                if (pCharsWritten != CSizeTPtr.Zero)
                {
                    // pCharsWritten is a pointer to size_t under the hood
                    // it is necessary to do a memory write to this location
                    var numBytesToCopyByteBuffer = BitConverter.GetBytes(numBytesToCopy);

                    // note: this may not be portable as the endian mode may affect the value the native code will receive
                    Marshal.Copy(
                        numBytesToCopyByteBuffer,
                        0,
                        pCharsWritten,
                        numBytesToCopyByteBuffer.Length);
                }

                return (DSZ_CLIB_ERRORNUM_NO_ERROR);
            }
        }

        internal static DszCLibGenerator GeneratorCreate(GeneratorWrapper generatorWrapper)
        {
            var generator = DSZ_CLIB_GENERATOR_INVALID;

            var errorNum = DszCLibGeneratorCreate(
                generatorWrapper.GenerateIntWrapped,
                generatorWrapper.GenerateStringWrapped,
                ref generator);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (generator);
        }

        internal static void GeneratorDestroy(DszCLibGenerator generator)
        {
            DszCLibGeneratorDestroy(generator);
            return;
        }

        #endregion

        #region Printer

        internal static DszCLibPrinter PrinterCreate(DszCLibGenerator generator)
        {
            var printer = DSZ_CLIB_PRINTER_INVALID;

            var errorNum = DszCLibPrinterCreate(
                generator,
                ref printer);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return (printer);
        }

        internal static void PrinterDestroy(DszCLibPrinter printer)
        {
            DszCLibPrinterDestroy(printer);
            return;
        }

        internal static void PrinterPrintInt(DszCLibPrinter printer)
        {
            var errorNum = DszCLibPrinterPrintInt(printer);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return;
        }

        internal static void PrinterPrintString(DszCLibPrinter printer)
        {
            var errorNum = DszCLibPrinterPrintString(printer);

            if (errorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                throw (new Exception(ErrorNumGetMessage(errorNum)));

            return;
        }

        #endregion

        #endregion

        #region direct P/Invoke mapping

        // direct P/Invoke mapping should never be called outside of this file
        // interfaces are provided for proper handling of data moving across ABIs

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern void DszCLibErrorNumGetMessage(
            DszCLibErrorNum errorNum,
            CCharPtr pMessage, CSizeT messageSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibLibraryInitialize();

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern void DszCLibLibraryUninitialize();

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibLibraryGetVersionString(
            CCharPtr pVersionString, CSizeT versionStringSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibLibraryGetVersionMajor(
            ref CSizeT pVersionMajor);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibLibraryGetVersionMinor(
            ref CSizeT pVersionMinor);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibLibraryGetVersionPatch(
            ref CSizeT pVersionPatch);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibLibraryGetVersionExtra(
            CCharPtr pVersionString, CSizeT versionStringSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern DszCLibErrorNum DszCLibAddressCreate(
            int streetNum,
            string street,
            string city,
            string province,
            string zipCode,
            string country,
            ref DszCLibAddress pAddress);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressDestroy(
            DszCLibAddress address);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressGetStreetNum(
            DszCLibAddress address,
            ref int pStreetNum);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressGetStreet(
            DszCLibAddress address,
            CCharPtr pStreet, CSizeT streetSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressGetCity(
            DszCLibAddress address,
            CCharPtr pCity, CSizeT citySize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressGetProvince(
            DszCLibAddress address,
            CCharPtr pProvince, CSizeT provinceSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressGetZipCode(
            DszCLibAddress address,
            CCharPtr pZipCode, CSizeT zipCodeSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressGetCountry(
            DszCLibAddress address,
            CCharPtr pCountry, CSizeT countrySize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibAddressToString(
            DszCLibAddress address,
            CCharPtr pAddressString, CSizeT addressStringSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern DszCLibErrorNum DszCLibPersonCreate(
            string lastName,
            string firstName,
            int age,
            DszCLibAddress address,
            ref DszCLibPerson pPerson);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPersonDestroy(
            DszCLibPerson person);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPersonGetLastName(
            DszCLibPerson person,
            CCharPtr lastName, CSizeT lastNameSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPersonGetFirstName(
            DszCLibPerson person,
            CCharPtr firstName, CSizeT firstNameSize,
            ref CSizeT pCharsWritten);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPersonGetAge(
            DszCLibPerson person,
            ref int pAge);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPersonGetAddress(
            DszCLibPerson person,
            ref DszCLibAddress pAddress);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPersonToString(
            DszCLibPerson person,
            CCharPtr personString, CSizeT personStringSize,
            ref CSizeT pCharsWritten);

        private delegate DszCLibErrorNum DszCLibGenerateIntFunction(
            int data,
            ref int pInt,
            CVoidPtr pUserData);

        private delegate DszCLibErrorNum DszCLibGenerateStringFunction(
            int data,
            CCharPtr pString, CSizeT stringSize,
            CSizeTPtr pCharsWritten, // we cannot use a "ref" parameter here because this will be invoked from the native code
            CVoidPtr pUserData);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibGeneratorCreate(
            DszCLibGenerateIntFunction fnGenerateInt,
            DszCLibGenerateStringFunction fnGenerateString,
            ref DszCLibGenerator pGenerator);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibGeneratorDestroy(
            DszCLibGenerator generator);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPrinterCreate(
            DszCLibGenerator generator,
            ref DszCLibPrinter pPrinter);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPrinterDestroy(
            DszCLibPrinter printer);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPrinterPrintInt(
            DszCLibPrinter printer);

        [DllImport("clib", CallingConvention = CallingConvention.StdCall)]
        private static extern DszCLibErrorNum DszCLibPrinterPrintString(
            DszCLibPrinter printer);

        #endregion

        #region helper methods to reduce boiler plate

        private delegate DszCLibErrorNum CLibStringOutDelegate<T>(T impl, CCharPtr pString, CSizeT stringSize, ref CSizeT pCharsWritten);

        private delegate void CLibStringOutDelegateNoReturn<T>(T impl, CCharPtr pString, CSizeT stringSize, ref CSizeT pCharsWritten);

        private delegate DszCLibErrorNum CLibStringOutDelegateNoInput(CCharPtr pString, CSizeT stringSize, ref CSizeT pCharsWritten);

        private delegate void CLibStringOutDelegateNoInputNoReturn(CCharPtr pString, CSizeT stringSize, ref CSizeT pCharsWritten);

        // a class to help allocate native memory
        private sealed class UnmanagedBuffer : IDisposable
        {
            private const int DEFAULT_SIZE_TO_ALLOCATE = 128;

            private int m_size = DEFAULT_SIZE_TO_ALLOCATE;

            private IntPtr m_pUnmanagedBuffer = IntPtr.Zero;

            public UnmanagedBuffer(int sizeToAllocate = DEFAULT_SIZE_TO_ALLOCATE)
            {
                m_size = sizeToAllocate;
                m_pUnmanagedBuffer = Marshal.AllocHGlobal(m_size);
                if (m_pUnmanagedBuffer == IntPtr.Zero)
                {
                    throw (new Exception("An error has occurred during P/Invoke: Failed to allocate native memory."));
                }
            }

            ~UnmanagedBuffer()
            {
                try
                {
                    Dispose();
                }
                catch { /* no throw */ }
            }

            public void Dispose()
            {
                if (m_pUnmanagedBuffer != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(m_pUnmanagedBuffer);
                    m_pUnmanagedBuffer = IntPtr.Zero;
                }
                return;
            }

            public IntPtr Address { get { return (m_pUnmanagedBuffer); } }

            public int Size { get { return (m_size); } }

            public IntPtr SizeAsIntPtr {  get { return ((IntPtr) m_size); } }

            public byte[] MakeByteArrayCopy()
            {
                Debug.Assert(m_pUnmanagedBuffer != IntPtr.Zero);

                var byteArray = new byte[m_size];
                Array.Clear(byteArray, 0, m_size);

                Marshal.Copy(m_pUnmanagedBuffer, byteArray, 0, m_size);

                return (byteArray);
            }

            public string ReadAsUtf8CString()
            {
                var utf8Bytes = MakeByteArrayCopy();
                // it is important remove the null terminator for the C-style string
                var result = Encoding.UTF8.GetString(utf8Bytes, 0, Array.IndexOf(utf8Bytes, (byte) 0));
                return (result);
            }
        }

        private static DszCLibErrorNum GetStringHelper<T>(CLibStringOutDelegate<T> cLibFunctionCall, T impl, ref string resultStr)
        {
            using (var unmanagedBuffer = new UnmanagedBuffer())
            {
                CSizeT pCharsWritten = CSizeT.Zero;

                var cLibErrorNum = cLibFunctionCall(impl, unmanagedBuffer.Address, unmanagedBuffer.SizeAsIntPtr, ref pCharsWritten);

                if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                    return (cLibErrorNum);

                resultStr = unmanagedBuffer.ReadAsUtf8CString();

                return (cLibErrorNum);
            }
        }

        private static void GetStringHelperNoReturn<T>(CLibStringOutDelegateNoReturn<T> cLibFunctionCall, T impl, ref string resultStr)
        {
            using (var unmanagedBuffer = new UnmanagedBuffer())
            {
                CSizeT pCharsWritten = CSizeT.Zero;

                cLibFunctionCall(impl, unmanagedBuffer.Address, unmanagedBuffer.SizeAsIntPtr, ref pCharsWritten);

                resultStr = unmanagedBuffer.ReadAsUtf8CString();
            }

            return;
        }

        private static DszCLibErrorNum GetStringHelperNoInput(CLibStringOutDelegateNoInput cLibFunctionCall, ref string resultStr)
        {
            using (var unmanagedBuffer = new UnmanagedBuffer())
            {
                CSizeT pCharsWritten = CSizeT.Zero;

                var cLibErrorNum = cLibFunctionCall(unmanagedBuffer.Address, unmanagedBuffer.SizeAsIntPtr, ref pCharsWritten);

                if (cLibErrorNum != DSZ_CLIB_ERRORNUM_NO_ERROR)
                    return (cLibErrorNum);

                resultStr = unmanagedBuffer.ReadAsUtf8CString();

                return (cLibErrorNum);
            }
        }

        private static void GetStringHelperNoInputNoReturn(CLibStringOutDelegateNoInputNoReturn cLibFunctionCall, ref string resultStr)
        {
            using (var unmanagedBuffer = new UnmanagedBuffer())
            {
                CSizeT pCharsWritten = CSizeT.Zero;

                cLibFunctionCall(unmanagedBuffer.Address, unmanagedBuffer.SizeAsIntPtr, ref pCharsWritten);

                resultStr = unmanagedBuffer.ReadAsUtf8CString();
            }
        }

        #endregion
    }
}
