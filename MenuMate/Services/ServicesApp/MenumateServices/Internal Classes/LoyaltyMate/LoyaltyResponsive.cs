using System.Collections.Generic;
using MenumateServices.DTO.LoyaltyMate;

namespace MenumateServices.LoyaltyMate
{
    public class LoyaltyResponsive
    {
        #region Public

        protected LoyaltyResponse CreateResponseNoError()
        {
            return CreateResponse(true, "", "", LoyaltyResponseCode.Successful);
        }

        protected LoyaltyResponse CreateResponseError(
                                            string inMessage,
                                            string inDescription,
                                            LoyaltyResponseCode inResponseCode)
        {
            return CreateResponse(false, inMessage, inDescription, inResponseCode);
        }

        protected LoyaltyMemberResponse CreateMemberResponseNoError(MemberInfo inMemberInfo)
        {
            return CreateMemberResponse(true, "", "", LoyaltyResponseCode.Successful, inMemberInfo);
        }


        protected LoyaltyMemberResponse CreateMemberResponseError(
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                MemberInfo inMemberInfo)
        {
            return CreateMemberResponse(false, inMessage, inDescription, inResponseCode, inMemberInfo);
        }


        protected LoyaltyMemberListResponse CreateMemberListResponseNoError(List<MemberInfo> inMemberInfoList)
        {
            return CreateMemberListResponse(true, "", "", LoyaltyResponseCode.Successful, inMemberInfoList);
        }

        protected LoyaltyMemberListResponse CreateMemberListResponseError(
                                                    string inMessage,
                                                    string inDescription,
                                                    LoyaltyResponseCode inResponseCode)
        {
            // Empty list
            var emptyList = new List<MemberInfo>();

            return CreateMemberListResponse(false, inMessage, inDescription, inResponseCode, emptyList);
        }


        protected LoyaltyTierResponse CreateTierResponseNoError(TierLevelInfo inTierInfo)
        {
            return CreateTierResponse(true, "", "", LoyaltyResponseCode.Successful, inTierInfo);
        }


        protected LoyaltyTierResponse CreateTierResponseError(
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                TierLevelInfo inTierInfo)
        {
            return CreateTierResponse(false, inMessage, inDescription, inResponseCode, inTierInfo);
        }



        protected LoyaltyTierListResponse CreateTierListResponseNoError(List<TierLevelInfo> inTierInfoList)
        {
            return CreateTierListResponse(true, "", "", LoyaltyResponseCode.Successful, inTierInfoList);
        }


        protected LoyaltyTierListResponse CreateTierListResponseError(
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                List<TierLevelInfo> inTierInfoList)
        {
            return CreateTierListResponse(false, inMessage, inDescription, inResponseCode, inTierInfoList);
        }





        protected LoyaltyPointsInfoResponse CreatePointsInfoResponseNoError(PointsInfo inPointsInfo)
        {
            return CreatePointsInfoResponse(true, "", "", LoyaltyResponseCode.Successful, inPointsInfo);
        }

        protected LoyaltyPointsInfoResponse CreatePointsInfoResponseError(
                                                       string inMessage,
                                                       string inDescription,
                                                       LoyaltyResponseCode inResponseCode,
                                                       PointsInfo inPointsInfo)
        {
            return CreatePointsInfoResponse(false, inMessage, inDescription, inResponseCode, inPointsInfo);
        }


        protected LoyaltyResponse CreateResponse(
                                            bool inSuccesful,
                                            string inMessage,
                                            string inDescription,
                                            LoyaltyResponseCode inResponseCode)
        {
            return LoyaltyResponse.CreateResponse(
                                           inSuccesful,
                                           inMessage,
                                           inDescription,
                                           inResponseCode);
        }

        protected LoyaltyMemberResponse CreateMemberResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                MemberInfo inMemberInfo)
        {
            return LoyaltyMemberResponse.CreateResponse(
                                                 inSuccesful,
                                                 inMessage,
                                                 inDescription,
                                                 inResponseCode,
                                                 inMemberInfo);
        }


        protected LoyaltyMemberListResponse CreateMemberListResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                List<MemberInfo> inMemberInfoList)
        {
            return LoyaltyMemberListResponse.CreateResponse(
                                                    inSuccesful,
                                                    inMessage,
                                                    inDescription,
                                                    inResponseCode,
                                                    inMemberInfoList.ToArray());
        }

        protected LoyaltyTierResponse CreateTierResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                TierLevelInfo inTierInfo)
        {
            return LoyaltyTierResponse.CreateResponse(
                                                 inSuccesful,
                                                 inMessage,
                                                 inDescription,
                                                 inResponseCode,
                                                 inTierInfo);
        }


        protected LoyaltyTierListResponse CreateTierListResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                List<TierLevelInfo> inTierInfoList)
        {
            return LoyaltyTierListResponse.CreateResponse(
                                                 inSuccesful,
                                                 inMessage,
                                                 inDescription,
                                                 inResponseCode,
                                                 inTierInfoList.ToArray());
        }



        protected LoyaltyPointsInfoResponse CreatePointsInfoResponse(
                                                bool inSuccesful,
                                                string inMessage,
                                                string inDescription,
                                                LoyaltyResponseCode inResponseCode,
                                                PointsInfo inPointsInfo)
        {
            return LoyaltyPointsInfoResponse.CreateResponse(
                                                 inSuccesful,
                                                 inMessage,
                                                 inDescription,
                                                 inResponseCode,
                                                 inPointsInfo);
        }


        #endregion
    }
}
