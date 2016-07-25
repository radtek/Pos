using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

namespace MenumateServices.WebMate.InternalClasses
{
    public class WebOrderURLManager
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        public static WebOrderURLManager Instance
        {
            get { return Nested.instance; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string OrdersRootURL
        {
            get { return order_url; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string IncompleteOrderURL
        {
            get { return incomplete_order_url; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string CompleteOrderURL
        {
            get { return complete_order_url; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string SuccessfulOrderURL
        {
            get { return successful_order_url; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string FailedOrderURL
        {
            get { return failed_order_url; }
        }

        public string CacheOrderURL
        {
            get { return cache_order_url; }
        }


        /// <summary>
        /// 
        /// </summary>
        public void CheckOrderURLs()
        {
            bool justCreated;

            CheckURL(order_url, out justCreated);
            CheckURL(incomplete_order_url, out justCreated);
            CheckURL(complete_order_url, out justCreated);

            CheckURL(successful_order_url, out justCreated);
            CheckURL(failed_order_url, out justCreated);
            CheckURL(cache_order_url, out justCreated);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inURL"></param>
        public void CheckURL(string inURL, out bool outJustCreated)
        {
            outJustCreated = false;

            try
            {
                if (!Directory.Exists(inURL))
                {
                    Directory.CreateDirectory(inURL);

                    outJustCreated = true; ;
                }
            }
            catch
            {
            }
        }

        /// <summary>
        /// NO LONGER USED!!!!!
        /// </summary>
        /// <returns></returns>
        public string[] PopNewUrlList()
        {
            return popAllURLs();
        }

        #endregion

        #region PROTECTED

        /// <summary>
        /// 
        /// </summary>
        internal WebOrderURLManager()
        {
            initURLs();
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

            internal static readonly WebOrderURLManager instance = new WebOrderURLManager();
        }

        string order_url = @"";
        string incomplete_order_url = @"";
        string complete_order_url   = @"";

        string successful_order_url = @"";
        string failed_order_url     = @"";
        string cache_order_url = @"";
        List<string> new_url_list;

        /// <summary>
        /// 
        /// </summary>
        protected void initURLs()
        {
            string baseDir = ServiceInfo.Instance.StartPath;

            order_url            = string.Format(@"{0}\{1}", baseDir, MenumateServices.Properties.Settings.Default.WebOrderURL);
            incomplete_order_url = string.Format(@"{0}\Incomplete", order_url);
            complete_order_url   = string.Format(@"{0}\Complete",   order_url);

            successful_order_url = string.Format(@"{0}\Successful", order_url);
            failed_order_url     = string.Format(@"{0}\Failed", order_url);
            cache_order_url = string.Format(@"{0}\Cache", order_url);
            new_url_list = new List<string>();
        }

        /// <summary>
        /// NOT LONGER USED!!!!
        /// </summary>
        /// <param name="inURL"></param>
        protected void pushNewURL(string inURL)
        {
            lock(new_url_list)
            {
                new_url_list.Add(inURL);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string[] popAllURLs()
        {
            string[] result;

            //:::::::::::::::::::::::::::::::

            lock(new_url_list)
            {
                result = new_url_list.ToArray();

                new_url_list.Clear();
            }

            //:::::::::::::::::::::::::::::::

            return result;
        }                    
 
        #endregion
    }
}
