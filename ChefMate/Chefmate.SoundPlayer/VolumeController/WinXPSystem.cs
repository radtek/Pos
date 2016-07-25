using System;
using System.Runtime.InteropServices;

namespace Chefmate.SoundPlayer.VolumeController
{
    /// <summary>
    /// Class that contains all the Win32 API calls
    /// we need for controlling the system sound
    /// </summary>
    public static class WinXPSystem
    {
        [DllImport("winmm.dll", CharSet=CharSet.Ansi)] 
        public static extern int mixerClose (int hmx);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerGetControlDetailsA(int hmxobj, ref VolumeStructures.MixerDetailStruct pmxcd, int fdwDetails);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerGetDevCapsA(int uMxId, VolumeStructures.MixerCapStruct pmxcaps, int cbmxcaps);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerGetID(int hmxobj, int pumxID, int fdwId);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerGetLineControlsA(int hmxobj, ref VolumeStructures.LineControlStruct pmxlc, int fdwControls);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerGetLineInfoA(int hmxobj, ref VolumeStructures.MixerLineStruct pmxl, int fdwInfo);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerGetNumDevs();

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerMessage(int hmx, int uMsg, int dwParam1, int dwParam2);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerOpen(out int phmx, int uMxId, int dwCallback, int dwInstance, int fdwOpen);

        [DllImport("winmm.dll", CharSet=CharSet.Ansi)]
        public static extern int mixerSetControlDetails(int hmxobj, ref VolumeStructures.MixerDetailStruct pmxcd, int fdwDetails);
    }

    public static class NativeMethods
    {

        [DllImport("winmm.dll", EntryPoint = "waveOutSetVolume")]
        public static extern int WaveOutSetVolume(IntPtr hwo, uint dwVolume);


        [DllImport("winmm.dll", SetLastError = true)]
        public static extern bool PlaySound(string pszSound, IntPtr hmod, uint fdwSound);
    }

}
