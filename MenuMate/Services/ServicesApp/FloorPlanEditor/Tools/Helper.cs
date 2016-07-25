using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows.Forms;
using System.Threading;
using System.Xml;

using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace FloorPlanIntegration
{
    class Helper
    {
        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// 
        /// </summary>
        Helper()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        public static Helper Instance
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

            internal static readonly Helper instance = new Helper();
        }

        //:::::::::::::::::::::::::::::::::

        /// <summary>
        /// Returns the startup path
        /// </summary>
        static public string StartPath
        {
            get
            {
                return System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase).Substring(6);
            }
        }

        //:::::::::::::::::::::::::::::::::

        static public string GetFileURI(string inFileName)
        {
            return StartPath + @"\" + inFileName;
        }

        //:::::::::::::::::::::::::::::::::

        static public XmlDocument GetXMLDocFromFile(string inConfigFilename)
        {
            XmlDocument result = new XmlDocument();

            //:::::::::::::::::::::::::::::

            XmlReader reader = new XmlTextReader(GetFileURI(inConfigFilename));
            reader.Read();
            result.Load(reader);

            //::::::::::::::::::::::::::::

            return result;
        }

        #region System_Windows_Forms_Timer

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        /// <returns></returns>
        static public System.Windows.Forms.Timer CreateTimer(int inInterval)
        {
            System.Windows.Forms.Timer result = new System.Windows.Forms.Timer();

            result.Interval = inInterval;
            result.Stop();

            return result;
        }

        #endregion

        #region System_Threading_Timer

        public delegate void TimerTick(object obj);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static public System.Threading.Timer CreateTimer(TimerTick inCallback, int inDueTime, int inPeriod)
        {
            TimerCallback callBack = new TimerCallback(inCallback);
            System.Threading.Timer result = new System.Threading.Timer(callBack, null, inDueTime, inPeriod);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        static public System.Threading.Timer CreateTimer(TimerTick inCallback)
        {
            TimerCallback callBack = new TimerCallback(inCallback);
            System.Threading.Timer result = new System.Threading.Timer(callBack, null, Timeout.Infinite, Timeout.Infinite);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTimer"></param>
        /// <param name="inDueTime"></param>
        /// <param name="inPeriod"></param>
        static public void StartTimer(System.Threading.Timer inTimer, int inDueTime, int inPeriod)
        {
            ChangeTimer(inTimer, inDueTime, inPeriod);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTimer"></param>
        static public void StopTimer(System.Threading.Timer inTimer)
        {
            ChangeTimer(inTimer, Timeout.Infinite, Timeout.Infinite);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTimer"></param>
        static public void KillTimer(System.Threading.Timer inTimer)
        {
            ChangeTimer(inTimer, Timeout.Infinite, Timeout.Infinite);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inTimer"></param>
        /// <param name="inDueTime"></param>
        /// <param name="inPeriod"></param>
        static public void ChangeTimer(System.Threading.Timer inTimer, int inDueTime, int inPeriod)
        {
            try
            {
                inTimer.Change(inDueTime, inPeriod);
            }
            catch
            {
                 throw;
            }
        }

        #endregion

        #region Key
        /// <summary>
        /// 
        /// </summary>
        /// <param name="inKey"></param>
        /// <returns></returns>
        public static bool keyIsANumber(Keys inKey)
        {
            return (inKey >= Keys.D0) && (inKey <= Keys.D9);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inKey"></param>
        /// <returns></returns>
        public static bool keyIsALetter(Keys inKey)
        {
            return (inKey >= Keys.A) && (inKey <= Keys.Z);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inKey"></param>
        /// <returns></returns>
        public static bool keyIsBackSpace(Keys inKey)
        {
            return inKey == Keys.Back;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inKey"></param>
        /// <returns></returns>
        public static bool keyIsDelete(Keys inKey)
        {
            return inKey == Keys.Delete;
        }

        #endregion

        #region Image

        /// <summary>
        /// 
        /// </summary>
        /// <param name="imageIn"></param>
        /// <param name="inImgFormat"></param>
        /// <returns></returns>
        public static byte[] imageToByteArray(Image imageIn, ImageFormat inImgFormat)
        {
            MemoryStream ms = new MemoryStream();
            imageIn.Save(ms, inImgFormat);
            return ms.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="byteArrayIn"></param>
        /// <returns></returns>
        public static Image byteArrayToImage(byte[] byteArrayIn)
        {
            return Image.FromStream(new MemoryStream(byteArrayIn));
        }

        #endregion
    }
}
