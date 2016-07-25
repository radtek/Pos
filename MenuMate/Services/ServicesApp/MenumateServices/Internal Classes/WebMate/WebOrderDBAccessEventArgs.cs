using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MenumateServices.WebMate.InternalClasses
{
    public enum WebOrderDBAccessResult
    {
        Succesful,
        FailedToAccess,
        DeadLock
    };

    public class WebOrderDBAccessEventArgs : EventArgs
    {
        #region PUBLIC

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileURI"></param>
        /// <param name="inResult"></param>
        public WebOrderDBAccessEventArgs(WebOrderDBAccessResult inResult, string inMessage) : base()
        {
            access_result = inResult;
            error_message = inMessage;
        }
 
        /// <summary>
        /// 
        /// </summary>
        public string ErrorMessage
        {
            get { return error_message; }
        }     
  
        /// <summary>
        /// 
        /// </summary>
        public WebOrderDBAccessResult Result
        {
            get { return access_result; }
        }
        
        #endregion

        #region PRIVATE

        WebOrderDBAccessResult access_result;
        string error_message;

        #endregion
    }
}
