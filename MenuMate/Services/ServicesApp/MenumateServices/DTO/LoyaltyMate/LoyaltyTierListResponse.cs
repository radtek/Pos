using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyTierListResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        TierLevelInfo[] _tierLevelList;

        internal LoyaltyTierListResponse(
                        bool inSuccesful,
                        string inMessage,
                        string inDescription,
                        LoyaltyResponseCode inResponseCode,
                        TierLevelInfo[] inTierLevelInfoList)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _tierLevelList = CreateTierLevelInfoList(inTierLevelInfoList);
        }

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public TierLevelInfo[] TierLevelList
        {
            get { return _tierLevelList; }
            set { _tierLevelList = value; }
        }


        public static LoyaltyTierListResponse CreateResponseNoError(TierLevelInfo[] inTierLevelInfoList)
        {
            return CreateResponse(true, "", "", LoyaltyResponseCode.Successful, inTierLevelInfoList);
        }

        public static LoyaltyTierListResponse CreateResponseError(
                                                        string inMessage,
                                                        string inDescription,
                                                        LoyaltyResponseCode inResponseCode,
                                                        TierLevelInfo[] inTierLevelInfoList)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode, inTierLevelInfoList);
        }


        public static LoyaltyTierListResponse CreateResponse(
                                                        bool inSuccesful,
                                                        string inMessage,
                                                        string inDescription,
                                                        LoyaltyResponseCode inResponseCode,
                                                        TierLevelInfo[] inTierLevelInfoList)
        {
            return new LoyaltyTierListResponse(
                                inSuccesful,
                                inMessage,
                                inDescription,
                                inResponseCode,
                                inTierLevelInfoList);
        }


        TierLevelInfo[] CreateTierLevelInfoList(TierLevelInfo[] inTierLevelInfoList)
        {
            List<TierLevelInfo> result = new List<TierLevelInfo>();

            foreach (TierLevelInfo info in inTierLevelInfoList)
            {
                result.Add(info);
            }

            return result.ToArray();
        }
    
    
    
    
    }
}
