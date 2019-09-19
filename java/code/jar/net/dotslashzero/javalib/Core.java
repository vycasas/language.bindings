package net.dotslashzero.javalib;

/**
 * Package only class.
 */
class Core
{
    static class WrappedType<T>
    {
        T m_value;

        public WrappedType(T value)
        {
            m_value = value;
        }

        public T getValue()
        {
            return (m_value);
        }

        public void setValue(T value)
        {
            m_value = value;
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
            setValue(Long.valueOf(address));
            return;
        }

        public long getAddress()
        {
            return (getValue().longValue());
        }
    }

    static final class AddressType extends NativeType
    {
        public AddressType(long impl)
        {
            super(Long.valueOf(impl));
        }
    }

    static final class PersonType extends NativeType
    {
        public PersonType(long impl)
        {
            super(Long.valueOf(impl));
        }
    }

    static final class PrinterType extends NativeType
    {
        public PrinterType(long impl)
        {
            super(Long.valueOf(impl));
        }
    }

    public static class ApiBase<T extends NativeType>
    {
        T m_impl;

        long getImpl()
        {
            return (m_impl.getAddress());
        }
    }

    static final class WrappedInteger extends WrappedType<Integer>
    {
        public WrappedInteger(Integer value)
        {
            super(value);
        }

        public void setInt(int value)
        {
            setValue(Integer.valueOf(value));
            return;
        }

        public int getInt()
        {
            return (getValue().intValue());
        }
    }

    static final class WrappedLong extends WrappedType<Long>
    {
        public WrappedLong(Long value)
        {
            super(value);
        }

        public void setLong(long value)
        {
            setValue(Long.valueOf(value));
            return;
        }

        public long getLong()
        {
            return (getValue().longValue());
        }
    }

    static final class WrappedString extends WrappedType<String>
    {
        public WrappedString(String value)
        {
            super(value);
        }

        public void setString(String value)
        {
            setValue(value);
            return;
        }

        public String getString()
        {
            return (getValue());
        }
    }
}
