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
        nativePrinterCreate(generator, _impl);
    }

    private static native int nativePrinterCreate(IGenerator generator, Core.PrinterType printerImpl);

    public void printInt()
    {
        nativePrinterPrintInt(_impl);
        return;
    }

    private static native int nativePrinterPrintInt(Core.PrinterType printerImpl);

    public void printString()
    {
        nativePrinterPrintString(_impl);
        return;
    }

    private static native int nativePrinterPrintString(Core.PrinterType printerImpl);
}
