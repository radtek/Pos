using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.Serialization;
using System.ServiceModel;

namespace MenumateServices.DTO.LoyaltyMate
{
    public enum LoyaltyResponseCode
    {
        Successful = 0,
        AuthenticationFailed,
        CreateMemberFailed,
        UpdateMemberFailed,
        DeleteMemberFailed,
        GetMemberFailed,
        PostTransactionFailed,
        CreateTierFailed,
        UpdateTierFailed,
        DeleteTierFailed,
        MemberNotExist,
    };

    /// <summary>
    /// Describes a Menumate's LoyaltyMate Service Response
    ///     Succesful:    True if the function succeeds 
    ///     Message:      Response's message
    ///     Description:  Detailed description of the response's message
    ///     ResponseCode: The transaction's response code.  Take a look at "enum ResponseCode"
    /// </summary>
    [DataContract]
    public class LoyaltyResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;

        /// <summary>
        /// 
        /// </summary>
        internal LoyaltyResponse(
                        bool inSuccesful,
                        string inMessage,
                        string inDescription,
                        LoyaltyResponseCode inResponseCode)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
        }

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static LoyaltyResponse CreateResponseNoError()
        {
            return CreateResponse(true, "", "", LoyaltyResponseCode.Successful);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        public static LoyaltyResponse CreateResponseError(
                                        string inMessage,
                                        string inDescription,
                                        LoyaltyResponseCode inResponseCode)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inSuccesful"></param>
        /// <param name="inMessage"></param>
        /// <param name="inDescription"></param>
        /// <param name="inResponseCode"></param>
        /// <returns></returns>
        public static LoyaltyResponse CreateResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode)
        {
            return new LoyaltyResponse(
                                inSuccesful,
                                inMessage,
                                inDescription,
                                inResponseCode);
        }
    }
}
