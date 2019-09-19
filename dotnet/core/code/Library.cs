namespace DotSlashZero.DotNetCoreLib
{
    public static class Library
    {
        public static bool Initialize()
        {
            var result = CLib.LibraryInitialize();
            return (result);
        }

        public static void Uninitialize()
        {
            CLib.LibraryUninitialize();
            return;
        }

        public static string VersionString
        {
            get
            {
                var versionString = CLib.LibraryGetVersionString();
                return (versionString);
            }
        }

        public static int VersionMajor
        {
            get
            {
                var versionMajor = CLib.LibraryGetVersionMajor();
                return (versionMajor);
            }
        }

        public static int VersionMinor
        {
            get
            {
                var versionMinor = CLib.LibraryGetVersionMinor();
                return (versionMinor);
            }
        }

        public static int VersionPatch
        {
            get
            {
                var versionPatch = CLib.LibraryGetVersionPatch();
                return (versionPatch);
            }
        }

        public static string VersionExtra
        {
            get
            {
                var versionExtra = CLib.LibraryGetVersionExtra();
                return (versionExtra);
            }
        }
    }
}
