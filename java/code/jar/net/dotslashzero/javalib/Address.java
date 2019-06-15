package net.dotslashzero.javalib;

public class Address extends Core.ApiBase<Core.AddressType>
{
    Address()
    {
        _impl = new Core.AddressType(0);
    }

    public Address(
        int streetNum, String street,
        String city, String province,
        String country, String zipCode
    ) throws JavaLibException
    {
        _impl = new Core.AddressType(0);
        // Note: Even if this native method returns a result that we can evaluate, the native function itself is
        // capable of throwing exceptions.
        nativeCreateAddress(streetNum, street, city, province, country, zipCode, _impl);
    }

    private static native int nativeCreateAddress(
        int streetNum, String street,
        String city, String province,
        String country, String zipCode,
        Core.AddressType addressImpl
    );
	
	@SuppressWarnings("deprecation")
    @Override
    protected synchronized void finalize()
    {
        nativeDestroyAddress(this.getImpl());
        return;
    }

    private static native int nativeDestroyAddress(long addressImpl);

    public int getStreetNum() throws JavaLibException
    {
        Core.WrappedInteger result = new Core.WrappedInteger(0);
        nativeGetStreetNum(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetStreetNum(long addressImpl, Core.WrappedInteger streetNum);

    public String getStreet() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetStreet(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetStreet(long addressImpl, Core.WrappedString street);

    public String getCity() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetCity(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetCity(long addressImpl, Core.WrappedString city);

    public String getProvince() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetProvince(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetProvince(long addressImpl, Core.WrappedString province);

    public String getCountry() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetCountry(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetCountry(long addressImpl, Core.WrappedString country);

    public String getZipCode() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetZipCode(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetZipCode(long addressImpl, Core.WrappedString zipCode);

    @Override
    public String toString()
    {
        try {
            StringBuilder sb = new StringBuilder();
            sb.append(this.getStreetNum());
            sb.append(" ");
            sb.append(this.getStreet());
            sb.append(System.lineSeparator());
            sb.append(this.getCity());
            sb.append(", ");
            sb.append(this.getProvince());
            sb.append(System.lineSeparator());
            sb.append(this.getCountry());
            sb.append(" ");
            sb.append(this.getZipCode());
            return (sb.toString());
        }
        catch (Throwable t) {
            return ("");
        }
    }
}
