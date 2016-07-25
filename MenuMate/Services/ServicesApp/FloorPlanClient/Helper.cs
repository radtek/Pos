using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

namespace FloorPlanClient
{
    class Helper
    {
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
    }
}
