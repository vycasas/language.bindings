package net.dotslashzero.javalib;

/**
 * General library operations.
 */
public class Library
{
    public static void initialize() throws UnsatisfiedLinkError
    {
        System.loadLibrary("javalib");
        return;
    }

    public static void terminate()
    {
        return;
    }

    public static String getVersionString()
    {
        Core.WrappedString result = new Core.WrappedString("");
        nativeGetVersionString(result);
        return (result.getValue());
    }

    private static native int nativeGetVersionString(Core.WrappedString versionString);

    public static long getVersionMajor()
    {
        Core.WrappedLong result = new Core.WrappedLong(0L);
        nativeGetVersionMajor(result);
        return (result.getValue());
    }

    private static native int nativeGetVersionMajor(Core.WrappedLong versionMajor);

    public static long getVersionMinor()
    {
        Core.WrappedLong result = new Core.WrappedLong(0L);
        nativeGetVersionMinor(result);
        return (result.getValue());
    }

    private static native int nativeGetVersionMinor(Core.WrappedLong versionMinor);

    private Library()
    {
    }
}