package net.dotslashzero.javalib;

/**
 * General library operations.
 */
public final class Library
{
    private Library()
    {
    }

    public static void initialize() throws UnsatisfiedLinkError, JavaLibException
    {
        System.loadLibrary("javalib");

        long nativeErrorNum = nativeInitialize();
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return;
    }

    public static void uninitialize()
    {
        nativeUninitialize();
        return;
    }

    public static String getVersionString() throws JavaLibException
    {
        Core.WrappedString versionString = new Core.WrappedString("");

        long nativeErrorNum = nativeGetVersionString(versionString);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (versionString.getString());
    }

    public static long getVersionMajor() throws JavaLibException
    {
        Core.WrappedLong versionMajor = new Core.WrappedLong(0L);

        long nativeErrorNum = nativeGetVersionMajor(versionMajor);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (versionMajor.getLong());
    }

    public static long getVersionMinor() throws JavaLibException
    {
        Core.WrappedLong versionMinor = new Core.WrappedLong(0L);

        long nativeErrorNum = nativeGetVersionMinor(versionMinor);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (versionMinor.getLong());
    }

    public static long getVersionPatch() throws JavaLibException
    {
        Core.WrappedLong versionPatch = new Core.WrappedLong(0L);

        long nativeErrorNum = nativeGetVersionPatch(versionPatch);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (versionPatch.getLong());
    }

    public static String getVersionExtra() throws JavaLibException
    {
        Core.WrappedString versionExtra = new Core.WrappedString("");

        long nativeErrorNum = nativeGetVersionExtra(versionExtra);
        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return (versionExtra.getString());
    }

    private static native long nativeInitialize();

    private static native long nativeUninitialize();

    private static native long nativeGetVersionString(Core.WrappedString versionString);

    private static native long nativeGetVersionMajor(Core.WrappedLong versionMajor);

    private static native long nativeGetVersionMinor(Core.WrappedLong versionMinor);

    private static native long nativeGetVersionPatch(Core.WrappedLong versionPatch);

    private static native long nativeGetVersionExtra(Core.WrappedString versionExtra);
}