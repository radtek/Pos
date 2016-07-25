using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Reflection;

using System.Drawing.Drawing2D;

namespace FloorPlanClient
{
    public enum ImageOrientation
    {
        Portrait,
        Lanscape
    };

    public class LocationBackImage
    {
        Image _image;
        ImageOrientation _imgOrientation = ImageOrientation.Lanscape;

        int BCK_LANDSCAPE_IMAGE_WIDTH = ImageWidthLandscape();
        int BCK_LANDSCAPE_IMAGE_HEIGHT = ImageHeightLandscape();

        int BCK_PORTRAIT_IMAGE_WIDTH = ImageWidthPortrait();
        int BCK_PORTRAIT_IMAGE_HEIGHT = ImageHeightPortrait();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOriginal"></param>
        public LocationBackImage()
        {
            _image = pCreateDefaultImage();
        }

        public LocationBackImage(ImageOrientation inImgOrientation)
        {
            _imgOrientation = inImgOrientation;
            _image = pCreateDefaultImage();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOriginal"></param>
        public LocationBackImage(Image inOriginal)
        {
            _image = pCreateImage(inOriginal);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        public LocationBackImage(string inFileName)
        {
            _image = pCreateImage(inFileName);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStream"></param>
        public LocationBackImage(Stream inStream)
        {
            _image = pCreateImage(inStream);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inByteArray"></param>
        public LocationBackImage(byte[] inByteArray)
        {
            _image = pCreateImage(inByteArray);
        }

        #region Static

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static int ImageWidthLandscape()
        {
            return 800;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static int ImageHeightLandscape()
        {
            return 600;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static int ImageWidthPortrait()
        {
            return 600;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static int ImageHeightPortrait()
        {
            return 800;
        }

        #endregion

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public Image BackImage
        {
            get
            {
                return _image;
            }
            set
            {
                pSetBackImage(value);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public ImageOrientation Orientation
        {
            get
            {
                return _imgOrientation;
            }
            set
            {
                pSetImageOrientation(value);
            }
        }

        #endregion

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImg"></param>
        private void pSetBackImage(Image inImg)
        {
            _image = pCreateImage(inImg);

            GC.Collect();
        }

        /// <summary>
        /// 
        /// </summary>
        public void pSetImageOrientation(ImageOrientation inOrientation)
        {
            _imgOrientation = inOrientation;
            _image = pCreateImage(_image);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImage"></param>
        /// <returns></returns>
        private Image pCreateImage(Image inImage)
        {
            try
            {
                int newWidth  = pCalculateWidth(_imgOrientation);
                int newHeight = pCalculateHeight(_imgOrientation);

                Bitmap newImage = new Bitmap(newWidth, newHeight);
                Graphics gr = Graphics.FromImage(newImage);

                gr.SmoothingMode = SmoothingMode.AntiAlias;
                gr.InterpolationMode = InterpolationMode.HighQualityBicubic;
                gr.PixelOffsetMode = PixelOffsetMode.HighQuality;

                gr.DrawImage(inImage, new Rectangle(0, 0, newWidth, newHeight));  

                return newImage;

                //Size newSize = new Size(pCalculateWidth(_imgOrientation), pCalculateHeight(_imgOrientation));
                //return new Bitmap(inImage, newSize);
            }
            catch
            {
                return pCreateDefaultImage();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImageName"></param>
        /// <returns></returns>
        private Image pCreateImage(string inImageName)
        {
            try
            {
                return Image.FromFile(inImageName);
            }
            catch
            {
                return pCreateDefaultImage();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inStream"></param>
        /// <returns></returns>
        private Image pCreateImage(Stream inStream)
        {
            try
            {
                return Image.FromStream(inStream);
            }
            catch
            {
                return pCreateDefaultImage();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inByteArray"></param>
        /// <returns></returns>
        private Image pCreateImage(byte[] inByteArray)
        {
            try
            {
                Size newSize = new Size(pCalculateWidth(_imgOrientation), pCalculateHeight(_imgOrientation));
                return new Bitmap(Helper.byteArrayToImage(inByteArray), newSize);
            }
            catch
            {
                return  pCreateDefaultImage();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private Image pCreateDefaultImage()
        {
            return _imgOrientation == ImageOrientation.Portrait ? FloorPlanClient.Properties.Resources.portrait : FloorPlanClient.Properties.Resources.landscape;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrientation"></param>
        private int pCalculateWidth(ImageOrientation inOrientation)
        {
            return inOrientation == ImageOrientation.Portrait ? BCK_PORTRAIT_IMAGE_WIDTH : BCK_LANDSCAPE_IMAGE_WIDTH; 
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrientation"></param>
        private int pCalculateHeight(ImageOrientation inOrientation)
        {
            return inOrientation == ImageOrientation.Portrait ? BCK_PORTRAIT_IMAGE_HEIGHT : BCK_LANDSCAPE_IMAGE_HEIGHT;
        }

        #endregion
    }
}
