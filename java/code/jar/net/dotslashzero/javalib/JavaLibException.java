package net.dotslashzero.javalib;

public final class JavaLibException extends Exception
{
    private static final long serialVersionUID = 5380981636603930675L;

    public JavaLibException(long errorNum)
    {
        super(getErrorMessage(errorNum));
    }

    private static String getErrorMessage(long errorNum)
    {
        Core.WrappedString errorMessage = new Core.WrappedString("");
        nativeGetErrorMessage(errorNum, errorMessage);
        return (errorMessage.getString());
    }

    private static native long nativeGetErrorMessage(long errorNum, Core.WrappedString errorMessage);
}
