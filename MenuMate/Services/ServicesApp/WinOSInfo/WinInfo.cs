using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Security.Principal;

namespace WinOSInfo
{
    public enum WinOSTypes
    {
        WinUnknown, Win95, Win98, Win98SE, WinME, WinNT351, WinNT4, Win2000, WinXP, WinVista, Win7
    };

    public class WinInfo
    {
        OperatingSystem _os; //Get Operating system information.
        Version         _vs; //Get version information about the os.
            
        /// <summary>
        /// 
        /// </summary>
        public WinInfo()
        {
            _os = Environment.OSVersion;
            _vs = _os.Version;
        }

        /// <summary>
        /// 
        /// </summary>
        public static WinInfo Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly WinInfo instance = new WinInfo();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public WinOSTypes WinOSType()
        {
            return (_os.Platform == PlatformID.Win32Windows) ? pWinOSType_Win32() : pWinOSType_WinNT();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string WinOSInfo()
        {
            string result = @"Windows";

            //::::::::::::::::::::::::::::::::::::::::

            string osStr = "";

            switch (WinOSType())
            {
                case WinOSTypes.Win95: osStr = @"95"; break;
                case WinOSTypes.Win98: osStr = @"98"; break;
                case WinOSTypes.Win98SE: osStr = @"98 SE"; break;
                case WinOSTypes.WinME: osStr = @"ME"; break;
                case WinOSTypes.WinNT351: osStr = @"NT 3.51"; break;
                case WinOSTypes.WinNT4: osStr = @"NT 4.0"; break;
                case WinOSTypes.Win2000: osStr = @"2000"; break;
                case WinOSTypes.WinXP: osStr = @"XP"; break;
                case WinOSTypes.WinVista: osStr = @"Vista"; break;
                case WinOSTypes.Win7: osStr = @"7"; break;
                case WinOSTypes.WinUnknown: osStr = @"Unknown"; break;
                default: osStr = @"Unknown"; break;
            }

            result = string.Format(@"{0} {1} {2}", result, osStr, WinOSArchitecture());

            //::::::::::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string WinOSArchitectureStr()
        {
            return string.Format(@"{0} bits", WinOSArchitecture());
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int WinOSArchitecture()
        {
            string pa = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");
            return ((String.IsNullOrEmpty(pa) || String.Compare(pa, 0, "x86", 0, 3, true) == 0) ? 32 : 64);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool IsAdministrator()
        {
            WindowsIdentity identity = WindowsIdentity.GetCurrent();

            if (null != identity)
            {
                WindowsPrincipal principal = new WindowsPrincipal(identity);
                return principal.IsInRole(WindowsBuiltInRole.Administrator);
            }

            return false;
        }

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private WinOSTypes pWinOSType_Win32()
        {
            switch (_vs.Minor)
            {
                case 0:  return WinOSTypes.Win95;
                case 10:
                    if (_vs.Revision.ToString() == "2222A")
                         return WinOSTypes.Win98SE;
                    else
                         return WinOSTypes.Win98;
                case 90: return WinOSTypes.WinME;
                default: return WinOSTypes.WinUnknown;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private WinOSTypes pWinOSType_WinNT()
        {
            switch (_vs.Major)
            {
                case 3: return WinOSTypes.WinNT351;
                case 4: return WinOSTypes.WinNT4;
                case 5:
                    if (_vs.Minor == 0)
                         return WinOSTypes.Win2000;
                    else
                         return WinOSTypes.WinXP;
                case 6:
                    if (_vs.Minor == 0)
                         return WinOSTypes.WinVista;
                    else
                         return WinOSTypes.Win7;
                default: return WinOSTypes.WinUnknown;
            }
        }

        #endregion
    }
}
