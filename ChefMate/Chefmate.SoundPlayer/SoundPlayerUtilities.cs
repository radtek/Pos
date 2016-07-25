using System;
using System.Collections.Generic;
using System.IO;
using Chefmate.SoundPlayer.VolumeController;

namespace Chefmate.SoundPlayer
{
    public class SoundPlayerUtilities
    {
        private const string MEDIA_PATH = @"C:\WINDOWS\Media";
        private const string MEDIA_EXTN = "*.wav";
        private static volatile SoundPlayerUtilities _instance;
        private static Object lockObj = new Object();
        private VolumeManager _volumenManager;


        private SoundPlayerUtilities()
        {
            _volumenManager = new VolumeManager();

        }
        public static SoundPlayerUtilities Instance
        {
            get
            {
                lock (lockObj)
                {
                    if (_instance == null)
                    {
                        _instance = new SoundPlayerUtilities();
                    }

                } return _instance;

            }
        }

        /// <summary>
        /// play the sound passed to the method
        /// </summary>
        /// <param name="soundPath"></param>
        public void PlaySound(string soundPath)
        {
            if (!string.IsNullOrWhiteSpace(soundPath) && !soundPath.Equals("Select Sound"))
            {
                System.Media.SoundPlayer player = new System.Media.SoundPlayer();
                player.SoundLocation = Path.Combine(new string[] { MEDIA_PATH, soundPath + ".wav" });
                player.Play();
            }
        }

        /// <summary>
        /// return the list of the sounds to be added to the dropbox
        /// </summary>
        /// <returns></returns>
        public IList<string> GetSoundList()
        {
            var soundFiles = Directory.GetFiles(MEDIA_PATH, MEDIA_EXTN);
            List<string> soundFileNames = new List<string>();
            soundFileNames.Add("Select Sound");

            foreach (var item in soundFiles)
            {
                soundFileNames.Add(Path.GetFileNameWithoutExtension(item));
            }
            return soundFileNames;
        }

        /// <summary>
        /// Return the Sound of the Master system
        /// </summary>
        public int GetSystemVolume()
        {
            return _volumenManager.GetSystemVolume();
        }

        /// <summary>
        /// Set the value passed to the Master volume system
        /// </summary>
        public void SetSystemVolume(int systemVolume)
        {
            _volumenManager.SetSystemVolume(systemVolume);
        }

    }
}