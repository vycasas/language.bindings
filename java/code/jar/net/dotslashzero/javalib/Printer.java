package net.dotslashzero.javalib;

public class Printer extends Core.ApiBase<Core.PrinterType>
{
    private IGenerator m_generator;

    Printer()
    {
        m_impl = new Core.PrinterType(0);
        m_generator = null;
    }

    public Printer(IGenerator generator) throws JavaLibException
    {
        m_impl = new Core.PrinterType(0);
        m_generator = generator;

		long nativeErrorNum = nativeCreatePrinter(m_impl);

        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));
    }

	@SuppressWarnings("deprecation")
    @Override
    protected synchronized void finalize()
    {
        nativeDestroyPrinter(this.getImpl());
        return;
    }

    public void printInt() throws JavaLibException
    {
        long nativeErrorNum = nativePrintInt(this.getImpl(), m_generator);

        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return;
    }

    public void printString() throws JavaLibException
    {
        long nativeErrorNum = nativePrintString(this.getImpl(), m_generator);

        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        return;
    }

    private static native long nativeCreatePrinter(Core.PrinterType printerImpl);

    private static native long nativeDestroyPrinter(long printerImpl);

    private static native long nativePrintInt(long printerImpl, IGenerator generator);

    private static native long nativePrintString(long printerImpl, IGenerator generator);
}
