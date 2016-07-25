using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using MMChangingImageController;
using MMChangingImageController.EventArgs;

namespace MMChangingImageManager
{
    
    public class ChangingImageManager
    {
        #region PUBLIC

        public enum ImageManagerType { Folder };

        /// <summary>
        /// /
        /// </summary>
        public ChangingImageManager()
        {
            controllers = new List<IChangingImageController>();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        public void SetRefreshInterval(Int32 inInterval, ImageManagerType inManagerType)
        {
            getController(inManagerType).SetRefreshInterval(inInterval);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inHandler"></param>
        public void SetImageChangedHandler(EventHandler<ChangingImageEventArgs> inHandler, ImageManagerType inManagerType)
        {
            getController(inManagerType).ImageChanged += inHandler;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        public void StartRefresh(ImageManagerType inManagerType)
        {
            getController(inManagerType).StartRefresh();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        public void StopRefresh(ImageManagerType inManagerType)
        {
            getController(inManagerType).StopRefresh();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPath"></param>
        /// <param name="inManagerType"></param>
        public void SetImagesPath(string inPath, ImageManagerType inManagerType)
        {
            getController(inManagerType).SetImagesPath(inPath);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        public void UpdateImages(ImageManagerType inManagerType)
        {
            getController(inManagerType).UpdateImages();
        }

        #endregion

        #region PROTECTED

        List<IChangingImageController> controllers;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        /// <returns></returns>
        protected IChangingImageController getController(ImageManagerType inManagerType)
        {
            int index = (int)inManagerType;

            try
            {
                return controllers[index];
            }
            catch
            {
                return createController(inManagerType);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inManagerType"></param>
        /// <returns></returns>
        protected IChangingImageController createController(ImageManagerType inManagerType)
        {
            IChangingImageController result;

            switch (inManagerType)
            {
                case ImageManagerType.Folder: result = new TimerChangingImageController(); break;

                default: result = new TimerChangingImageController(); break;
            }

            controllers.Add(result);

            return result;
        }

        #endregion
    }
}
