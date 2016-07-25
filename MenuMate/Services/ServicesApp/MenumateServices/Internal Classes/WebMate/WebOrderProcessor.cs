using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Timers;

using MenumateServices.WebMate.DTO;

namespace MenumateServices.WebMate.InternalClasses
{
    public class WebOrderProcessor : WebOrderResponsive
    {
        WebOrderURLManager   web_order_url_manager;
        WebOrderXMLManager   web_order_xml_manager;
        WebOrderFileSupplier web_order_file_supplier;

        WebOrderDBManager web_order_db_manager;

        List<string>  urls_to_be_watched;
        System.Timers.Timer urls_to_be_watched_timer;

        /// <summary>
        /// 
        /// </summary>
        internal WebOrderProcessor()
        {
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

            internal static readonly WebOrderProcessor instance = new WebOrderProcessor();
        }

        #region Public

        /// <summary>
        /// 
        /// </summary>
        public static WebOrderProcessor Instance
        {
            get { return Nested.instance; }
        }

        /// <summary>
        /// 
        /// </summary>
        public void Init()
        {
            web_order_url_manager = WebOrderURLManager.Instance;

            web_order_xml_manager = new WebOrderXMLManager();
            web_order_xml_manager.Init();

            web_order_file_supplier = WebOrderFileSupplier.Instance;
            web_order_file_supplier.Init();
            web_order_file_supplier.WebOrderFileSupplied += webOrderFileSupplied;

            web_order_db_manager = new WebOrderDBManager();

            startMonitoringURL();

            urls_to_be_watched = new List<string>();
            urls_to_be_watched_timer = new System.Timers.Timer(1000); // 1 sec
            urls_to_be_watched_timer.Elapsed += onUrlsToBeWatchedEvent;
        }

        /// <summary>
        /// 
        /// </summary>
        public void Done()
        {
            stopMonitoringURL();
            web_order_db_manager.Done();
        }

        private static Object lock_object = new Object();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inURL"></param>
        /// <param name="inWebOrder"></param>
        public void NotifyCompleteOrderURLCreated(string inURL)
        {
            // Lock the url list to prevent multi thread conflicts from happening
            lock(lock_object)
            {
                urls_to_be_watched.Add(inURL);

                if(!urls_to_be_watched_timer.Enabled)
                {
                    urls_to_be_watched_timer.Enabled = true;
                }
            }
        }

        #endregion

        #region PROTECTED

        /// <summary>
        /// 
        /// </summary>
        protected void startMonitoringURL()
        {
            string completeOrderURL = web_order_url_manager.CompleteOrderURL;

            try
            {
                string[] alldirs = System.IO.Directory.GetDirectories(completeOrderURL, @"*.*", System.IO.SearchOption.AllDirectories);

                foreach (string dir in alldirs)
                {
                    string[] dirs = System.IO.Directory.GetDirectories(dir, @"*.*", System.IO.SearchOption.AllDirectories);

                    if (dirs.Length > 0 && !dir.Contains("Cache"))
                    {
                        web_order_file_supplier.AddWatchedFolders(dirs);
                    }
                }
               
                web_order_file_supplier.Start(); 
            }
            catch (Exception exc)
            {
                ServiceLogger.Log(string.Format(@"Failed to start monitoring complete order's urls: {0}   :{1}", completeOrderURL, exc.Message));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        protected void stopMonitoringURL()
        {
            web_order_file_supplier.Stop();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        protected void onUrlsToBeWatchedEvent(Object source, ElapsedEventArgs e)
        {
            System.Timers.Timer timer = (System.Timers.Timer)source;
            timer.Enabled = false;

            urls_to_be_watched.Sort();
            foreach(string url in urls_to_be_watched)
            {
                object handle = web_order_file_supplier.AddWatchedFolder(url);
                web_order_file_supplier.StartSupplier(handle);
            }

            urls_to_be_watched.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="fileURI"></param>
        protected void webOrderFileSupplied(object sender, string fileURI)
        {
            if (!web_order_db_manager.DeadLockPaused)
            {
                processOrderFile(fileURI);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="fileURI"></param>
        protected void processOrderFile(string inFileURI)
        {            
            bool added = false;
            WebOrderDBAccessProcess process = web_order_db_manager.AddProcess(out added);            

            //....................................................

            WebOrder webOrder;
            string orderHandle = readOrderFromFile(inFileURI, out webOrder);

            process.OrderURI    = inFileURI;
            process.WebOrder    = webOrder;

            process.HasCompleted += dbAccessProcessComplete;
            process.HasFailed    += dbAccessProcessFailed;
            process.DeadLock     += dbAccessDeadLock;

            //....................................................

            removeCompleteOrder( process.WebOrder );
            
            System.Reflection.MethodBase currentMethod = System.Reflection.MethodBase.GetCurrentMethod();
            ServiceLogger.LogWithMethod(currentMethod, @": Complete order file has been removed from folder");

            //....................................................

            if (added)
            {
                web_order_db_manager.StartProcess(process);
            }
            else
            {
                web_order_db_manager.AddProcessToPoll(process);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileURI"></param>
        /// <param name="webOrder"></param>
        /// <returns></returns>
        protected string readOrderFromFile( string inFileURI, out WebOrder outWebOrder)
        {
            return web_order_xml_manager.ReadCompletOrderFromFile(inFileURI, out outWebOrder);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        protected void removeCompleteOrder(WebOrder inWebOrder)
        {
            web_order_xml_manager.RemoveComplete(inWebOrder);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        protected void removeIncompleteOrder(WebOrder inWebOrder)
        {
            web_order_xml_manager.RemoveIncomplete(inWebOrder);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        protected void dbAccessProcessComplete(object sender, WebOrderDBAccessEventArgs args)
        {
            WebOrderDBAccessProcess process = (WebOrderDBAccessProcess)sender;

            System.Reflection.MethodBase currentMethod = System.Reflection.MethodBase.GetCurrentMethod();
            ServiceLogger.LogWithMethod(currentMethod, string.Format(@": Completing order: {0}", process.WebOrder.Handle));

            moveToSucessfulOrder(process.WebOrder);
            ServiceLogger.LogWithMethod(currentMethod, @": Complete order file has been moved from succesful folder");

            web_order_db_manager.TerminateProcess(process);

             ServiceLogger.LogWithMethod(currentMethod, string.Format(@": Order {0} has been completed", process.WebOrder.Handle));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        protected void dbAccessProcessFailed(object sender, WebOrderDBAccessEventArgs args)
        {
            WebOrderDBAccessProcess process = (WebOrderDBAccessProcess)sender;

            System.Reflection.MethodBase currentMethod = System.Reflection.MethodBase.GetCurrentMethod();
            ServiceLogger.LogWithMethod(currentMethod, string.Format(@": Finishing failed order: {0}", process.WebOrder.Handle));

            ServiceLogger.Log(String.Format("Failed to load XML order:{0}. {1}", process.OrderURI, args.ErrorMessage));

            moveToFailedOrder(process.WebOrder, args.ErrorMessage);
            ServiceLogger.LogWithMethod(currentMethod, @": Complete order file has been moved from failed folder");

            web_order_db_manager.TerminateProcess(process);

            ServiceLogger.LogWithMethod(currentMethod, string.Format(@": Failed order {0} has been finished", process.WebOrder.Handle));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        protected void dbAccessDeadLock(object sender, WebOrderDBAccessEventArgs args)
        {
            WebOrderDBAccessProcess process = (WebOrderDBAccessProcess)sender;

            ServiceLogger.Log(String.Format("Failed to load XML order:{0}. {1}", process.OrderURI, args.ErrorMessage));

            web_order_db_manager.SetDeadLockPause();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inProcess"></param>
        protected void moveToSucessfulOrder(WebOrder inWebOrder)
        {
            web_order_xml_manager.MoveOrderToSuccesfulFolder(inWebOrder);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrder"></param>
        /// <param name="inMessage"></param>
        protected void moveToFailedOrder(WebOrder inWebOrder, string inMessage)
        {
            web_order_xml_manager.MoveOrderToFailedFolder(inWebOrder, inMessage);
        }

        #endregion
    }
}
