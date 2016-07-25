using System;
using System.Management;
using Chefmate.Logger;
using NAudio.CoreAudioApi;

namespace Chefmate.SoundPlayer.VolumeController
{
    public class VolumeManager
    {
       public int GetSystemVolume()
        {
          return  GetVolume();
        }


        public void SetSystemVolume(int systemVolume)
        {
            SetVolume(systemVolume);
        }

        public int GetVolume()
        {

            var osFriendlyName = GetOsName();

            if (osFriendlyName.Contains(global::Chefmate.SoundPlayer.VolumeController.VolumeStructures.MsWindowsFriendlyNames.WindowsXp))
            {
                return GetSystemVolumeXp();
            }
            else
            {
                return GetVolumeVistaPlus();
            }
        }


        public void SetVolume(int value)
        {
            if (value < 0)
                value = 0;

            if (value > 100)
                value = 100;

            var osFriendlyName = GetOsName();

            if (osFriendlyName.Contains(global::Chefmate.SoundPlayer.VolumeController.VolumeStructures.MsWindowsFriendlyNames.WindowsXp))
            {
                SetSystemVolumeXp(value);
            }
            else
            {
                SetVolumeForVistaPlus(value);
            }
        }

 
        public int GetSystemVolumeXp()
        {
            //return the list of the sounds to be added to the dropbox
            int sysVolume = PC_VolumeControl.VolumeControl.GetVolume();
            // Get the volume on a scale of 1 to 100( to fit the tracker)
            return sysVolume / (ushort.MaxValue / 100);
        }


        public void SetSystemVolumeXp(int systemVolume)
        {
            // converting the trackbar value to system volume level
            int sysVol = (systemVolume * (ushort.MaxValue / 100));
            PC_VolumeControl.VolumeControl.SetVolume(sysVol);
        }


        public int GetVolumeVistaPlus()
        {
            int result = 100;
            try
            {
                MMDeviceEnumerator devEnum = new MMDeviceEnumerator();
                MMDevice device = devEnum.GetDefaultAudioEndpoint(DataFlow.Render, Role.Multimedia);
                result = (int)(device.AudioEndpointVolume.MasterVolumeLevelScalar * 100);
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("GetVolumeVistaPlus : ", ex.Message);
            }

            return result;
        }

        private void SetVolumeForVistaPlus(int value)
        {
            try
            {
                MMDeviceEnumerator devEnum = new MMDeviceEnumerator();
                MMDevice device = devEnum.GetDefaultAudioEndpoint(DataFlow.Render, Role.Multimedia);
                device.AudioEndpointVolume.MasterVolumeLevelScalar = ((float)(value / 100.0f));

            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("SetVolumeForVistaPlus : ", ex.Message);
            }

        }


        private string GetOsName()
        {
            string result = string.Empty;

            ManagementObjectSearcher searcher = new ManagementObjectSearcher("SELECT Caption FROM Win32_OperatingSystem");
            foreach (var o in searcher.Get())
            {
                var os = (ManagementObject) o;
                result = os["Caption"].ToString();
                break;
            }

            return result;
        }

    }   

}
