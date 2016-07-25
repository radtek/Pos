using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Timers;
using System.Drawing;
using System.IO;

using MMChangingImageController.EventArgs;
using MMChangingImageController.Exceptions;

namespace MMChangingImageController
{
    public class TimerChangingImageController : IChangingImageController
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        public TimerChangingImageController()
        {
            resetImageIndex();
            image_list = new List<string>();
            //image_list = new List<Image>();

            empty_list_detected = false;

            //::::::::::::::::::::::::::::::::::

            SetImagesPath(startPath());
            Directory.CreateDirectory(images_url);

            //::::::::::::::::::::::::::::::::::
            initTimer();
        }

        /// <summary>
        /// 
        /// </summary>
        public EventHandler<ChangingImageEventArgs> ImageChanged
        {
            get { return image_changed;  }
            set { image_changed = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inInterval"></param>
        public void SetRefreshInterval(Int32 inInterval)
        {
            changing_image_timer.Interval = inInterval;
        }

        /// <summary>
        /// 
        /// </summary>
        public void StartRefresh()
        {
            startTimer(true);
        }

        /// <summary>
        /// 
        /// </summary>
        public void StopRefresh()
        {
            startTimer(false);
            empty_list_detected = false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPath"></param>
        public void SetImagesPath(string inPath)
        {
            images_url = inPath;
        }

        /// <summary>
        /// 
        /// </summary>
        public void UpdateImages()
        {
            updateImageList();

            if( empty_list_detected )
            {
                StartRefresh();
                empty_list_detected = false;
            } 
        }

        #endregion

        #region PROTECTED

        int DEFAULT_IMAGE_INTERVAL = 5; 

          int changing_image_interval;
        Timer changing_image_timer;
        EventHandler<ChangingImageEventArgs> image_changed;

        string images_url;
        Int16 current_image_index;
        List<string> image_list;
        //List<Image> image_list;
        bool empty_list_detected;

        /// <summary>
        /// 
        /// </summary>
        protected void resetImageIndex()
        {
            current_image_index = 0;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected string startPath()
        {
            string path = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase).Substring(6);

            return path + @"\images";
        }

        /// <summary>
        /// 
        /// </summary>
        protected void initTimer()
        {
            changing_image_interval = DEFAULT_IMAGE_INTERVAL;

            // Create a timer with a 1 second interval.
            changing_image_timer = new System.Timers.Timer(changing_image_interval);

            // Hook up the Elapsed event for the timer. 
            changing_image_timer.Elapsed += OnChangingImageTimedEvent;

            StopRefresh();
        }

        /// <summary>
        /// 
        /// </summary>
        protected void startTimer(bool inStart)
        {
            if(inStart)
            {
                changing_image_timer.Interval = changing_image_interval;
            }

            changing_image_timer.Enabled = inStart;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected bool isTimerEnabled()
        {
            return changing_image_timer.Enabled;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void OnChangingImageTimedEvent(Object source, ElapsedEventArgs e)
        {
            Timer timer = (Timer)source;
            timer.Enabled = false;

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                Image currentImage = getNextImage();

                ChangingImageEventArgs eventArgs = createChangingImageEventArgs(currentImage);
                OnImageChanged(eventArgs);
                updateTimerInterval();
            }
            catch(ExceptionImageListEmpty exc)
            {
                timer.Enabled = false;
                empty_list_detected = true;

                return;
            }
            catch
            {
            }

            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

            timer.Enabled = true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected Image getNextImage()
        {
            string fileName;

            //::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                fileName = image_list[current_image_index++];
                //return image_list[current_image_index++];
            }
            catch
            {
                current_image_index = 0;
                if (image_list.Count > 0)
                {
                    fileName = image_list[current_image_index++];
                    //return image_list[current_image_index++];
                }
                else
                {
                    throw new ExceptionImageListEmpty(@"No image to show");
                }
            }

            //::::::::::::::::::::::::::::::::::::::::::::::

            try
            {
                return Image.FromFile(fileName);
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImmage"></param>
        /// <returns></returns>
        protected ChangingImageEventArgs createChangingImageEventArgs(Image inImmage)
        {
            ChangingImageEventArgs result = new ChangingImageEventArgs(inImmage);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inEventArgs"></param>
        protected void OnImageChanged(ChangingImageEventArgs inEventArgs)
        {
            EventHandler<ChangingImageEventArgs> handler = ImageChanged;

            if(handler != null)
            {
                handler(this, inEventArgs);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        void updateImageList()
        {
            bool timerEnabled = isTimerEnabled();

            StopRefresh();

            loadImagesFromFolder(images_url);
            resetImageIndex();

            if(timerEnabled)
            {
                StartRefresh();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inImageURL"></param>
        void loadImagesFromFolder(string inImageURL)
        {
            image_list.Clear();

            string[] fileNames = Directory.GetFiles(inImageURL);

            foreach(string fileName in fileNames)
            {
                image_list.Add(fileName);
                //image_list.Add(Image.FromFile(fileName));
            }
        }
       
        void updateTimerInterval()
        {           
            changing_image_timer.Elapsed -= OnChangingImageTimedEvent;

            changing_image_interval = 5000;
            // Create a timer with a 1 second interval.
            changing_image_timer = new System.Timers.Timer(changing_image_interval);

            // Hook up the Elapsed event for the timer. 
            changing_image_timer.Elapsed += OnChangingImageTimedEvent;

            StartRefresh();
   
        }

        #endregion

        #region PRIVATE
        #endregion
    }
}
