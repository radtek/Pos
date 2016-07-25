using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using WebMateTestProgram.WebMateServiceReference;

namespace WebMateTestProgram
{
    public class WebmateClientInterface
    {
        #region PUBLIC SECTION
        /// <summary>
        /// 
        /// </summary>
        public static WebmateClientInterface Instance 
        {
            get { return Nested.instance; } 
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="endpointConfigurationName"></param>
        /// <param name="remoteAddress"></param>
        public void Reset(string endpointConfigurationName, string remoteAddress)
        {                 
            webmateClient_ = new WCFServiceWebMateClient(
                                    endpointConfigurationName,
                                    remoteAddress);
        }

        /// <summary>
        /// 
        /// </summary>
        public string OpenOrder()
        {
            string result = @"";

            //.............................................

            try
            {
                openOrder(out result);
            }
            catch(Exception exc)
            {
                 errorMs_ = exc.Message;

                 throw;
            }

            //..............................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        public void CommitOrder(string inOrderHandle)
        {
            try
            {
                commitOrder(inOrderHandle);
            }
            catch (Exception exc)
            {
                errorMs_ = exc.Message;

                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inSiteName"></param>
        /// <param name="inOrderHandle"></param>
        /// <param name="outStatus"></param>
        public void GetCommittedOrderStatus(
                          string inSiteName,
                          string inStoreName,
                          string inOrderHandle,
                      out WebOrderStatus outStatus)
        {
            try
            {
                getCommittedOrderStatus(
                          inSiteName,
                          inStoreName,
                          inOrderHandle,
                      out outStatus);
            }
            catch (Exception exc)
            {
                errorMs_ = exc.Message;

                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="inOrderGUID"></param>
        /// <param name="inOrderTotal"></param>
        /// <param name="inOrderTime"></param>
        /// <param name="inExpectedTime"></param>
        /// <param name="inScheduled"></param>
        /// <param name="inSiteName"></param>
        public void SaveHeaderSection(
                        string inOrderHandle,           // Incomplete Order Handle
                        string inOrderGUID,             // Order GUID
                        decimal inOrderTotal,           // Order Total
                        DateTime inOrderTime,           // Order Date
                        DateTime inExpectedTime,        // Expected Date
                        UInt16 inScheduled,             // Scheduled
                        string inStoreName,             // Store Name
                        bool inRespond = false)   // PaymentRequired      
        {
            try
            {
                saveHeaderSection(
                        inOrderHandle, 
                        inOrderGUID,      
                        inOrderTotal,    
                        inOrderTime,    
                        inExpectedTime, 
                        inScheduled,      
                        inStoreName,
                        inRespond);
            }
            catch (Exception exc)
            {
                errorMs_ = exc.Message;

                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="inSiteGUID"></param>
        public void SaveFromSection(
                        string inOrderHandle, // Incomplete Order Handle
                        string inSiteGUID,   // Store GUID
                        string inSiteName)   // Store Name
        {
            try
            {
                saveFromSection(
                        inOrderHandle,
                        inSiteGUID,
                        inSiteName);
            }
            catch (Exception exc)
            {
                errorMs_ = exc.Message;

                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="inCovers"></param>
        /// <param name="inMemberNumber"></param>
        /// <param name="inMemberNamer"></param>
        /// <param name="inComments"></param>
        /// <param name="inDeliverySection"></param>
        /// <param name="inPaymentsSection"></param>
        /// <param name="inOrderItems"></param>
        public void SaveAccountSection(
                        string inOrderHandle,    // Incomplete Order Handle
                        UInt32 inCovers,         // Covers
                        string inMemberNumber,   // Member Number
                        string inMemberNamer,    // Member Name
                        List<string> inComments, // Comments
                        DTO_WebOrderDelivery   inDeliverySection, // Delivery Section          
                        DTO_WebOrderPayments   inPaymentsSection, // Payments Section,
                        List<DTO_WebOrderItem> inOrderItems)      // Order Items 
        {
            try
            {
                saveOrderAccountSection(
                    inOrderHandle,
                    inCovers,
                    inMemberNumber,
                    inMemberNamer,
                    inComments,
                    inDeliverySection,
                    inPaymentsSection,
                    inOrderItems);             
            }
            catch (Exception exc)
            {
                errorMs_ = exc.Message;

                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="incompleteHeader"></param>
        public void RetrieveIncompleteHeaderSection(
                                                string inOrderHandle,
                                                out DTO_WebOrderHeader incompleteHeader)
        {
            try
            {
                retrieveIncompleteHeader(inOrderHandle, out incompleteHeader);
            }
            catch (Exception ex)
            {
                errorMs_ = ex.Message;
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="incompleteFrom"></param>
        public void RetrieveIncompleteFromSection(
                                                string inOrderHandle,
                                                out DTO_WebOrderFrom incompleteFrom)
        {
            try
            {
                retrieveIncompleteFrom(inOrderHandle, out incompleteFrom);
            }
            catch (Exception ex)
            {
                errorMs_ = ex.Message;
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="incompleteAccount"></param>
        public void RetrieveIncompleteAccountSection(
                                        string inOrderHandle,
                                        out DTO_WebOrderAccount incompleteAccount)
        {
            try
            {
                retrieveIncompleteAccount(inOrderHandle, out incompleteAccount);
            }
            catch (Exception ex)
            {
                errorMs_ = ex.Message;

                throw;
            }
        }        

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inRecipient"></param>
        /// <param name="inStreetNo"></param>
        /// <param name="inStreetName"></param>
        /// <param name="inSuburb"></param>
        /// <param name="inCity"></param>
        /// <param name="inCountry"></param>
        /// <param name="inPhoneNo"></param>
        /// <returns></returns>
        public DTO_WebOrderDelivery CreateAccountDelivery(
                                    string inRecipient,
                                    string inStreetNo,
                                    string inStreetName,
                                    string inSuburb,
                                    string inCity,
                                    string inCountry,
                                    string inPhoneNo,
                                    string inNote)
        {
            DTO_WebOrderDelivery result = new DTO_WebOrderDelivery();

            //......................................................

            result.Recipient = inRecipient;
            result.StreetNo = inStreetNo;
            result.StreetName = inStreetName;
            result.Suburb = inSuburb;
            result.City = inCity;
            result.Country = inCountry;
            result.Phone = inPhoneNo;
            result.Note = inNote;

            //......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inPaymentRequired"></param>
        /// <returns></returns>
        public DTO_WebOrderPayments CreateAccountPayments(bool inPaymentRequired)
        {
            DTO_WebOrderPayments result = new DTO_WebOrderPayments();

            //......................................................

            result.PaymentRequired = inPaymentRequired;

            //......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDiscountID"></param>
        /// <param name="inGUID"></param>
        /// <param name="inQty"></param>
        /// <param name="inBasePrice"></param>
        /// <returns></returns>
        public DTO_WebOrderItem CreateAccountOrderItem(
                                uint    inDiscountID,
                                string  inGUID,
                                float   inQty,
                                decimal inBasePrice,
                                List<DTO_WebOrderItemOption> inItemOptions,
                                List<DTO_WebOrderItemSide>   inItemSides)
        {
            DTO_WebOrderItem result = new DTO_WebOrderItem();

            //......................................................

            result.DiscountID  = inDiscountID;
            result.UID         = inGUID;
            result.Qty         = inQty;
            result.BasePrice   = inBasePrice;
            result.ItemOptions = inItemOptions.ToArray();
            result.ItemSides = inItemSides.ToArray();

            //......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDiscountID"></param>
        /// <param name="inGUID"></param>
        /// <param name="inQty"></param>
        /// <param name="inBasePrice"></param>
        /// <returns></returns>
        public DTO_WebOrderItemOption CreateAccountOrderItemOption(
                        uint    inDiscountID,
                        string  inGUID,
                        float   inQty,
                        decimal inBasePrice)
        {
            DTO_WebOrderItemOption result = new DTO_WebOrderItemOption();

            //......................................................

            result.DiscountID = inDiscountID;
            result.UID        = inGUID;
            result.Qty        = inQty;
            result.BasePrice  = inBasePrice;
            //......................................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inDiscountID"></param>
        /// <param name="inGUID"></param>
        /// <param name="inQty"></param>
        /// <param name="inBasePrice"></param>
        /// <returns></returns>
        public DTO_WebOrderItemSide CreateAccountOrderItemSide(
                uint    inDiscountID,
                string  inGUID,
                float   inQty,
                decimal inBasePrice)
        {
            DTO_WebOrderItemSide result = new DTO_WebOrderItemSide();

            //......................................................

            result.DiscountID = inDiscountID;
            result.UID        = inGUID;
            result.Qty        = inQty;
            result.BasePrice  = inBasePrice;

            //......................................................

            return result;
        }
        #endregion

        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly WebmateClientInterface instance = new WebmateClientInterface();
        }

        WCFServiceWebMateClient webmateClient_ = new WCFServiceWebMateClient();

        string errorMs_ = @"";
        string testOrderHandle_ = @"";
        string orderHandle_ = @"";

        /// <summary>
        /// 
        /// </summary>
        internal WebmateClientInterface()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        void openOrder(out string outOrderHandle)
        {
            DTO_WebOrderResponse response = webmateClient_.OpenWebOrder(out outOrderHandle);

            if (!response.Succesful)
            {
                throw new Exception(string.Format(@"{0}: {1}", response.Message, response.Description));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        void commitOrder(string inOrderHandle)
        {
            DTO_WebOrderResponse response = webmateClient_.CommitOrder(inOrderHandle);

            if (!response.Succesful)
            {
                throw new Exception(string.Format(@"{0}: {1}", response.Message, response.Description));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSiteName"></param>
        /// <param name="inSiteName"></param>
        /// <param name="inOrderHandle"></param>
        /// <param name="outStatus"></param>
        void getCommittedOrderStatus(
                  string inSiteName,
                  string inStoreName,
                  string inOrderHandle,
              out WebOrderStatus outStatus)
        {
            DTO_WebOrderResponse response = webmateClient_.GetCommittedWebOrderStatus(
                                                              out outStatus, 
                                                              inSiteName,
                                                              inStoreName,
                                                              inOrderHandle);

            if (!response.Succesful)
            {
                throw new Exception(string.Format(@"{0}: {1}", response.Message, response.Description));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        void saveHeaderSection(
                 string inOrderHandle,              // Incomplete Order Handle
                 string inOrderGUID,                // Order GUID
                 decimal inOrderTotal,              // Order Total
                 DateTime inOrderTime,              // Order Date
                 DateTime inExpectedTime,           // Expected Date
                 UInt16 inScheduled,                // Scheduled
                 string inStoreName,                // Store Name
                 bool inRespond = false,
                 bool inPaymentRequired = false)      
        {
            DTO_WebOrderHeader   orderHeader = createOrderHeaderSection(
                                                     inOrderGUID,      
                                                     inOrderTotal,    
                                                     inOrderTime,    
                                                     inExpectedTime, 
                                                     inScheduled,      
                                                     inStoreName,
                                                     inRespond);      

            DTO_WebOrderResponse response    = webmateClient_.SaveIncompleteOrderHeaderSection(
                                                                inOrderHandle,
                                                                orderHeader);

            if (!response.Succesful)
            {
                throw new Exception(string.Format(@"{0}: {1}", response.Message, response.Description));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        void saveFromSection(
                  string inOrderHandle, // Incomplete Order Handle
                  string inSiteGUID,   // Website GUID
                  string inSiteName)   // Website Name
        {
            DTO_WebOrderFrom orderFrom = createOrderFromSection(inSiteGUID, inSiteName);
            DTO_WebOrderResponse response  = webmateClient_.SaveIncompleteOrderFromSection(
                                                                inOrderHandle,
                                                                orderFrom);

            if (!response.Succesful)
            {
                throw new Exception(string.Format(@"{0}: {1}", response.Message, response.Description));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        void saveOrderAccountSection(
                 string inOrderHandle,    // Incomplete Order Handle
                 UInt32 inCovers,         // Covers
                 string inMemberNumber,   // Member Number
                 string inMemberNamer,    // Member Name
                 List<string> inComments, // Comments
                 DTO_WebOrderDelivery inDeliverySection, // Delivery Section          
                 DTO_WebOrderPayments inPaymentsSection, // Payments Section,
                 List<DTO_WebOrderItem> inOrderItems)    // Order Items 

        {
            DTO_WebOrderAccount orderAccount = createOrderAccountSection(
                                                      inCovers,          // Covers
                                                      inMemberNumber,    // Member Number
                                                      inMemberNamer,     // Member Name
                                                      inComments,        // Comments
                                                      inDeliverySection, // Delivery Section          
                                                      inPaymentsSection, // Payments Section,
                                                      inOrderItems);     // Order Items 

            DTO_WebOrderResponse response     = webmateClient_.SaveIncompleteOrderAccountSection(
                                                                inOrderHandle,
                                                                orderAccount);

            if (!response.Succesful)
            {
                throw new Exception(string.Format(@"{0}: {1}", response.Message, response.Description));
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderHeader createOrderHeaderSection(
                                 string inOrderGUID,                // Order GUID
                                 decimal inOrderTotal,              // Order Total
                                 DateTime inOrderTime,              // Order Date
                                 DateTime inExpectedTime,           // Expected Date
                                 UInt16 inScheduled,                // Scheduled
                                 string inStoreName,                // Store Name
                                 bool inRespond = false)      
        {
            DTO_WebOrderHeader result = new DTO_WebOrderHeader();

            //.........................................

            result.GUID         = inOrderGUID;
            result.OrderTotal   = inOrderTotal;
            result.OrderDate    = inOrderTime;
            result.ExpectedDate = inExpectedTime;
            result.Scheduled    = inScheduled;
            result.StoreName    = inStoreName;
            result.Respond      = inRespond;

            //.........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderFrom createOrderFromSection(string inStoreGUID, string inStoreName)
        {
            DTO_WebOrderFrom result = new DTO_WebOrderFrom();

            //.........................................

            result.SiteID   = inStoreGUID;
            result.SiteName = inStoreName;

            //.........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        DTO_WebOrderAccount createOrderAccountSection(
                                   UInt32 inCovers,         // Covers
                                   string inMemberNumber,   // Member Number
                                   string inMemberNamer,    // Member Name
                                   List<string> inComments, // Comments
                                   DTO_WebOrderDelivery inDeliverySection, // Delivery Section          
                                   DTO_WebOrderPayments inPaymentsSection, // Payments Section,
                                   List<DTO_WebOrderItem> inOrderItems)    // Order Items 
        {
            DTO_WebOrderAccount result = new DTO_WebOrderAccount();

            //.........................................

            result.Covers       = inCovers;
            result.MemberNumber = inMemberNumber;
            result.Name         = inMemberNamer;

            result.Comments   = inComments.ToArray();
            result.Delivery   = inDeliverySection;
            result.Payments   = inPaymentsSection;
            result.OrderItems = inOrderItems.ToArray();

            //.........................................

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="incompleteAccount"></param>
        private void retrieveIncompleteAccount(string inOrderHandle, out DTO_WebOrderAccount incompleteAccount)
        {
            if (string.IsNullOrEmpty(inOrderHandle))
                throw new Exception("Invalid order handle");

            DTO_WebOrderResponse response = webmateClient_.RetrieveIncompleteOrderAccountSection(
                                                                                 out incompleteAccount,
                                                                                 inOrderHandle);
            if (!response.Succesful)
                throw new Exception(response.Message);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="incompleteFrom"></param>
        private void retrieveIncompleteFrom(string inOrderHandle, out DTO_WebOrderFrom incompleteFrom)
        {
            if (string.IsNullOrEmpty(inOrderHandle))
                throw new Exception("Invalid order handle");

            DTO_WebOrderResponse response = webmateClient_.RetrieveIncompleteOrderFromSection(
                                                                                    out incompleteFrom,
                                                                                    inOrderHandle);
            if (!response.Succesful)
                throw new Exception(response.Message);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inOrderHandle"></param>
        /// <param name="incompleteHeader"></param>
        private void retrieveIncompleteHeader(string inOrderHandle, out DTO_WebOrderHeader incompleteHeader)
        {
            if (string.IsNullOrEmpty(inOrderHandle))
                throw new Exception("Invalid order handle");

            DTO_WebOrderResponse response = webmateClient_.RetrieveIncompleteOrderHeaderSection(
                                                                                    out incompleteHeader,
                                                                                    inOrderHandle);
            if (!response.Succesful)
                throw new Exception(response.Message);
        }
    }
}
