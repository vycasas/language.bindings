package net.dotslashzero.javalib;

public class Person extends Core.ApiBase<Core.PersonType>
{
    Person()
    {
        m_impl = new Core.PersonType(0);
    }

    public Person(String lastName, String firstName, int age, Address address) throws JavaLibException
    {
        m_impl = new Core.PersonType(0);

        long nativeErrorNum = nativeCreatePerson(lastName, firstName, age, address.getImpl(), m_impl);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));
    }

	@SuppressWarnings("deprecation")
    @Override
    protected synchronized void finalize()
    {
        nativeDestroyPerson(this.getImpl());
        return;
    }

    public String getLastName() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");

        long nativeErrorNum = nativeGetLastName(this.getImpl(), result);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (result.getValue());
    }

    public String getFirstName() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");

        long nativeErrorNum = nativeGetFirstName(this.getImpl(), result);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (result.getValue());
    }

    public int getAge() throws JavaLibException
    {
        Core.WrappedInteger result = new Core.WrappedInteger(0);

        long nativeErrorNum = nativeGetAge(this.getImpl(), result);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (result.getValue());
    }

    public Address getAddress() throws JavaLibException
    {
        Address result = new Address();

        long nativeErrorNum = nativeGetAddress(this.getImpl(), result.m_impl);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (result);
    }

    @Override
    public String toString()
    {
        Core.WrappedString personString = new Core.WrappedString("");

        long nativeErrorNum = nativeToString(this.getImpl(), personString);
        if (nativeErrorNum != 0)
            return ("");

        return (personString.getString());
    }

    private static native long nativeCreatePerson(
        String lastName,
        String firstName,
        int age,
        long addressImpl,
        Core.PersonType personImpl);

    private static native long nativeDestroyPerson(long personImpl);

    private static native long nativeGetLastName(long personImpl, Core.WrappedString lastName);

    private static native long nativeGetFirstName(long personImpl, Core.WrappedString firstName);

    private static native long nativeGetAge(long personImpl, Core.WrappedInteger age);

    private static native long nativeGetAddress(long personImpl, Core.AddressType addressImpl);

    private static native long nativeToString(long personImpl, Core.WrappedString personString);
}
