package net.dotslashzero.javalib;

public class JavaLibException extends Exception
{
    Core.ExceptionType _impl;

    long getImpl()
    {
        return (_impl.getAddress());
    }

    JavaLibException(long impl)
    {
        _impl = new Core.ExceptionType(impl);
    }

    @Override
    protected void finalize()
    {
        //nativeDestroyException(this.getImpl());
        return;
    }

    private static native int nativeDestroyException(long exceptionImpl);

    @Override
    public String getMessage()
    {
        char[] message = new char[512];
        int nativeCallResult = nativeGetMessage(this.getImpl(), message);
        if (nativeCallResult != 0)
            return (new String());
        return (new String(message));
    }

    private static native int nativeGetMessage(long exceptionImpl, char[] message);
}
