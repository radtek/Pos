using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using MenumateServices.WebMate.DTO;

namespace MenumateServices.WebMate.InternalClasses
{
    /// <summary>
    /// Creates WebOrders responses. Classes with functions that return WebOrder Responses must derive from this one
    /// </summary>
    public class WebOrderResponsive
    {
        #region Protected
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected DTO_WebOrderResponse createResponseNoError()
        {
            return createResponse(true, "", "", ResponseCode.Successful);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        protected DTO_WebOrderResponse createResponseError(
                                                                string inMessage,
                                                                string inDescription,
                                                                ResponseCode inResponseCode)
        {
            return createResponse(false, inMessage, inDescription, inResponseCode);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSuccesful"></param>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        protected DTO_WebOrderResponse createResponse(
                                            bool inSuccesful,
                                            string inMessage,
                                            string inDescription,
                                            ResponseCode inResponseCode)
        {
            DTO_WebOrderResponse result = new DTO_WebOrderResponse();

            //..................................................

            result.Succesful = inSuccesful;
            result.Message = inMessage;
            result.Description = inDescription;
            result.Response = inResponseCode;

            //..................................................

            return result;
        }
        #endregion
    }
}
