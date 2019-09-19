using System;
using System.Diagnostics;

namespace DotSlashZero.DotNetCoreLib
{
    public sealed class Address
    {
        private IntPtr m_pImpl = IntPtr.Zero;

        public Address(
            int streetNum,
            string street,
            string city,
            string province,
            string zipCode,
            string country)
        {
            m_pImpl = CLib.AddressCreate(
                streetNum,
                street,
                city,
                province,
                zipCode,
                country);
        }

        internal Address(IntPtr pImpl)
        {
            m_pImpl = pImpl;
        }

        ~Address()
        {
            try
            {
                if (m_pImpl != IntPtr.Zero)
                {
                    CLib.AddressDestroy(m_pImpl);
                    m_pImpl = IntPtr.Zero;
                }
            }
            catch { /* no throw */ }
        }

        public int StreetNum
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var streetNum = CLib.AddressGetStreetNum(m_pImpl);
                return (streetNum);
            }
        }

        public string Street
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var street = CLib.AddressGetStreet(m_pImpl);
                return (street);
            }
        }

        public string City
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var city = CLib.AddressGetCity(m_pImpl);
                return (city);
            }
        }

        public string Province
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var province = CLib.AddressGetProvince(m_pImpl);
                return (province);
            }
        }

        public string ZipCode
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var zipCode = CLib.AddressGetZipCode(m_pImpl);
                return (zipCode);
            }
        }

        public string Country
        {
            get
            {
                Debug.Assert(m_pImpl != IntPtr.Zero);
                var country = CLib.AddressGetCountry(m_pImpl);
                return (country);
            }
        }

        public override string ToString()
        {
            Debug.Assert(m_pImpl != IntPtr.Zero);
            var addressString = CLib.AddressToString(m_pImpl);
            return (addressString);
        }

        internal IntPtr GetImpl()
        {
            return (m_pImpl);
        }
    }
}
