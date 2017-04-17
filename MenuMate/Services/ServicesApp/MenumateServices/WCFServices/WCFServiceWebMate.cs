using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Diagnostics;
using MenumateServices.WebMate.DTO;
using MenumateServices.WebMate.InternalClasses;
using System.IO;
using Newtonsoft.Json;

/// <summary>
/// Classess in this file:
///     WCFServiceWebMate: WCF Service that implements WebMate Service
/// </summary>
namespace MenumateServices.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WCFServiceWebMate" in both code and config file together.
    public class WCFServiceWebMate : IWCFServiceWebMate
    {
        #region PUBLIC
        /// <summary>
        /// 
        /// </summary>
        /// <param name="outHandle"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse OpenWebOrder(out string outHandle)
        {
            try
            {
                WebOrder webOrder = openWebOrder();
                outHandle = webOrder.Handle;

                // WebOrder successfuly open
                return createResponseNoError();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In OpenWebOrder WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 42, short.MaxValue);
                outHandle = Guid.Empty.ToString();

                // WebOrder: Failed to create an order
                return createResponseError("Failed to open a weborder", exc.Message, ResponseCode.FailedToOpenOrder);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="outHandle"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse CommitOrder(string inOrderHandle)
        {
            try
            {
                getWebOrder(inOrderHandle).Commit();

                // WebOrder successfuly committed
                return createResponseNoError();
            }
            catch (ExceptionOrderGUIDDuplicate exc)
            {
                EventLog.WriteEntry("In CommitOrder WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 43, short.MaxValue);
                // WebOrder: Failed to commit an order
                return createResponseError("Failed to commit a weborder", exc.Message, ResponseCode.OrderGUIDDuplicate);
            }
            catch (ExceptionOrderEmpty exc)
            {
                EventLog.WriteEntry("In CommitOrder WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 44, short.MaxValue);
                // WebOrder: Failed to commit an order
                return createResponseError("Failed to commit a weborder", exc.Message, ResponseCode.OrderEmpty);
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In CommitOrder WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 45, short.MaxValue);
                // WebOrder: Failed to commit an order
                return createResponseError("Failed to commit a weborder", exc.Message, ResponseCode.FailedToCommitOrder);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="inOrderHeader"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse SaveIncompleteOrderHeaderSection(
                                            string inOrderHandle,
                                            DTO_WebOrderHeader inOrderHeaderSection)
        {
            try
            {
                if ((getWebOrder(inOrderHandle).HeaderSection = inOrderHeaderSection) != null)
                {
                    // WebOrder successfuly open
                    return createResponseNoError();
                }
                else
                {
                    // WebOrder: Failed to create an order
                    return createResponseError(@"Failed to set order's header",
                                               string.Format(@"Invalid order handle [{0}]", inOrderHandle),
                                               ResponseCode.FailedToCreateOrderHeaderSection);
                }
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In SaveIncompleteOrderHeaderSection WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 46, short.MaxValue);
                // WebOrder: Failed to create an order
                return createResponseError(@"Failed to set order's header", exc.Message, ResponseCode.FailedToCreateOrderHeaderSection);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="inOrderFrom"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse SaveIncompleteOrderFromSection(
                                            string inOrderHandle,
                                            DTO_WebOrderFrom inOrderFromSection)
        {
            try
            {
                if ((getWebOrder(inOrderHandle).FromSection = inOrderFromSection) != null)
                {
                    // WebOrder successfuly open
                    return createResponseNoError();
                }
                else
                {
                    // WebOrder: Failed to create an order
                    return createResponseError(@"Failed to set order's from section",
                                               string.Format(@"Invalid order handle [{0}]", inOrderHandle),
                                               ResponseCode.FailedToCreateOrderFromSection);
                }
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In SaveIncompleteOrderFromSection WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 47, short.MaxValue);
                // WebOrder: Failed to create an order
                return createResponseError(@"Failed to set order's from section", exc.Message, ResponseCode.FailedToCreateOrderFromSection);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inWebOrderHandle"></param>
        /// <param name="intOrderAccount"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse SaveIncompleteOrderAccountSection(
                                            string inOrderHandle,
                                            DTO_WebOrderAccount intOrderAccountSection)
        {
            try
            {
                try
                {
                    JsonSerializer serializer = new JsonSerializer();
                    serializer.NullValueHandling = NullValueHandling.Ignore;
                    string fileName = WebOrderURLManager.Instance.CacheOrderURL + @"\" + "WebOrder_" + inOrderHandle + DateTime.Now.ToString("dd-mm-yy HH-MM-ss") + ".txt";
                    using (StreamWriter sw = new StreamWriter(fileName))
                    {
                        using (JsonWriter writer = new JsonTextWriter(sw))
                        {
                            serializer.Serialize(writer, intOrderAccountSection);
                        }
                    }
                }
                catch (Exception exc)
                {
                    EventLog.WriteEntry("In SaveIncompleteOrderAccountSection WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 48, short.MaxValue);
                }

                if ((getWebOrder(inOrderHandle).AccountSection = intOrderAccountSection) != null)
                {
                    // WebOrder successfuly open
                    return createResponseNoError();
                }
                else
                {
                    // WebOrder: Failed to create an order
                    return createResponseError(@"Failed to set order's account section",
                                               string.Format(@"Invalid order handle [{0}]", inOrderHandle),
                                               ResponseCode.FailedToCreateOrderAccountSection);
                }
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In SaveIncompleteOrderAccountSection WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 49, short.MaxValue);
                // WebOrder: Failed to create an order
                return createResponseError(@"Failed to set order's account section", exc.Message, ResponseCode.FailedToCreateOrderAccountSection);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="outHeader"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse RetrieveIncompleteOrderHeaderSection(
                                    string inOrderHandle,
                                out DTO_WebOrderHeader outHeaderSection)
        {
            try
            {
                outHeaderSection = getWebOrder(inOrderHandle).HeaderSection;

                // WebOrder's header successfuly retrieved
                return createResponseNoError();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In RetrieveIncompleteOrderHeaderSection WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 50, short.MaxValue);
                //outHeaderSection = getWebOrder(inOrderHandle).EmptyHeaderSection;
                outHeaderSection = null;

                // WebOrder: Failed to retrieve an order's header section
                return createResponseError(@"Failed to retrieve order's header",
                                           exc.Message,
                                           ResponseCode.FailedToRetrieveOrderHeaderSection);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="outFrom"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse RetrieveIncompleteOrderFromSection(
                                    string inOrderHandle,
                                out DTO_WebOrderFrom outFromSection)
        {
            try
            {
                outFromSection = getWebOrder(inOrderHandle).FromSection;

                // WebOrder's header successfuly retrieved
                return createResponseNoError();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In RetrieveIncompleteOrderFromSection WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 51, short.MaxValue);
                //outFromSection = getWebOrder(inOrderHandle).EmptyFromSection;
                outFromSection = null;

                // WebOrder: Failed to retrieve an order's from section
                return createResponseError(@"Failed to retrieve order's from section",
                                           exc.Message,
                                           ResponseCode.FailedToRetrieveOrderFromSection);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="outHeader"></param>
        /// <returns></returns>
        public DTO_WebOrderResponse RetrieveIncompleteOrderAccountSection(
                                string inOrderHandle,
                                out DTO_WebOrderAccount outAccountSection)
        {
            try
            {
                outAccountSection = getWebOrder(inOrderHandle).AccountSection;

                // WebOrder's header successfuly retrieved
                return createResponseNoError();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In RetrieveIncompleteOrderAccountSection WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 52, short.MaxValue);
                //outAccountSection = getWebOrder(inOrderHandle).EmptyAccountSection;
                outAccountSection = null;

                // WebOrder: Failed to retrieve an order's account section
                return createResponseError(@"Failed to retrieve order's account section",
                                           exc.Message,
                                           ResponseCode.FailedToRetrieveOrderAccountSection);
            }
        }

        public DTO_WebOrderResponse GetCommittedWebOrderStatus(
                                    string inSiteName,
                                    string inStoreName,
                                    string inOrderHandle,
                                out WebOrderStatus outStatus)
        {
            try
            {
                WebOrderCommit webOrderCommit = new WebOrderCommit();

                webOrderCommit.GetCommittedWebOrderStatus(
                                      inSiteName,
                                      inStoreName,
                                      inOrderHandle,
                                  out outStatus);

                // WebOrder's header successfuly retrieved
                return createResponseNoError();
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In GetCommittedWebOrderStatus WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 53, short.MaxValue);
                outStatus = WebOrderStatus.Unknown;

                // WebOrder: Failed to retrieve an order's account section
                return createResponseError(@"Failed to get committed order's status",
                                           exc.Message,
                                           ResponseCode.FailedToGetCommittedOrderStatus);
            }
        }
        #endregion

        #region PRIVATE
        /// <summary>
        /// Creates an empty web order
        /// Creates a WebOrder object
        /// </summary>
        /// <returns></returns>
        WebOrder createWebOrder()
        {
            return new WebOrder();
        }

        /// <summary>
        /// Creates an empty web order
        /// Creates a WebOrder object and opens its related file 
        /// </summary>
        /// <returns></returns>
        WebOrder openWebOrder()
        {
            try
            {
                WebOrder result = createWebOrder();
                result.Open();

                return result;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In openWebOrder WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 54, short.MaxValue);
            }
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        WebOrder getWebOrder(string inOrderHandle)
        {
            WebOrder order = findWebOrder(inOrderHandle);

            if (order != null)
            {
                return order;
            }

            EventLog.WriteEntry("In getWebOrder WebMate", "Invalid order handle" + "Trace" + "Invalid order handle", EventLogEntryType.Error, 56, short.MaxValue);
            throw new Exception(@"Invalid order handle");
        }

        /// <summary>
        /// NOT USED SO FAR!!!
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        WebOrder popWebOrder(string inOrderHandle)
        {
            WebOrder order = findWebOrder(inOrderHandle);

            if (order != null)
            {
                removeIncompleteOrder(order);
                return order;
            }
            EventLog.WriteEntry("In popWebOrder WebMate", "Invalid order handle" + "Trace" + "Invalid order handle", EventLogEntryType.Error, 57, short.MaxValue);
            throw new Exception(@"Invalid order handle");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        WebOrder findWebOrder(string inOrderHandle)
        {
            return readIncompleteOrder(inOrderHandle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <returns></returns>
        WebOrder readIncompleteOrder(string inOrderHandle)
        {
            WebOrder result = createWebOrder();

            result.Load(inOrderHandle);

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrder"></param>
        void removeIncompleteOrder(WebOrder inOrder)
        {
            inOrder.Remove();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        protected DTO_WebOrderResponse createResponseNoError()
        {
            return createResponse(true, "", "", ResponseCode.Successful);
        }

        /// <summary>
        /// Creates a Web Order's Error response object  
        /// </summary>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        DTO_WebOrderResponse createResponseError(
                                    string inMessage,
                                    string inDescription,
                                ResponseCode inResponseCode)
        {
            return createResponse(false, inMessage, inDescription, inResponseCode);
        }

        /// <summary>
        /// Creates a Web Order's response object  
        /// </summary>
        /// <param name="inSuccesful"></param>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        DTO_WebOrderResponse createResponse(
                                bool inSuccesful,
                                string inMessage,
                                string inDescription,
                                ResponseCode inResponseCode)
        {
            DTO_WebOrderResponse result = new DTO_WebOrderResponse();

            //..................................................

            try
            {
                result.Succesful = inSuccesful;
                result.Message = inMessage;
                result.Description = inDescription;
                result.Response = inResponseCode;
            }
            catch (Exception exc)
            {
                EventLog.WriteEntry("In createResponse WebMate", exc.Message + "Trace" + exc.StackTrace, EventLogEntryType.Error, 55, short.MaxValue);
            }

            //..................................................

            return result;
        }
        #endregion
    }
}
