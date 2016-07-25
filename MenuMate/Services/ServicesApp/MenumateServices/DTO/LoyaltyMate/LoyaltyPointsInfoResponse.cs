using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace MenumateServices.DTO.LoyaltyMate
{
    [DataContract]
    public class LoyaltyPointsInfoResponse : MMServiceResponse
    {
        LoyaltyResponseCode _responseCode;
        PointsInfo _pointsInfo;

        [DataMember]
        public LoyaltyResponseCode ResponseCode
        {
            get { return _responseCode; }
            set { _responseCode = value; }
        }

        [DataMember]
        public PointsInfo PointsInfo
        {
            get { return _pointsInfo; }
            set { _pointsInfo = value; }
        }


        internal LoyaltyPointsInfoResponse(
                        bool inSuccesful,
                        string inMessage,
                        string inDescription,
                        LoyaltyResponseCode inResponseCode,
                        PointsInfo inPointsInfo)
            : base(inSuccesful, inMessage, inDescription)
        {
            _responseCode = inResponseCode;
            _pointsInfo = CreatePointsInfo(inPointsInfo);
        }

        public static LoyaltyPointsInfoResponse CreateResponseNoError(PointsInfo inPointsInfo)
        {
            return CreateResponse(true, "", "", LoyaltyResponseCode.Successful, inPointsInfo);
        }

        public static LoyaltyPointsInfoResponse CreateResponseError(
                                       string inMessage,
                                       string inDescription,
                                       LoyaltyResponseCode inResponseCode,
                                       PointsInfo inPointsInfo)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode, inPointsInfo);
        }

        public static LoyaltyPointsInfoResponse CreateResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                PointsInfo inPointsInfo)
        {
            return new LoyaltyPointsInfoResponse(
                                inSuccesful,
                                inMessage,
                                inDescription,
                                inResponseCode,
                                inPointsInfo);
        }

        PointsInfo CreatePointsInfo(PointsInfo inPointsInfo)
        {
            var result = new PointsInfo();
            result.Balance = inPointsInfo.Balance;
            result.EndDate = inPointsInfo.EndDate;
            result.StartDate = inPointsInfo.EndDate;
            result.PointsType = inPointsInfo.PointsType;
            result.UniqueId = inPointsInfo.UniqueId;
            return result;
        }
    }
}
