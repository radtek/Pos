using System;
using System.Runtime.InteropServices;

namespace Safemate.Core.Backup
{
    static public class NtfsWrapper
    {
        [DllImport("kernel32.dll", EntryPoint = "CreateHardLink")]
        static extern Boolean createLink(String to, String from,
                                         IntPtr lpSecurityAttributes);

        [DllImport("kernel32.dll", EntryPoint = "DeleteFile")]
        static extern Boolean deleteFile(String filename);

        static public void CreateLink(String from, String to)
        {
            if (!createLink(to, from, IntPtr.Zero))
                throw new Exception(String.Format(
                                        "Couldn't link \"{0}\" to \"{1}\".)",
                                        from, to));
        }

        static public void DirectDeleteFile(String filename)
        {
            if (!deleteFile(filename))
                throw new Exception("Couldn't delete file " +
                                    filename);
        }
    };
};
