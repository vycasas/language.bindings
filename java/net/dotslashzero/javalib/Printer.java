package net.dotslashzero.javalib;

public class Printer extends Core.ApiBase<Core.PrinterType>
{
    Printer()
    {
        _impl = new Core.PrinterType(0);
    }

    public Printer(IGenerator generator)
    {
        _impl = new Core.PrinterType(0);
        nativeCreatePrinter(generator, _impl);
    }

    private static native int nativeCreatePrinter(IGenerator generator, Core.PrinterType printerImpl);

    @Override
    protected void finalize()
    {
        nativeDestroyPrinter(this.getImpl());
        return;
    }

    private static native int nativeDestroyPrinter(long printerImpl);

    public void printInt()
    {
        nativePrintInt(this.getImpl());
        return;
    }

    private static native int nativePrintInt(long printerImpl);

    public void printString()
    {
        nativePrintString(this.getImpl());
        return;
    }

    private static native int nativePrintString(long printerImpl);
}
