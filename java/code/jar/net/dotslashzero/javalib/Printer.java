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

		Core.GeneratorType generatorImpl = new Core.GeneratorType(0);
        long nativeErrorNum = nativeCreateGenerator(generatorImpl);

        if (nativeErrorNum != 0)
            throw (new JavaLibException(nativeErrorNum));

        // note: this will take ownership of generatorImpl, no need to destroy explicitly when succeeded
		nativeErrorNum = nativeCreatePrinter(generatorImpl.getAddress(), m_impl);

        if (nativeErrorNum != 0) {
            nativeDestroyGenerator(generatorImpl.getAddress());
            throw (new JavaLibException(nativeErrorNum));
        }
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

	private static native long nativeCreateGenerator(Core.GeneratorType generatorImpl);

    private static native long nativeDestroyGenerator(long generatorImpl);

    // note: this will take ownership of generatorImpl, no need to destroy explicitly when successful
    private static native long nativeCreatePrinter(long generatorImpl, Core.PrinterType printerImpl);

    private static native long nativeDestroyPrinter(long printerImpl);

    private static native long nativePrintInt(long printerImpl, IGenerator generator);

    private static native long nativePrintString(long printerImpl, IGenerator generator);
}
