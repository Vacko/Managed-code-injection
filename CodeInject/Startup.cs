using System;
using System.Diagnostics;
using System.Windows.Forms;

namespace CodeInject
{
    internal static class Startup
    {
        #region Private Fields

        private static bool m_initialized;

        #endregion Private Fields

        #region Private Methods

        [STAThread]
        private static int EntryPoint(string args)
        {
            if(m_initialized)
                return 0;

            m_initialized = true;

            MessageBox.Show($"Managed code running inside [{Process.GetCurrentProcess().ProcessName}]");

            return 0;
        }

        #endregion Private Methods
    }
}