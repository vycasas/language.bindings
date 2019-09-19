using System;
using System.Diagnostics;

namespace DotSlashZero.DotNetCoreLib
{
    public sealed class Printer
    {
        private IntPtr m_pImpl = IntPtr.Zero;

        // these two references are for keeping the objects alive
        private IGenerator m_generator = null;
        private CLib.GeneratorWrapper m_generatorWrapper = null;

        public Printer(IGenerator generator)
        {
            m_generator = generator;
            var pGeneratorImpl = CreateGeneratorImpl();

            m_pImpl = CLib.PrinterCreate(pGeneratorImpl);
        }

        ~Printer()
        {
            try
            {
                if (m_pImpl != IntPtr.Zero)
                {
                    CLib.PrinterDestroy(m_pImpl);
                    m_pImpl = IntPtr.Zero;
                }
            }
            catch { /* no throw */ }
        }

        public void PrintInt()
        {
            Debug.Assert(m_pImpl != IntPtr.Zero);
            CLib.PrinterPrintInt(m_pImpl);
            return;
        }

        public void PrintString()
        {
            Debug.Assert(m_pImpl != IntPtr.Zero);
            CLib.PrinterPrintString(m_pImpl);
            return;
        }

        private IntPtr CreateGeneratorImpl()
        {
            m_generatorWrapper = new CLib.GeneratorWrapper(
                m_generator.GenerateInt,
                m_generator.GenerateString);

            var generatorImpl = CLib.GeneratorCreate(m_generatorWrapper);

            return (generatorImpl);
        }
    }
}
