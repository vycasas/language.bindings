package net.dotslashzero.javalib;

public class Address extends Core.ApiBase<Core.AddressType>
{
    Address()
    {
        m_impl = new Core.AddressType(0);
    }

    public Address(
        int streetNum,
        String street,
        String city,
        String province,
        String zipCode,
        String country) throws JavaLibException
    {
        m_impl = new Core.AddressType(0);

        long nativeErrorNum = nativeCreateAddress(
            streetNum,
            street,
            city,
            province,
            zipCode,
            country,
            m_impl);

        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));
    }

	@SuppressWarnings("deprecation")
    @Override
    protected synchronized void finalize()
    {
        nativeDestroyAddress(this.getImpl());
        return;
    }

    public int getStreetNum() throws JavaLibException
    {
        Core.WrappedInteger streetNum = new Core.WrappedInteger(0);

        long nativeErrorNum = nativeGetStreetNum(this.getImpl(), streetNum);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (streetNum.getInt());
    }

    public String getStreet() throws JavaLibException
    {
        Core.WrappedString street = new Core.WrappedString("");

        long nativeErrorNum = nativeGetStreet(this.getImpl(), street);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (street.getString());
    }

    public String getCity() throws JavaLibException
    {
        Core.WrappedString city = new Core.WrappedString("");

        long nativeErrorNum = nativeGetProvince(this.getImpl(), city);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (city.getString());
    }

    public String getProvince() throws JavaLibException
    {
        Core.WrappedString province = new Core.WrappedString("");

        long nativeErrorNum = nativeGetProvince(this.getImpl(), province);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (province.getString());
    }

    public String getZipCode() throws JavaLibException
    {
        Core.WrappedString zipCode = new Core.WrappedString("");

        long nativeErrorNum = nativeGetZipCode(this.getImpl(), zipCode);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (zipCode.getString());
    }

    public String getCountry() throws JavaLibException
    {
        Core.WrappedString country = new Core.WrappedString("");

        long nativeErrorNum = nativeGetCountry(this.getImpl(), country);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (country.getString());
    }

    @Override
    public String toString()
    {
        Core.WrappedString addressString = new Core.WrappedString("");

        long nativeErrorNum = nativeToString(this.getImpl(), addressString);
        if (nativeErrorNum != 0)
            return ("");

        return (addressString.getString());
    }

    private static native long nativeCreateAddress(
        int streetNum,
        String street,
        String city,
        String province,
        String zipCode,
        String country,
        Core.AddressType addressImpl);

    private static native long nativeDestroyAddress(long addressImpl);

    private static native long nativeGetStreetNum(long addressImpl, Core.WrappedInteger streetNum);

    private static native long nativeGetStreet(long addressImpl, Core.WrappedString street);

    private static native long nativeGetCity(long addressImpl, Core.WrappedString city);

    private static native long nativeGetProvince(long addressImpl, Core.WrappedString province);

    private static native long nativeGetZipCode(long addressImpl, Core.WrappedString zipCode);

    private static native long nativeGetCountry(long addressImpl, Core.WrappedString country);

    private static native long nativeToString(long addressImpl, Core.WrappedString adressString);
}
