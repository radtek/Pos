using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using MMChangingImageController.EventArgs;

namespace MMChangingImageController
{
    public interface IChangingImageController
    {
        /// <summary>
        /// 
        /// </summary>
        EventHandler<ChangingImageEventArgs> ImageChanged
        {
            get;
            set;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        void SetRefreshInterval(Int32 inInterval);

        /// <summary>
        /// 
        /// </summary>
        void StartRefresh();

        /// <summary>
        /// 
        /// </summary>
        void StopRefresh();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPath"></param>
       void SetImagesPath(string inPath);

        /// <summary>
        /// 
        /// </summary>
        void UpdateImages();
    }
}
