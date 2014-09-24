package net.dotslashzero.javalib;

/**
 * Package only class.
 */
class Core
{
    static class WrappedType<T>
    {
        T _value;

        public WrappedType(T value)
        {
            _value = value;
        }

        public T getValue()
        {
            return (_value);
        }

        public void setValue(T value)
        {
            _value = value;
            return;
        }
    }

    static class NativeType extends WrappedType<Long>
    {
        public NativeType(long impl)
        {
            super(Long.valueOf(impl));
        }

        public void setAddress(long address)
        {
            this.setValue(Long.valueOf(address));
            return;
        }

        public long getAddress()
        {
            return (this.getValue().longValue());
        }
    }

    static class ExceptionType extends NativeType
    {
        public ExceptionType(long impl)
        {
            super(Long.valueOf(impl));
        }
    }

    static class AddressType extends NativeType
    {
        public AddressType(long impl)
        {
            super(Long.valueOf(impl));
        }
    }

    static class PersonType extends NativeType
    {
        public PersonType(long impl)
        {
            super(Long.valueOf(impl));
        }
    }

    static class PrinterType extends NativeType
    {
        public PrinterType(long impl)
        {
            super(Long.valueOf(impl));
        }
    }

    public static class ApiBase<T extends NativeType>
    {
        T _impl;

        long getImpl()
        {
            return (_impl.getAddress());
        }
    }

    static class WrappedInteger extends WrappedType<Integer>
    {
        public WrappedInteger(Integer value)
        {
            super(value);
        }

        public void setInt(int value)
        {
            this.setValue(Integer.valueOf(value));
            return;
        }

        public int getInt()
        {
            return (this.getValue().intValue());
        }
    }

    static class WrappedString extends WrappedType<String>
    {
        public WrappedString(String value)
        {
            super(value);
        }

        public void setString(String value)
        {
            this.setValue(value);
            return;
        }

        public String getString()
        {
            return (this.getValue());
        }
    }
}
