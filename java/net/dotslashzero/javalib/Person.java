package net.dotslashzero.javalib;

public class Person extends Core.ApiBase<Core.PersonType>
{
    Person()
    {
        _impl = new Core.PersonType(0);
    }

    public Person(String lastName, String firstName, int age, Address address) throws JavaLibException
    {
        _impl = new Core.PersonType(0);
        // native function makes a copy of the address argument.
        nativeCreatePerson(lastName, firstName, age, address.getImpl(), _impl);
    }

    private static native int nativeCreatePerson(
        String lastName, String firstName, int age, long addressImpl,
        Core.PersonType personImpl
    );
	
	@SuppressWarnings("deprecation")
    @Override
    protected synchronized void finalize()
    {
        nativeDestroyPerson(this.getImpl());
        return;
    }

    private static native int nativeDestroyPerson(long personImpl);

    public String getLastName() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetLastName(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetLastName(long personImpl, Core.WrappedString lastName);

    public String getFirstName() throws JavaLibException
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetFirstName(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetFirstName(long personImpl, Core.WrappedString firstName);

    public int getAge() throws JavaLibException
    {
        Core.WrappedInteger result = new Core.WrappedInteger(0);
        nativeGetAge(this.getImpl(), result);
        return (result.getValue());
    }

    private static native int nativeGetAge(long personImpl, Core.WrappedInteger age);

    public Address getAddress() throws JavaLibException
    {
        Address result = new Address();
        nativeGetAddress(this.getImpl(), result._impl);
        return (result);
    }

    private static native int nativeGetAddress(long personImpl, Core.AddressType addressImpl);    

    @Override
    public String toString()
    {
        try {
            StringBuilder sb = new StringBuilder();
            sb.append(this.getLastName());
            sb.append(", ");
            sb.append(this.getFirstName());
            sb.append(System.lineSeparator());
            sb.append(this.getAge());
            sb.append(" years old");
            sb.append(System.lineSeparator());
            sb.append(this.getAddress().toString());
            return (sb.toString());
        }
        catch (Throwable t) {
            return ("");
        }

    }
}
