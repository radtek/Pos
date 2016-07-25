using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Drawing;

namespace MMChangingImageController.EventArgs
{
    public class ChangingImageEventArgs : System.EventArgs
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderItemList"></param>
        public ChangingImageEventArgs(Image inImage) : base()
        {
            showing_image = inImage;
        }

        /// <summary>
        /// 
        /// </summary>
        public Image ShowingImage 
        {
            get { return showing_image; }
        }

        #endregion

        #region PROTECTED

        Image showing_image;

        #endregion
    }
}

