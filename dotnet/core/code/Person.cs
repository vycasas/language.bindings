using System;
using System.Diagnostics;

namespace DotSlashZero.DotNetCoreLib
{
    public sealed class Person
    {
        private IntPtr m_pImpl = IntPtr.Zero;

        public Person(
            string lastName,
            string firstName,
            int age,
            Address address)
        {
            m_pImpl = CLib.PersonCreate(
                lastName,
                firstName,
                age,
                address.GetImpl());
        }

        ~Person()
        {
            try
            {
                if (m_pImpl != IntPtr.Zero)
                {
                    CLib.PersonDestroy(m_pImpl);
                    m_pImpl = IntPtr.Zero;
                }
            }
            catch { /* no throw */ }
        }

        public string LastName
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var lastName = CLib.PersonGetLastName(m_pImpl);
                return (lastName);
            }
        }

        public string FirstName
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var firstName = CLib.PersonGetFirstName(m_pImpl);
                return (firstName);
            }
        }

        public int Age
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var age = CLib.PersonGetAge(m_pImpl);
                return (age);
            }
        }

        public Address Address
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var addressImpl = CLib.PersonGetAddress(m_pImpl);
                var address = new Address(addressImpl);
                return (address);
            }
        }

        public override string ToString()
        {
            Debug.Assert(m_pImpl != IntPtr.Zero);
            var personString = CLib.PersonToString(m_pImpl);
            return (personString);
        }
    }
}
