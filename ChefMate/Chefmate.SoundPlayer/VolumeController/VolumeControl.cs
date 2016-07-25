using System;
using System.Runtime.InteropServices;
using Chefmate.Logger;

namespace Chefmate.SoundPlayer.VolumeController
{
    public class VolumeControl
    {
        /// <summary>
        /// method to retrieve a mixer control
        /// </summary>
        /// <param name="hmxObj"></param>
        /// <param name="type"></param>
        /// <param name="ctrlType"></param>
        /// <param name="mixerCtrl"></param>
        /// <param name="curVolume"></param>
        /// <returns></returns>
        private static bool GetSystemMixer(int hmxObj, int type, int ctrlType, out VolumeStructures.MixerStruct mixerCtrl, out int curVolume)
        {
            //create our method level variables
            int details;
            bool bReturn;
            curVolume = -1;

            //create our struct objects
            VolumeStructures.LineControlStruct lineControls = new VolumeStructures.LineControlStruct();
            VolumeStructures.MixerLineStruct line = new VolumeStructures.MixerLineStruct();
            VolumeStructures.MixerDetailStruct mcDetails = new VolumeStructures.MixerDetailStruct();
            VolumeStructures.UnsignedMixerDetailStruct detailsUnsigned = new VolumeStructures.UnsignedMixerDetailStruct();

            //create a new mixer control
            mixerCtrl = new VolumeStructures.MixerStruct();
           
            //set the properties of out mixerline object
            line.cbStruct = Marshal.SizeOf(line);
            line.dwComponentType = type;
            //get the line info and assign it to our details variable
            details = WinXPSystem.mixerGetLineInfoA(hmxObj, ref line, VolumeConstants.MIXER_GETLINEINFOF_COMPONENTTYPE);

            //make sure we didnt receive any errors
            if (VolumeConstants.MMSYSERR_NOERROR == details)
            {
                int mcSize = 152;
                //get the size of the unmanaged type
                int control = Marshal.SizeOf(typeof(VolumeStructures.MixerStruct));
                //allocate a block of memory
                lineControls.pamxctrl = Marshal.AllocCoTaskMem(mcSize);
                //get the size of the line controls
                lineControls.cbStruct = Marshal.SizeOf(lineControls);

                //set properties for our mixer control
                lineControls.dwLineID = line.dwLineID;
                lineControls.dwControl = ctrlType;
                lineControls.cControls = 1;
                lineControls.cbmxctrl = mcSize;

                // Allocate a buffer for the control
                mixerCtrl.cbStruct = mcSize;

                // Get the control
                details = WinXPSystem.mixerGetLineControlsA(hmxObj, ref lineControls, VolumeConstants.MIXER_GETLINECONTROLSF_ONEBYTYPE);

                //once again check to see if we received any errors
                if (VolumeConstants.MMSYSERR_NOERROR == details)
                {
                    bReturn = true;
                    //Copy the control into the destination structure
                    mixerCtrl = (VolumeStructures.MixerStruct)Marshal.PtrToStructure(lineControls.pamxctrl, typeof(VolumeStructures.MixerStruct));
                }
                else
                {
                    bReturn = false;
                }

                int mcDetailsSize = Marshal.SizeOf(typeof(VolumeStructures.MixerDetailStruct));
                int mcDetailsUnsigned = Marshal.SizeOf(typeof(VolumeStructures.UnsignedMixerDetailStruct));
                mcDetails.cbStruct = mcDetailsSize;
                mcDetails.dwControlID = mixerCtrl.dwControlID;
                mcDetails.paDetails = Marshal.AllocCoTaskMem(mcDetailsUnsigned);
                mcDetails.cChannels = 1;
                mcDetails.item = 0;
                mcDetails.cbDetails = mcDetailsUnsigned;
                details = WinXPSystem.mixerGetControlDetailsA(hmxObj, ref mcDetails, VolumeConstants.MIXER_GETCONTROLDETAILSF_VALUE);
                detailsUnsigned = (VolumeStructures.UnsignedMixerDetailStruct)Marshal.PtrToStructure(mcDetails.paDetails, typeof(VolumeStructures.UnsignedMixerDetailStruct));
                curVolume = detailsUnsigned.dwValue;
                return bReturn;
            }

            bReturn = false;
            return bReturn;
        }


        /// <summary>
        /// method for setting the value for a volume control
        /// </summary>
        /// <param name="hmxObj"></param>
        /// <param name="mixerControl"></param>
        /// <param name="volumeLevel"></param>
        /// <returns>true/false</returns>
        private static bool SetSystemMixer(int hmxObj, VolumeStructures.MixerStruct mixerControl, int volumeLevel)
        {
            bool boolReturn;
            int details;

            //create our struct object for controlling the system sound
            VolumeStructures.MixerDetailStruct mixerDetails = new VolumeStructures.MixerDetailStruct();
            VolumeStructures.UnsignedMixerDetailStruct volume = new VolumeStructures.UnsignedMixerDetailStruct();

            //set out mixer control properties
            mixerDetails.item = 0;
            //set the id of the mixer control
            mixerDetails.dwControlID = mixerControl.dwControlID;
            //return the size of the mixer details struct
            mixerDetails.cbStruct = Marshal.SizeOf(mixerDetails);
            //return the volume
            mixerDetails.cbDetails = Marshal.SizeOf(volume);

            //Allocate a buffer for the mixer control value buffer
            mixerDetails.cChannels = 1;
            volume.dwValue = volumeLevel;

            //Copy the data into the mixer control value buffer
            mixerDetails.paDetails = Marshal.AllocCoTaskMem(Marshal.SizeOf(typeof(VolumeStructures.UnsignedMixerDetailStruct)));
            Marshal.StructureToPtr(volume, mixerDetails.paDetails, false);

            //Set the control value
            details = WinXPSystem.mixerSetControlDetails(hmxObj, ref mixerDetails, VolumeConstants.MIXER_SETCONTROLDETAILSF_VALUE);

            //Check to see if any errors were returned
            if (VolumeConstants.MMSYSERR_NOERROR == details)
            {
                boolReturn = true;
            }
            else
            {
                boolReturn = false;
            }
            return boolReturn;
        }


        /// <summary>
        /// method for retrieving the current volume from the system
        /// </summary>
        /// <returns>int value</returns>
        public static int GetSystemVolume()
        {
            //method level variables
            int curVolume;
            int mixerCtrl;

            //create a new volume control
            VolumeStructures.MixerStruct mixer = new VolumeStructures.MixerStruct();
            
            //open the mixer
            WinXPSystem.mixerOpen(out mixerCtrl, 0, 0, 0, 0);

            //set the type to volume control type
            int type = VolumeConstants.MIXERCONTROL_CONTROLTYPE_VOLUME;

            //get the mixer control and get the current volume level
            GetSystemMixer(mixerCtrl, VolumeConstants.MIXERLINE_COMPONENTTYPE_DST_SPEAKERS, type, out mixer, out curVolume);

            //close the mixer control since we are now done with it
            WinXPSystem.mixerClose(mixerCtrl);

            //return the current volume to the calling method
            return curVolume;
        }


        /// <summary>
        /// method for setting the volume to a specific level
        /// </summary>
        /// <param name="volumeLevel">volume level we wish to set volume to</param>
        public static void SetSystemVolume(int volumeLevel)
        {
            try
            {
                int curVolume;
                int mixerCtrl;

                //create a new volume control
                VolumeStructures.MixerStruct volumeCtrl = new VolumeStructures.MixerStruct();

                //open the mixer control
                WinXPSystem.mixerOpen(out mixerCtrl, 0, 0, 0, 0);

                //set the type to volume control type
                int controlType = VolumeConstants.MIXERCONTROL_CONTROLTYPE_VOLUME;

                //get the current mixer control and get the current volume
                GetSystemMixer(mixerCtrl, VolumeConstants.MIXERLINE_COMPONENTTYPE_DST_SPEAKERS, controlType, out volumeCtrl, out curVolume);

                //Check for volume level. If the volume level
                //is greater than the maximum level then set the volume to
                //the maximum level. If it's less than the minimum level
                //then set it to the minimun level
                if (volumeLevel > volumeCtrl.lMaximum)
                {
                    volumeLevel = volumeCtrl.lMaximum;
                }
                else if (volumeLevel < volumeCtrl.lMinimum)
                {
                    volumeLevel = volumeCtrl.lMinimum;
                }

                //set the volume
                SetSystemMixer(mixerCtrl, volumeCtrl, volumeLevel);

                //now re-get the mixer control
                GetSystemMixer(mixerCtrl, VolumeConstants.MIXERLINE_COMPONENTTYPE_DST_SPEAKERS, controlType, out volumeCtrl, out curVolume);

                //make sure the volume level is equal to the current volume
                if (volumeLevel != curVolume)
                {
                    throw new Exception("Cannot Set Volume");
                }

                //close the mixer control as we are finished with it
                WinXPSystem.mixerClose(mixerCtrl);
            }
            catch (Exception ex)
            {
                ChefmateLogger.Instance.LogError("SetSystemVolume",ex.Message);
            }            
        }
    }
}